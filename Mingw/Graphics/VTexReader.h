#ifndef VIRTUAL_TEXTURE_READER_H
#define VIRTUAL_TEXTURE_READER_H
#include "VTexRegistery.h"
#include <fstream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <iostream>

// ============================================================
//  VtexReader
//  Reads individual tiles from .vtex files on demand.
//  Designed to be called from the streaming thread.
//  Thread-safe — each read opens/closes its own file handle,
//  or uses a cached open handle per texture.
// ============================================================

class VTexReader {
public:

    // Read a single tile from a .vtex file into outPixels
    // vtId    = texture ID from GTextureRegistry
    // tileX   = tile column  (in tile units, not pixels)
    // tileY   = tile row     (in tile units, not pixels)
    // mip     = mip level (0 = full res)
    // Returns true on success, false on failure
    bool readTile(int vtId, int tileX, int tileY, int mip,
                  std::vector<uint8_t>& outPixels)
    {
        // Get the .vtex filepath from the registry
        const VTexEntry& entry = GTextureRegistry.get(vtId);
        const std::string& path = entry.vtexPath;

        // Open the file
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "[VtexReader] Failed to open: " << path << "\n";
            return false;
        }

        // Read header
        VtexHeader header;
        file.read(reinterpret_cast<char*>(&header), sizeof(VtexHeader));

        // Validate magic
        if (strncmp(header.magic, "VTEX", 4) != 0) {
            std::cerr << "[VtexReader] Invalid magic in: " << path << "\n";
            return false;
        }

        // Validate mip
        if (mip >= header.mipLevels) {
            std::cerr << "[VtexReader] Mip " << mip
                      << " out of range (max " << header.mipLevels - 1
                      << ") in: " << path << "\n";
            return false;
        }

        // Calculate tile dimensions at this mip level
        int mipW = std::max(1, header.width  >> mip); // width  at this mip
        int mipH = std::max(1, header.height >> mip); // height at this mip
        int tilesX = (mipW + VTEX_TILE_SIZE - 1) / VTEX_TILE_SIZE;
        int tilesY = (mipH + VTEX_TILE_SIZE - 1) / VTEX_TILE_SIZE;

        // Validate tile coordinates
        if (tileX >= tilesX || tileY >= tilesY) {
            std::cerr << "[VtexReader] Tile (" << tileX << "," << tileY
                      << ") out of range at mip " << mip << "\n";
            return false;
        }

        // Calculate the global tile index
        // We need to sum up all tiles in previous mips first
        int globalTileIndex = 0;
        for (int m = 0; m < mip; m++) {
            int mW = std::max(1, header.width  >> m);
            int mH = std::max(1, header.height >> m);
            int tX = (mW + VTEX_TILE_SIZE - 1) / VTEX_TILE_SIZE;
            int tY = (mH + VTEX_TILE_SIZE - 1) / VTEX_TILE_SIZE;
            globalTileIndex += tX * tY;
        }
        globalTileIndex += tileY * tilesX + tileX;

        // Seek to the tile index entry for this tile
        // Tile index starts right after the header
        int64_t tileIndexStart = sizeof(VtexHeader);
        int64_t tileEntryOffset = tileIndexStart
                                + globalTileIndex * sizeof(VTexTileEntry);

        file.seekg(tileEntryOffset);

        // Read the tile index entry
        VTexTileEntry entry2;
        file.read(reinterpret_cast<char*>(&entry2), sizeof(VTexTileEntry));

        if (entry2.dataSize <= 0) {
            std::cerr << "[VtexReader] Invalid tile entry at index "
                      << globalTileIndex << "\n";
            return false;
        }

        // Seek to tile pixel data and read it
        file.seekg(entry2.offset);
        outPixels.resize(entry2.dataSize);
        file.read(reinterpret_cast<char*>(outPixels.data()), entry2.dataSize);

        if (!file) {
            std::cerr << "[VtexReader] Read failed for tile ("
                      << tileX << "," << tileY << ") mip " << mip << "\n";
            return false;
        }

        return true;
    }

    // Convenience: how many tiles wide is this texture at a given mip?
    int tilesX(int vtId, int mip) {
        const VTexEntry& entry = GTextureRegistry.get(vtId);
        int mipW = std::max(1, entry.width >> mip);
        return (mipW + VTEX_TILE_SIZE - 1) / VTEX_TILE_SIZE;
    }

    // Convenience: how many tiles tall is this texture at a given mip?
    int tilesY(int vtId, int mip) {
        const VTexEntry& entry = GTextureRegistry.get(vtId);
        int mipH = std::max(1, entry.height >> mip);
        return (mipH + VTEX_TILE_SIZE - 1) / VTEX_TILE_SIZE;
    }
};

// Global reader — the streaming thread uses this
extern VTexReader GVtexReader;

#endif