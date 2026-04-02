#ifndef VIRTUAL_TEXTURE_SYSTEM_H
#define VIRTUAL_TEXTURE_SYSTEM_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <list>
#include <algorithm>
#include "VTexReader.h"

// ============================================================
//  Constants
// ============================================================
static constexpr int VT_PHYSICAL_SIZE    = 8192;  // physical texture size in pixels
static constexpr int VT_PHYSICAL_TILES   = VT_PHYSICAL_SIZE / VTEX_TILE_SIZE; // 32x32 = 1024 slots
static constexpr int VT_PAGE_TABLE_SIZE  = VT_GRID_SIZE; // 512x512

// ============================================================
//  Texture layers
// ============================================================
enum TextureLayer {
    LAYER_ALBEDO    = 0,
    LAYER_NORMAL    = 1,
    LAYER_ROUGHNESS  = 2,
    LAYER_EMISSIVE  = 3,
    LAYER_COUNT     = 4
};

// ============================================================
//  TileID — uniquely identifies one tile to load
// ============================================================
struct TileID {
    uint16_t x, y;   // virtual tile coordinates (0-511)
    uint8_t  mip;
    uint8_t  layer;  // which TextureLayer

    bool operator==(const TileID& o) const {
        return x==o.x && y==o.y && mip==o.mip && layer==o.layer;
    }
    bool operator<(const TileID& o) const {
        if (x != o.x)     return x < o.x;
        if (y != o.y)     return y < o.y;
        if (mip != o.mip) return mip < o.mip;
        return layer < o.layer;
    }
};

struct TileIDHash {
    size_t operator()(const TileID& t) const {
        return std::hash<uint64_t>()(
            (uint64_t)t.x              |
            ((uint64_t)t.y    << 16)   |
            ((uint64_t)t.mip  << 32)   |
            ((uint64_t)t.layer << 40)
        );
    }
};

// ============================================================
//  Physical slot — a location in the physical texture
// ============================================================
struct PhysicalSlot {
    uint16_t x, y;   // tile coordinate in physical texture (0-31)
    bool     valid = false;
};

// ============================================================
//  Page table entry — stored as RGBA8 in the page table texture
//  R = physical tile X
//  G = physical tile Y
//  B = mip level that was loaded
//  A = 255 if valid, 0 if not loaded yet
// ============================================================
struct PageTableEntry {
    uint8_t physX, physY, mip, valid;
};

// ============================================================
//  PhysicalCache — LRU eviction for one physical texture
//  Manages VT_PHYSICAL_TILES * VT_PHYSICAL_TILES slots (1024)
// ============================================================
class PhysicalCache {
public:
    PhysicalCache() {
        int total = VT_PHYSICAL_TILES * VT_PHYSICAL_TILES;
        m_slots.resize(total);

        // Pre-fill all slots with their physical coordinates
        for (int i = 0; i < total; i++) {
            m_slots[i].slot.x     = (uint16_t)(i % VT_PHYSICAL_TILES);
            m_slots[i].slot.y     = (uint16_t)(i / VT_PHYSICAL_TILES);
            m_slots[i].slot.valid = true;
            m_slots[i].occupied   = false;
        }
    }

    // Allocate a fresh slot — only works until cache is full
    bool allocate(PhysicalSlot& out) {
        if (m_nextFree >= (int)m_slots.size()) return false;
        int idx = m_nextFree++;
        out = m_slots[idx].slot;

        m_lruList.push_front(idx);
        m_lruMap[idx] = m_lruList.begin();
        return true;
    }

    // Evict the least recently used slot
    // evictedOut tells you which TileID was there so you can
    // invalidate it in the page table
    PhysicalSlot evict(TileID* evictedOut) {
        int idx = m_lruList.back();
        m_lruList.pop_back();
        m_lruMap.erase(idx);

        if (evictedOut) *evictedOut = m_slots[idx].tile;
        m_slots[idx].occupied = false;

        // Put back at front as MRU — it's about to be reused
        m_lruList.push_front(idx);
        m_lruMap[idx] = m_lruList.begin();

        return m_slots[idx].slot;
    }

    // Mark a slot as recently used — called when feedback pass
    // sees a tile that's already loaded
    void touch(const PhysicalSlot& slot) {
        int idx = slot.y * VT_PHYSICAL_TILES + slot.x;
        auto it = m_lruMap.find(idx);
        if (it != m_lruMap.end()) {
            m_lruList.erase(it->second);
            m_lruList.push_front(idx);
            m_lruMap[idx] = m_lruList.begin();
            m_slots[idx].occupied = true;
        }
    }

private:
    struct Entry {
        PhysicalSlot slot;
        TileID       tile;
        bool         occupied = false;
    };

    std::vector<Entry>                        m_slots;
    std::list<int>                            m_lruList;
    std::unordered_map<int,
        std::list<int>::iterator>             m_lruMap;
    int                                       m_nextFree = 0;
};

// ============================================================
//  VirtualTextureSystem
// ============================================================
class VTexSystem {
public:
    void init();
    void processFeedback(GLuint feedbackFBO, int fbWidth, int fbHeight);
    void flushUploads();
    void bind(int albedoUnit, int normalUnit, int roughnessUnit,
              int emissiveUnit, int pageTableUnit);
    void shutdown();
    int loadedTileCount() const { return (int)m_loadedTiles.size(); }
    int queueSize() {
        std::lock_guard<std::mutex> lock(m_requestMutex);
        return (int)m_requestQueue.size();
    }
    void requestTile(const TileID& id, int priority);
    std::vector<int> m_tileToVtId;
private:
    // One physical texture per layer
    GLuint m_physicalTex[LAYER_COUNT] = {};

    // One cache per layer — each manages its own 1024 slots
    PhysicalCache m_cache[LAYER_COUNT];

    // One shared page table — all layers use the same virtual coords
    GLuint m_pageTableTex [LAYER_COUNT] = {};
    std::vector<PageTableEntry> m_pageTableCPU[LAYER_COUNT]; // CPU mirror for partial updates

    // PBO for async feedback readback
    GLuint m_feedbackPBO = 0;
    bool   m_pboReady    = false;
    bool   m_pboAllocated = false;
    // Which tiles are currently loaded, per layer
    std::unordered_map<TileID, PhysicalSlot, TileIDHash> m_loadedTiles;

    // Streaming
    std::thread      m_streamThread;
    std::atomic<bool> m_running{false};

    struct UploadRequest {
        TileID               id;
        TileID               originalId;
        PhysicalSlot         slot;
        std::vector<uint8_t> data;
    };

    std::mutex m_requestMutex;
    std::priority_queue<
        std::pair<int, TileID>,         // priority, tile
        std::vector<std::pair<int,TileID>>,
        std::greater<std::pair<int,TileID>>
    > m_requestQueue;

    std::mutex                 m_uploadMutex;
    std::vector<UploadRequest> m_pendingUploads;

    std::unordered_map<TileID, bool, TileIDHash> m_inFlight;
    std::vector<uint8_t> m_feedbackPixels;
    // Internal helpers
    const VTexEntry* findEntryForTile(const TileID&);
    void streamThreadFunc();
    void uploadTile(const UploadRequest& req);
    void updatePageTable(const TileID& id, const PhysicalSlot& slot);
    void invalidatePageTable(const TileID& id);
    PhysicalSlot acquireSlot(const TileID& id);
    std::vector<TileID> parseFeedbackBuffer(
        const std::vector<uint8_t>& pixels, int w, int h);
};

extern VTexSystem GVirtualTextureSystem;
#endif