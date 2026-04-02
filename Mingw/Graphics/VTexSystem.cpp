#include "VTexSystem.h"
void VTexSystem::init() {
    // ---- Physical textures (one per layer) ----
    // Each layer gets its own format since they store different data
    GLint internalFormats[LAYER_COUNT] = {
        GL_RGBA8,   // ALBEDO    — full color, needs all 4 channels
        GL_RGBA8,     // NORMAL    — only RG needed, reconstruct Z in shader
        GL_RGBA8,      // ROUGHNESS — single channel
        GL_RGBA8    // EMISSIVE  — full color
    };
    for (int i = 0; i < LAYER_COUNT; i++) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_physicalTex[i]);
        glTextureStorage2D(m_physicalTex[i], 1, internalFormats[i],
                        VT_PHYSICAL_SIZE,VT_PHYSICAL_SIZE);
        glTextureParameteri(m_physicalTex[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_physicalTex[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_physicalTex[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_physicalTex[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    for (int i = 0; i < LAYER_COUNT; i++) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_pageTableTex[i]);
        glTextureStorage2D(m_pageTableTex[i], 1, GL_RGBA8, VT_PAGE_TABLE_SIZE, VT_PAGE_TABLE_SIZE);
        glTextureParameteri(m_pageTableTex[i], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_pageTableTex[i], GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        m_pageTableCPU[i].assign(VT_PAGE_TABLE_SIZE * VT_PAGE_TABLE_SIZE, {0,0,0,0});
            
        std::vector<uint8_t> zeros(VT_PAGE_TABLE_SIZE * VT_PAGE_TABLE_SIZE * 4, 0);
        glTextureSubImage2D(m_pageTableTex[i], 0, 0, 0,
                        VT_PAGE_TABLE_SIZE, VT_PAGE_TABLE_SIZE,
                        GL_RGBA, GL_UNSIGNED_BYTE,
                        zeros.data());
    }

    // ---- PBO for async feedback readback ----
    glGenBuffers(1, &m_feedbackPBO);
    
    // ---- Start streaming thread ----
    m_running = true;
    m_streamThread = std::thread(&VTexSystem::streamThreadFunc, this);
    std::cout << "[VT] Initialized.\n"
              << "     Physical texture: "
              << VT_PHYSICAL_TILES << "x" << VT_PHYSICAL_TILES
              << " tiles per layer (" << LAYER_COUNT << " layers)\n"
              << "     Page table: "
              << VT_PAGE_TABLE_SIZE << "x" << VT_PAGE_TABLE_SIZE << "\n";
    m_tileToVtId.assign(VT_GRID_SIZE * VT_GRID_SIZE, -1);
    for (int i = 0; i < GTextureRegistry.count(); i++) {
        const VTexEntry& e = GTextureRegistry.get(i);
        for (int ty = 0; ty < e.region.tilesH; ty++)
            for (int tx = 0; tx < e.region.tilesW; tx++)
                m_tileToVtId[(e.region.originY + ty) * VT_GRID_SIZE + (e.region.originX + tx)] = i;
    }
}
void VTexSystem::shutdown() {
    // Stop streaming thread
    m_running = false;
    if (m_streamThread.joinable())
        m_streamThread.join();

    // Delete all physical textures
    for (int i = 0; i < LAYER_COUNT; i++) {
        glDeleteTextures(1, &m_pageTableTex[i]);
        if (m_physicalTex[i]) {
            glDeleteTextures(1, &m_physicalTex[i]);
            m_physicalTex[i] = 0;
        }
    }

    // Delete PBO
    if (m_feedbackPBO) {
        glDeleteBuffers(1, &m_feedbackPBO);
        m_feedbackPBO = 0;
    }
}
PhysicalSlot VTexSystem::acquireSlot(const TileID& id) {
    PhysicalSlot slot;

    // Try to allocate a fresh slot from this layer's cache
    if (!m_cache[id.layer].allocate(slot)) {
        // Cache full — evict the LRU tile from this layer
        TileID evicted;
        slot = m_cache[id.layer].evict(&evicted);

        // Remove from loaded map
        m_loadedTiles.erase(evicted);

        // Invalidate its page table entry
        invalidatePageTable(evicted);
    }
    return slot;
}
void VTexSystem::updatePageTable(
    const TileID& id, const PhysicalSlot& slot)
{
    PageTableEntry& entry = m_pageTableCPU[id.layer][id.y * VT_PAGE_TABLE_SIZE + id.x];
    entry.physX  = (uint8_t)slot.x;
    entry.physY  = (uint8_t)slot.y;
    entry.mip    = id.mip;
    entry.valid  = 255;
}

void VTexSystem::invalidatePageTable(const TileID& id) {
    m_pageTableCPU[id.layer][id.y * VT_PAGE_TABLE_SIZE + id.x] = {0,0,0,0};
}
void VTexSystem::uploadTile(const UploadRequest& req) {
    
    int destX = req.slot.x * VTEX_TILE_SIZE;
    int destY = req.slot.y * VTEX_TILE_SIZE;

    // Pick the right format for this layer
    GLenum formats[LAYER_COUNT] = {
        GL_RGBA,    // ALBEDO
        GL_RGBA,      // NORMAL
        GL_RGBA,
        GL_RGBA     // EMISSIVE
    };
    glTextureSubImage2D(m_physicalTex[req.id.layer], 0, destX, destY,
                    VTEX_TILE_SIZE, VTEX_TILE_SIZE,
                    formats[req.id.layer],
                    GL_UNSIGNED_BYTE,
                    req.data.data());
}
void VTexSystem::requestTile(const TileID& id, int priority) {
    // Already loaded?
    {
        std::lock_guard<std::mutex> lock(m_uploadMutex);
        if (m_loadedTiles.count(id)) return;
    }

    // Already being streamed?
    std::lock_guard<std::mutex> lock(m_requestMutex);
    if (m_inFlight.count(id)) return;

    m_inFlight[id] = true;
    m_requestQueue.push({ priority, id });
}
void VTexSystem::streamThreadFunc() {
    
    while (m_running) {
        std::pair<int, TileID> req;
        bool hasWork = false;

        {
            std::lock_guard<std::mutex> lock(m_requestMutex);
            if (!m_requestQueue.empty()) {
                req     = m_requestQueue.top();
                m_requestQueue.pop();
                hasWork = true;
            }
        }

        if (!hasWork) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        const TileID& id = req.second;
        
        // Convert virtual tile coords → vtId + local tile coords
        // by looking up which texture owns this virtual region
        
        const VTexEntry* entry = findEntryForTile(id);
        if (!entry) continue; // no texture owns this region
        
        int localX = id.x - (entry->region.originX >> id.mip);
        int localY = id.y - (entry->region.originY >> id.mip);
        // Load the tile from disk
        UploadRequest upload;
        
        upload.originalId   = id;
        upload.id   = id;
        upload.id.layer = entry->layer;
        upload.slot = acquireSlot(id);
        
        bool ok = GVtexReader.readTile( entry->id, localX, localY, id.mip, upload.data);
        
        if (!ok) {
            std::lock_guard<std::mutex> lock(m_requestMutex);
            m_inFlight.erase(id);
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(m_uploadMutex);
            m_pendingUploads.push_back(std::move(upload));
        }
    }
}
const VTexEntry* VTexSystem::findEntryForTile(const TileID& id) {
    int mx = id.x << id.mip;
    int my = id.y << id.mip;
    int idx = my * VT_GRID_SIZE + mx;
    if (idx < 0 || idx >= (int)m_tileToVtId.size()) return nullptr;
    int vtId = m_tileToVtId[idx];
    if (vtId < 0) return nullptr;
    return &GTextureRegistry.get(vtId);
}
void VTexSystem::processFeedback(GLuint feedbackFBO, int fbWidth, int fbHeight){
    size_t bufSize = (size_t)fbWidth * fbHeight * 4;

    if (!m_pboAllocated) {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_feedbackPBO);
        glBufferData(GL_PIXEL_PACK_BUFFER, bufSize, nullptr, GL_STREAM_READ);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        m_pboAllocated = true;
    }

    // Read previous frame's PBO — GPU has had a full frame to complete it
    if (m_pboReady) {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_feedbackPBO);
        uint8_t* ptr = (uint8_t*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        if (ptr) {
            m_feedbackPixels.assign(ptr, ptr + bufSize);
            glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
            auto needed = parseFeedbackBuffer(m_feedbackPixels, fbWidth, fbHeight);
            for (int i = 0; i < (int)needed.size(); i++)
                requestTile(needed[i], i);
        }
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }

    // Kick off async read for this frame — returns immediately
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_feedbackPBO);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, feedbackFBO);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, fbWidth, fbHeight, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    m_pboReady = true;
}
std::vector<TileID> VTexSystem::parseFeedbackBuffer(
    const std::vector<uint8_t>& pixels, int w, int h)
{
    std::unordered_map<TileID, int, TileIDHash> counts;
    
    int alpha0 = 0, alpha1 = 0, alpha2 = 0, alpha3 = 0, alpha4 = 0, alphaOther = 0;
    for (int i = 0; i < w * h; i++) {
        uint8_t r = pixels[i*4 + 0]; // tile X
        uint8_t g = pixels[i*4 + 1]; // tile Y
        uint8_t b = pixels[i*4 + 2]; // mip
        uint8_t a = pixels[i*4 + 3]; // layer (0=albedo etc), 255=invalid/sky
        
        
        if (a == 0 || a<25) continue; // background pixel, skip
        int layer = -1;
        if      (a >= 25  && a < 75)  layer = 0;  // ~51
        else if (a >= 75  && a < 125) layer = 1;  // ~102
        else if (a >= 125 && a < 175) layer = 2;  // ~153
        else if (a >= 175 && a < 225) layer = 3;  // ~204
        else continue; // invalid
        if (r >= VT_GRID_SIZE) continue;
        if (g >= VT_GRID_SIZE) continue;
        if (b >= 8) continue;
        TileID id;
        id.x     = r;
        id.y     = g;
        id.mip   = b;
        id.layer = (uint8_t)layer;
        counts[id]++;
    }
    //std::cout << "[VT] total: " << total << std::endl;
    //std::cout << "[VT] noMaterial=" << noMaterial << " hasMaterial=" << hasMaterial << "\n" << std::flush;
    // Sort by coverage — most visible tiles first
    std::vector<std::pair<TileID, int>> sorted(counts.begin(), counts.end());
    std::sort(sorted.begin(), sorted.end(),
        [](auto& a, auto& b){ return a.second > b.second; });
    //std::cout << "[VT] Unique tiles needed: " << counts.size() << "\n" << std::flush;
    std::vector<TileID> result;
    result.reserve(sorted.size());
    for (auto& [id, _] : sorted)
        result.push_back(id);
    return result;
}

void VTexSystem::flushUploads() {
    static int frame = 0;
    if (frame++ % 60 == 0) {
        std::lock_guard<std::mutex> lock(m_uploadMutex);
        //std::cout << "[VT] loadedTiles=" << m_loadedTiles.size() << " pendingUploads=" << m_pendingUploads.size() << " inFlight=" << m_inFlight.size() << "\n" << std::flush;
    }
    std::vector<UploadRequest> uploads;
    {
        std::lock_guard<std::mutex> lock(m_uploadMutex);
        uploads.swap(m_pendingUploads);
    }
    
    // Cap uploads per frame to avoid spikes
    static constexpr int MAX_UPLOADS_PER_FRAME = 32;
    int count = 0;
    bool anyUploaded[LAYER_COUNT] = { false, false, false, false };
    for (auto& req : uploads) {
        if (count++ >= MAX_UPLOADS_PER_FRAME) {
            // Put remaining back
            std::lock_guard<std::mutex> lock(m_uploadMutex);
            m_pendingUploads.insert(
                m_pendingUploads.begin(),
                uploads.begin() + count - 1,
                uploads.end());
            break;
        }
        uploadTile(req);
        m_loadedTiles[req.originalId] = req.slot;
        updatePageTable(req.id, req.slot);
        anyUploaded[req.id.layer] = true;
        {
            std::lock_guard<std::mutex> lock(m_requestMutex);
            m_inFlight.erase(req.originalId);
        }
    }

    for (int i = 0; i < LAYER_COUNT; i++) {
        if (!anyUploaded[i]) continue; // skip if nothing changed
    glTextureSubImage2D(m_pageTableTex[i], 0, 0, 0,
                        VT_PAGE_TABLE_SIZE, VT_PAGE_TABLE_SIZE,
                        GL_RGBA, GL_UNSIGNED_BYTE,
                        m_pageTableCPU[i].data());
    }
    
}

void VTexSystem::bind(int albedoUnit, int normalUnit, int roughnessUnit,
                      int emissiveUnit, int pageTableUnit)
{
    glActiveTexture(GL_TEXTURE0 + albedoUnit);
    glBindTexture(GL_TEXTURE_2D, m_physicalTex[LAYER_ALBEDO]);

    glActiveTexture(GL_TEXTURE0 + normalUnit);
    glBindTexture(GL_TEXTURE_2D, m_physicalTex[LAYER_NORMAL]);

    glActiveTexture(GL_TEXTURE0 + roughnessUnit);
    glBindTexture(GL_TEXTURE_2D, m_physicalTex[LAYER_ROUGHNESS]);

    glActiveTexture(GL_TEXTURE0 + emissiveUnit);
    glBindTexture(GL_TEXTURE_2D, m_physicalTex[LAYER_EMISSIVE]);

    // One page table per layer
    for (int i = 0; i < LAYER_COUNT; i++) {
        glActiveTexture(GL_TEXTURE0 + pageTableUnit + i);
        glBindTexture(GL_TEXTURE_2D, m_pageTableTex[i]);
    }
}