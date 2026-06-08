#version 460 core

out vec4 FragColor;

struct Material {
    vec4 baseColorFactor;
    vec4 emissiveFactor;

    vec2 regionSize;
    vec2 albedoRegionOrigin;
    vec2 normalRegionOrigin;
    vec2 metallicRegionOrigin;
    vec2 emissiveRegionOrigin;

    float metallicFactor;
    float roughnessFactor;
    float normalScale;
    float occlusionStrength;
    float alphaCutoff;

    int albedoImageIndex;
    int metallicRoughnessImageIndex;
    int normalImageIndex;
    int emissiveImageIndex;
    int doubleSided;
    int alphaMode;
    int  pad1;
    int  pad2;
    int  pad3;
};

layout(binding = 5, std430) readonly buffer materials {
    Material uMaterials[];
};

in DATA {
    vec3 FragPos;
    vec3 vNormals;
    vec4 color;
    vec2 texCoords;
    flat int materialIndex;
} fs_in;


uniform float uPhysicalSize;
uniform float uTileSize;
uniform float uPageTableSize;

uniform vec3  lightPos;
uniform vec3  lightDir;
uniform vec3  viewPos;
uniform vec3  lightColor;
uniform float lightCutoff      = 30.0;
uniform float lightOuterCutoff = 45.0;
precision mediump float;
const float PI = 3.14159265359;

void main() {
    FragColor = uMaterials[fs_in.materialIndex].baseColorFactor;
}