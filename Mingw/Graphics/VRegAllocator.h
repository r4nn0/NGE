#ifndef VIRTUAL_REGION_ALLOCATOR_H
#define VIRTUAL_REGION_ALLOCATOR_H
#include <iostream>
#include <vector>

// ============================================================
//  VirtualRegionAllocator
//
//  Carves up the 512x512 virtual tile grid and assigns each
//  texture a fixed rectangular region.
//
//  Strategy: simple row-based shelf packer.
//  We fill left to right, and when a row is full we start
//  a new row below it at the height of the tallest texture
//  in that row.
//
//  Example for a 512x512 grid:
//
//  +----32----+--16--+--16--+-- ...
//  |          |      |      |
//  32  tex0   | tex1 | tex2 |   <- shelf height = 32
//  |  4096px  |2048px|2048px|
//  +----------+------+------+
//  |                        |
//  16        tex3            |   <- shelf height = 16
//  |        2048px           |
//  +------------------------+
//  ...
// ============================================================

static constexpr int VT_GRID_SIZE = 1024; // 512x512 tile grid
static constexpr int VTEX_TILE_SIZE = 128;
static constexpr int VTEX_VERSION   = 1;

struct VirtualRegion {
    int originX = 0;  // tile coordinate of top-left corner in virtual grid
    int originY = 0;
    int tilesW  = 0;  // width  in tiles
    int tilesH  = 0;  // height in tiles
    bool valid  = false;
};

class VRegAllocator {
public:
    VRegAllocator()
        : m_cursorX(0), m_cursorY(0), m_shelfHeight(0) {}

    // Allocate a region for a texture of given pixel dimensions
    // Returns a VirtualRegion with the assigned tile coordinates
    VirtualRegion allocate(int textureW, int textureH) {
        // How many tiles does this texture need?
        int tilesW = (textureW + VTEX_TILE_SIZE - 1) / VTEX_TILE_SIZE;
        int tilesH = (textureH + VTEX_TILE_SIZE - 1) / VTEX_TILE_SIZE;

        // Does it fit on the current shelf?
        if (m_cursorX + tilesW > VT_GRID_SIZE) {
            // Move to next shelf
            m_cursorX     = 0;
            m_cursorY    += m_shelfHeight;
            m_shelfHeight = 0;
        }

        // Check if we've run out of virtual space entirely
        if (m_cursorY + tilesH > VT_GRID_SIZE) {
            std::cerr << "[VT] Virtual address space exhausted! "
                      << "Consider increasing VT_GRID_SIZE.\n";
            return { 0, 0, 0, 0, false };
        }

        // Assign region
        VirtualRegion region;
        region.originX = m_cursorX;
        region.originY = m_cursorY;
        region.tilesW  = tilesW;
        region.tilesH  = tilesH;
        region.valid   = true;

        // Advance cursor
        m_cursorX    += tilesW;
        m_shelfHeight = std::max(m_shelfHeight, tilesH);

        std::cout << "[VT] Allocated region: origin=("
                  << region.originX << "," << region.originY
                  << ") size=(" << tilesW << "x" << tilesH << ")\n";

        return region;
    }

private:
    int m_cursorX;      // current X position on the current shelf
    int m_cursorY;      // Y position of the current shelf
    int m_shelfHeight;  // tallest texture on the current shelf
};

extern VRegAllocator GRegionAllocator;
#endif