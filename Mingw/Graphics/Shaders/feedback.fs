#version 460 core

in vec2 v_texCoords;
flat in int v_materialIndex;

struct Material {
    vec4  baseColorFactor;
    vec4  emissiveFactor;
    vec2  regionSize;
    vec2  albedoRegionOrigin;
    vec2  normalRegionOrigin;
    vec2  metallicRegionOrigin;
    vec2  emissiveRegionOrigin;
    float metallicFactor;
    float roughnessFactor;
    float normalScale;
    float occlusionStrength;
    float alphaCutoff;
    int   albedoImageIndex;
    int   metallicRoughnessImageIndex;
    int   normalImageIndex;
    int   emissiveImageIndex;
    int   doubleSided;
    int   alphaMode;
    int   pad1;
    int   pad2;
    int   pad3;
};

layout(binding = 5, std430) readonly buffer materials {
    Material uMaterials[];
};

uniform float uPageTableSize; // 512.0

// 4 outputs — one per layer
layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outRoughness;
layout(location = 3) out vec4 outEmissive;

// Encodes a tile address into RGBA8
// R=tileX G=tileY B=mip A=layer (255=invalid)
vec4 encodeTile(vec2 regionOrigin, vec2 regionSize, int layer) {
    vec2 tileCoord = regionOrigin + v_texCoords * regionSize;
    vec2 dx  = dFdx(v_texCoords * regionSize);
    vec2 dy  = dFdy(v_texCoords * regionSize);
    float mip = clamp(floor(log2(max(length(dx), length(dy)))), 0.0, 7.0);
    tileCoord = floor(tileCoord / pow(2.0, mip));
    tileCoord = clamp(tileCoord, vec2(0.0), vec2(uPageTableSize - 1.0));
    return vec4(
        tileCoord.x / 255.0,
        tileCoord.y / 255.0,
        mip / 255.0,
        float(layer + 1.0) / 5.0  // layer+1 so layer 0 = alpha 1, never 0
    );
}
/*
void main() {
    // DEBUG: output materialIndex as color
    // if materialIndex >= 0, output green
    // if materialIndex < 0, output red
    if (fs_in.materialIndex < 0) {
        outAlbedo = vec4(1.0, 0.0, 0.0, 1.0); // red = no material
    } else {
        outAlbedo = vec4(0.0, float(fs_in.materialIndex), 0.0, 1.0); // green = has material
    }
    outNormal    = vec4(0.0);
    outRoughness = vec4(0.0);
    outEmissive  = vec4(0.0);
}*/
void main() {
    // A=255 means invalid/background — parseFeedbackBuffer skips these
    vec4 invalid = vec4(0.0, 0.0, 0.0, 0.0);

    if (v_materialIndex < 0) {
        outAlbedo    = invalid;
        outNormal    = invalid;
        outRoughness = invalid;
        outEmissive  = invalid;
        return;
    }
        Material mat = uMaterials[v_materialIndex];

        outAlbedo    = (mat.albedoImageIndex >= 0) ? encodeTile(mat.albedoRegionOrigin,    mat.regionSize, 0): invalid;

        outNormal    = (mat.normalImageIndex >= 0) ? encodeTile(mat.normalRegionOrigin,    mat.regionSize, 1) : invalid;

        outRoughness = (mat.metallicRoughnessImageIndex >= 0) ? encodeTile(mat.metallicRegionOrigin,  mat.regionSize, 2) : invalid;

        outEmissive  = (mat.emissiveImageIndex >= 0) ? encodeTile(mat.emissiveRegionOrigin,  mat.regionSize, 3) : invalid;
    
}