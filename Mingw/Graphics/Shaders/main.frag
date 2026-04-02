#version 460 core

in vec2  vUV;
in vec3  vNormal;
in vec3  vWorldPos;
in vec4  vColor;

// ============================================================
//  Virtual texture samplers
// ============================================================
uniform sampler2D uPhysicalAlbedo;
uniform sampler2D uPhysicalNormal;
uniform sampler2D uPhysicalRoughness;
uniform sampler2D uPhysicalMetallic;
uniform sampler2D uPhysicalEmissive;
uniform sampler2D uPageTable;          // shared page table for all layers

// ============================================================
//  Virtual texture parameters (same for all layers)
// ============================================================
uniform vec2  uRegionOrigin;    // tile coord of this texture's region origin
uniform vec2  uRegionSize;      // size of region in tiles
uniform float uPhysicalSize;    // 4096.0
uniform float uTileSize;        // 128.0
uniform float uPageTableSize;   // 512.0

// ============================================================
//  Material factors
// ============================================================
uniform vec4  uBaseColorFactor;
uniform float uMetallicFactor;
uniform float uRoughnessFactor;
uniform vec4  uEmissiveFactor;
uniform float uNormalScale;
uniform int   uAlphaMode;       // 0=OPAQUE 1=MASK 2=BLEND
uniform float uAlphaCutoff;

// ============================================================
//  Lighting
// ============================================================
uniform vec3 uCameraPos;
uniform vec3 uLightDir;         // directional light direction (normalized)
uniform vec3 uLightColor;
uniform vec3 uAmbientColor;

// Which layers are actually present for this material
uniform bool uHasAlbedo;
uniform bool uHasNormal;
uniform bool uHasRoughness;
uniform bool uHasMetallic;
uniform bool uHasEmissive;

out vec4 fragColor;

// ============================================================
//  Core virtual texture lookup
//  Returns the physical UV to sample for a given virtual UV
//  Returns vec3 so we can also return a "valid" flag (w=1 or 0)
// ============================================================
vec3 vtexLookup(vec2 uv) {
    // Which tile are we in?
    vec2 tileCoord = uRegionOrigin + uv * uRegionSize;

    // Estimate mip
    vec2 dx = dFdx(uv * uRegionSize);
    vec2 dy = dFdy(uv * uRegionSize);
    float mip = floor(log2(max(length(dx), length(dy))));
    mip = clamp(mip, 0.0, 7.0);

    // Coarsen tile coord for mip
    tileCoord = floor(tileCoord / pow(2.0, mip));
    tileCoord = clamp(tileCoord, vec2(0.0), vec2(uPageTableSize - 1.0));

    // Page table lookup (NEAREST — no interpolation)
    vec2 pageTableUV = (tileCoord + 0.5) / uPageTableSize;
    vec4 pageEntry   = texture(uPageTable, pageTableUV);

    // Not loaded yet?
    if (pageEntry.a < 0.5)
        return vec3(0.0); // signal: not loaded

    // Decode physical tile location
    vec2 physTile = floor(pageEntry.rg * 255.0 + 0.5);

    // UV within the tile (0..1)
    vec2 inTileUV = fract((uRegionOrigin + uv * uRegionSize) / pow(2.0, mip));

    // Bias slightly inward to prevent tile edge bleeding
    float border = 0.5 / uTileSize;
    inTileUV = inTileUV * (1.0 - 2.0 * border) + border;

    // Final physical UV
    vec2 physUV = (physTile * uTileSize + inTileUV * uTileSize) / uPhysicalSize;

    return vec3(physUV, 1.0); // xy=UV, z=valid
}

// ============================================================
//  PBR — simple direct lighting
// ============================================================
const float PI = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a  = roughness * roughness;
    float a2 = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float geometrySchlick(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    return geometrySchlick(max(dot(N,V), 0.0), roughness)
         * geometrySchlick(max(dot(N,L), 0.0), roughness);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// ============================================================
//  Main
// ============================================================
void main() {
    vec3 lookup = vtexLookup(vUV);
    bool vtexValid = lookup.z > 0.5;
    vec2 physUV   = lookup.xy;

    // ---- Albedo ----
    vec4 albedo = uBaseColorFactor * vColor;
    if (uHasAlbedo && vtexValid)
        albedo *= texture(uPhysicalAlbedo, physUV);

    // Alpha test
    if (uAlphaMode == 1 && albedo.a < uAlphaCutoff)
        discard;

    // ---- Normal ----
    vec3 N = normalize(vNormal);
    if (uHasNormal && vtexValid) {
        // Normal map stored as RG (reconstruct Z)
        vec2 rg     = texture(uPhysicalNormal, physUV).rg * 2.0 - 1.0;
        float z     = sqrt(max(0.0, 1.0 - dot(rg, rg)));
        vec3 tangentNormal = vec3(rg * uNormalScale, z);

        // Transform from tangent space to world space
        // Simple approximation using screen-space derivatives
        vec3 dPdx   = dFdx(vWorldPos);
        vec3 dPdy   = dFdy(vWorldPos);
        vec2 dUVdx  = dFdx(vUV);
        vec2 dUVdy  = dFdy(vUV);

        float det = dUVdx.x * dUVdy.y - dUVdx.y * dUVdy.x;
        vec3 T = normalize((dPdx * dUVdy.y - dPdy * dUVdx.y) / det);
        vec3 B = normalize((dPdy * dUVdx.x - dPdx * dUVdy.x) / det);
        mat3 TBN = mat3(T, B, N);

        N = normalize(TBN * tangentNormal);
    }

    // ---- Roughness / Metallic ----
    float roughness = uRoughnessFactor;
    float metallic  = uMetallicFactor;

    if (vtexValid) {
        if (uHasRoughness)
            roughness *= texture(uPhysicalRoughness, physUV).r;
        if (uHasMetallic)
            metallic  *= texture(uPhysicalMetallic,  physUV).r;
    }

    roughness = clamp(roughness, 0.04, 1.0);
    metallic  = clamp(metallic,  0.0,  1.0);

    // ---- PBR lighting ----
    vec3 V   = normalize(uCameraPos - vWorldPos);
    vec3 L   = normalize(-uLightDir);
    vec3 H   = normalize(V + L);

    vec3 F0  = mix(vec3(0.04), albedo.rgb, metallic);

    float NDF = distributionGGX(N, H, roughness);
    float G   = geometrySmith(N, V, L, roughness);
    vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator   = NDF * G * F;
    float denominator = 4.0 * max(dot(N,V), 0.0) * max(dot(N,L), 0.0) + 0.0001;
    vec3 specular    = numerator / denominator;

    vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);
    vec3 Lo     = (kD * albedo.rgb / PI + specular) * uLightColor * NdotL;

    // ---- Ambient + emissive ----
    vec3 ambient = uAmbientColor * albedo.rgb;

    vec3 emissive = vec3(0.0);
    if (uHasEmissive && vtexValid)
        emissive = texture(uPhysicalEmissive, physUV).rgb * uEmissiveFactor.rgb;

    vec3 color = ambient + Lo + emissive;

    // ---- Tone mapping + gamma ----
    color = color / (color + vec3(1.0));           // Reinhard
    color = pow(color, vec3(1.0 / 2.2));           // gamma correction

    fragColor = vec4(color, albedo.a);
}
