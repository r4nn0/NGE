#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "tiny_gltf.h"
#include "Graphics/VTexSystem.h"

bool generateVtex(const std::string& srcPath, const std::string& dstPath) {

    struct stat st;
    if (stat(dstPath.c_str(), &st) == 0) {
        //std::cout << "[VT] Already exists: " << dstPath << "\n";
        return true;
    }

    std::cout << "[VT] Processing: " << srcPath << " -> " << dstPath << "\n" << std::flush;

    // Load source image — force 4 channels (RGBA)
    int srcW, srcH, srcChannels;
    uint8_t* pixels = stbi_load(srcPath.c_str(), &srcW, &srcH, &srcChannels, 4);
    if (!pixels) {
        std::cerr << "[VT] Failed to load: " << srcPath
                  << " — " << stbi_failure_reason() << "\n";
        return false;
    }

    // Build mip chain
    // mip 0 = full res, mip 1 = half res, etc.
    int mipLevels = (int)std::floor(std::log2(std::max(srcW, srcH))) + 1;
    mipLevels = std::min(mipLevels, 8); // cap at 8 mips

    // Collect all mip levels as raw pixel buffers
    std::vector<std::vector<uint8_t>> mips(mipLevels);
    std::vector<int> mipW(mipLevels), mipH(mipLevels);

    // Mip 0 = original pixels
    mipW[0] = srcW;
    mipH[0] = srcH;
    mips[0].assign(pixels, pixels + srcW * srcH * 4);
    stbi_image_free(pixels);

    // Generate remaining mips by downsampling
    for (int m = 1; m < mipLevels; m++) {
        mipW[m] = std::max(1, mipW[m-1] / 2);
        mipH[m] = std::max(1, mipH[m-1] / 2);
        mips[m] = downsample(mips[m-1].data(), mipW[m-1], mipH[m-1]);
    }

    // Count total tiles across all mips
    int totalTiles = 0;
    struct MipInfo { int tilesX, tilesY, tileOffset; };
    std::vector<MipInfo> mipInfo(mipLevels);

    for (int m = 0; m < mipLevels; m++) {
        mipInfo[m].tilesX      = (mipW[m] + VTEX_TILE_SIZE - 1) / VTEX_TILE_SIZE;
        mipInfo[m].tilesY      = (mipH[m] + VTEX_TILE_SIZE - 1) / VTEX_TILE_SIZE;
        mipInfo[m].tileOffset  = totalTiles; // index of first tile for this mip
        totalTiles += mipInfo[m].tilesX * mipInfo[m].tilesY;
    }

    // ---- Write .vtex file ----
    std::ofstream file(dstPath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[VT] Failed to open for write: " << dstPath << "\n";
        return false;
    }

    // Write header
    VtexHeader header;
    memcpy(header.magic, "VTEX", 4);
    header.version   = VTEX_VERSION;
    header.width     = srcW;
    header.height    = srcH;
    header.tileSize  = VTEX_TILE_SIZE;
    header.mipLevels = mipLevels;
    header.tileCount = totalTiles;
    file.write(reinterpret_cast<char*>(&header), sizeof(VtexHeader));

    // Reserve space for tile index (we'll fill it in after writing tile data)
    int64_t tileIndexOffset = file.tellp();
    std::vector<VTexTileEntry> tileIndex(totalTiles, {0, 0});
    file.write(reinterpret_cast<char*>(tileIndex.data()),
               totalTiles * sizeof(VTexTileEntry));

    // Write tile data + record offsets
    int tileSize2D = VTEX_TILE_SIZE * VTEX_TILE_SIZE * 4; // bytes per tile

    for (int m = 0; m < mipLevels; m++) {
        for (int ty = 0; ty < mipInfo[m].tilesY; ty++) {
            for (int tx = 0; tx < mipInfo[m].tilesX; tx++) {
                int globalTileIdx = mipInfo[m].tileOffset + ty * mipInfo[m].tilesX + tx;

                // Record where this tile starts
                tileIndex[globalTileIdx].offset   = (int64_t)file.tellp();
                tileIndex[globalTileIdx].dataSize = tileSize2D;

                // Extract and write tile pixels
                auto tilePixels = extractTile(
                    mips[m].data(), mipW[m], mipH[m], tx, ty);

                file.write(reinterpret_cast<char*>(tilePixels.data()), tileSize2D);
            }
        }
    }

    // Go back and write the real tile index now that we have all offsets
    file.seekp(tileIndexOffset);
    file.write(reinterpret_cast<char*>(tileIndex.data()),
               totalTiles * sizeof(VTexTileEntry));

    file.close();

    std::cout << "[VT] Done: " << totalTiles << " tiles, "
              << mipLevels << " mip levels\n";
    return true;
}

VRegAllocator GRegionAllocator;
VTexRegistery GTextureRegistry;
VTexReader GVtexReader;
VTexSystem GVirtualTextureSystem;