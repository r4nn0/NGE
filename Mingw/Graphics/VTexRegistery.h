#ifndef VIRTUAL_TEXTURE_REGISTERY_H
#define VIRTUAL_TEXTURE_REGISTERY_H
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdint>
#include "VRegAllocator.h"


struct VtexHeader {
    char  magic[4];    // "VTEX"
    int   version;
    int   width;
    int   height;
    int   tileSize;
    int   mipLevels;
    int   tileCount;
};

struct VTexTileEntry {
    int64_t offset;    // byte offset from start of file
    int32_t dataSize;  // size in bytes
};

struct VTexEntry {
    int         id;
    int         layer;
    std::string vtexPath;
    int         width;
    int         height;
    VirtualRegion region;
};
static std::vector<uint8_t> downsample(
    const uint8_t* src, int srcW, int srcH)
{
    int dstW = std::max(1, srcW / 2);
    int dstH = std::max(1, srcH / 2);
    std::vector<uint8_t> dst(dstW * dstH * 4);

    for (int y = 0; y < dstH; y++) {
        for (int x = 0; x < dstW; x++) {
            for (int c = 0; c < 4; c++) {
                // Average 2x2 block
                int x0 = x * 2,     y0 = y * 2;
                int x1 = std::min(x0 + 1, srcW - 1);
                int y1 = std::min(y0 + 1, srcH - 1);

                int p00 = src[(y0 * srcW + x0) * 4 + c];
                int p10 = src[(y0 * srcW + x1) * 4 + c];
                int p01 = src[(y1 * srcW + x0) * 4 + c];
                int p11 = src[(y1 * srcW + x1) * 4 + c];

                dst[(y * dstW + x) * 4 + c] = (uint8_t)((p00 + p10 + p01 + p11) / 4);
            }
        }
    }
    return dst;
}
static std::vector<uint8_t> extractTile(
    const uint8_t* src, int srcW, int srcH,
    int tileX, int tileY)
{
    std::vector<uint8_t> tile(VTEX_TILE_SIZE * VTEX_TILE_SIZE * 4, 0);

    int pixelX = tileX * VTEX_TILE_SIZE;
    int pixelY = tileY * VTEX_TILE_SIZE;

    for (int row = 0; row < VTEX_TILE_SIZE; row++) {
        int srcRow = pixelY + row;
        if (srcRow >= srcH) break;

        for (int col = 0; col < VTEX_TILE_SIZE; col++) {
            int srcCol = pixelX + col;
            if (srcCol >= srcW) break;

            int srcIdx  = (srcRow * srcW + srcCol) * 4;
            int dstIdx  = (row * VTEX_TILE_SIZE + col) * 4;

            tile[dstIdx + 0] = src[srcIdx + 0];
            tile[dstIdx + 1] = src[srcIdx + 1];
            tile[dstIdx + 2] = src[srcIdx + 2];
            tile[dstIdx + 3] = src[srcIdx + 3];
        }
    }
    return tile;
}

bool generateVtex(const std::string&, const std::string&);


class VTexRegistery {
public:

    int registerTexture(const std::string& filepath, int width, int height) {
        auto it = m_pathToId.find(filepath);
        if (it != m_pathToId.end())
            return it->second;

        int id = (int)m_entries.size();
        m_pathToId[filepath] = id;
        

        std::string vtexPath = filepath.substr(0, filepath.find(".")) + ".vtex";
        generateVtex(filepath, vtexPath);
        
        VirtualRegion region = GRegionAllocator.allocate(width, height);
        if (region.tilesW == 0) {
            std::cerr << "[VT] Failed to allocate region for: " << filepath << "\n";
            return -1;
        }
        m_entries.push_back({ id, 0, vtexPath, width, height, region});
        return id;
    }
    void setLayer(int id, int layer) {
        m_entries[id].layer = layer;
    }
    const VTexEntry& get(int id) const {
        return m_entries[id];
    }

    int count() const { return (int)m_entries.size(); }

private:
    std::vector<VTexEntry>        m_entries;
    std::unordered_map<std::string, int>    m_pathToId;
};

extern VTexRegistery GTextureRegistry;

#endif