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

uniform sampler2D uPhysicalAlbedo;
uniform sampler2D uPhysicalNormal;
uniform sampler2D uPhysicalRoughness;
uniform sampler2D uPhysicalEmissive;
uniform sampler2D uPageTableAlbedo;
uniform sampler2D uPageTableNormal;
uniform sampler2D uPageTableRoughness;
uniform sampler2D uPageTableEmissive;
uniform sampler2D uShadowMap;
uniform mat4      uLightSpaceMatrix;
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

float calculateShadow(vec3 fragPos, vec3 N, vec3 L) {
    vec4 fragPosLightSpace = uLightSpaceMatrix * vec4(fragPos, 1.0);
    
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    projCoords = projCoords * 0.5 + 0.5;
    
    if (projCoords.z > 1.0) return 0.0;
    if (projCoords.x < 0.0 || projCoords.x > 1.0) return 0.0;
    if (projCoords.y < 0.0 || projCoords.y > 1.0) return 0.0;
    
    float currentDepth = projCoords.z;
    
    float bias = max(0.005 * (1.0 - dot(N, L)), 0.001);
    
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float pcfDepth = texture(uShadowMap, 
                projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    return shadow;
}

vec3 vtexLookup(vec2 uv, vec2 regionOrigin, vec2 regionSize, sampler2D pageTable, vec2 dx, vec2 dy) {
    
    float mipF = clamp(floor(log2(max(length(dx * regionSize), length(dy * regionSize)))), 0.0, 7.0);
    float mipScale = exp2(mipF);

    vec2 localTile = floor(uv * regionSize / mipScale);
    vec2 mipRegionSize = max(vec2(1.0), floor(regionSize / mipScale));
    localTile = clamp(localTile, vec2(0.0), mipRegionSize - 1.0);

    vec2 tileCoord = regionOrigin + localTile;
    tileCoord = clamp(tileCoord, vec2(0.0), vec2(uPageTableSize - 1.0));

    vec2 pageUV    = (tileCoord + 0.5) / uPageTableSize;
    vec4 pageEntry = texture(pageTable, pageUV);

    if (pageEntry.a < 0.5) return vec3(0.0);

    vec2 physTile = floor(pageEntry.rg * 255.0 + 0.5);
    vec2 inTileUV = fract(uv * regionSize / mipScale);
    float border  = 0.5 / uTileSize;
    inTileUV      = inTileUV * (1.0 - 2.0 * border) + border;

    vec2 physUV = (physTile * uTileSize + inTileUV * uTileSize) / uPhysicalSize;
    return vec3(physUV, 1.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    return GeometrySchlickGGX(max(dot(N, V), 0.0), roughness) * GeometrySchlickGGX(max(dot(N, L), 0.0), roughness);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
vec3 ACESFilmic(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}
void main() {
    vec3 N = normalize(fs_in.vNormals);
    vec4 baseColor = fs_in.color;
    vec3 ambient = vec3(0.5) * baseColor.rgb;
    float metallic  = 0.0;
    float roughness = 0.5;
    vec3  emissiveFactor = vec3(0.0);
    vec2 dx = dFdx(fs_in.texCoords);
    vec2 dy = dFdy(fs_in.texCoords);

    float skyFactor = dot(N, vec3(0.0, 1.0, 0.0)) * 0.5 + 0.5;
    vec3 skyColor    = vec3(0.4, 0.5, 0.6);
    vec3 groundColor = vec3(0.2, 0.18, 0.15);
    if (fs_in.materialIndex >= 0) {
        Material mat = uMaterials[fs_in.materialIndex];

        if (mat.albedoImageIndex >= 0) {
            vec3 lookup = vtexLookup(fs_in.texCoords, mat.albedoRegionOrigin, mat.regionSize, uPageTableAlbedo, dx ,dy);
            if (lookup.z > 0.5){
                baseColor *= texture(uPhysicalAlbedo, lookup.xy);
            }
        }

        baseColor *= mat.baseColorFactor;
        ambient = mix(groundColor, skyColor, skyFactor) * baseColor.rgb;
        ambient = max(ambient, vec3(0.15) * baseColor.rgb);
        if (mat.alphaMode == 1 && baseColor.a < mat.alphaCutoff){
            discard;
        }

        metallic  = mat.metallicFactor;
        roughness = mat.roughnessFactor;

        if (mat.metallicRoughnessImageIndex >= 0) {
            vec3 mrLookup = vtexLookup(fs_in.texCoords, mat.metallicRegionOrigin, mat.regionSize, uPageTableRoughness, dx ,dy);
            if (mrLookup.z > 0.5) {
                vec3 mr    = texture(uPhysicalRoughness, mrLookup.xy).rgb;
                ambient   *= mr.r * mat.occlusionStrength;
                roughness *= mr.g;               
                metallic  *= mr.b;
            }
        }

        roughness = clamp(roughness, 0.04, 1.0);
        metallic  = clamp(metallic,  0.0,  1.0);

        if (mat.normalImageIndex >= 0) {
            vec3 nLookup = vtexLookup(fs_in.texCoords, mat.normalRegionOrigin, mat.regionSize, uPageTableNormal, dx ,dy);
            if (nLookup.z > 0.5) {
                vec2 rg   = texture(uPhysicalNormal, nLookup.xy).rg * 2.0 - 1.0;
                float z   = sqrt(max(0.0, 1.0 - dot(rg, rg)));
                vec3 tn   = vec3(rg * mat.normalScale, z);

                vec3 dPdx = dFdx(fs_in.FragPos);
                vec3 dPdy = dFdy(fs_in.FragPos);
                vec2 dUdx = dFdx(fs_in.texCoords);
                vec2 dUdy = dFdy(fs_in.texCoords);
                float det = dUdx.x * dUdy.y - dUdx.y * dUdy.x;
                vec3 T    = normalize((dPdx * dUdy.y - dPdy * dUdx.y) / det);
                vec3 B    = normalize((dPdy * dUdx.x - dPdx * dUdy.x) / det);
                N         = normalize(mat3(T, B, N) * tn);
            }
        }

        emissiveFactor = mat.emissiveFactor.rgb;
        if (mat.emissiveImageIndex >= 0) {
            vec3 eLookup = vtexLookup(fs_in.texCoords, mat.emissiveRegionOrigin, mat.regionSize, uPageTableEmissive, dx ,dy);
            if (eLookup.z > 0.5){
                emissiveFactor *= texture(uPhysicalEmissive, eLookup.xy).rgb;
            }
        }
    }

    vec3 V   = normalize(viewPos - fs_in.FragPos);
    vec3 L   = lightDir;
    vec3 H   = normalize(V + L);
    vec3 F0  = mix(vec3(0.04), baseColor.rgb, metallic);

    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3  F   = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3  numerator   = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3  specular    = numerator / denominator;

    vec3  kD    = (vec3(1.0) - F) * (1.0 - metallic);
    float NdotL = max(dot(N, L), 0.0);

    if (NdotL <= 0.0) {
        vec3 result = ambient + emissiveFactor;
        result = ACESFilmic(result);
        result = pow(result, vec3(1.0 / 2.2));
        FragColor = vec4(result, baseColor.a);
        return;
    }

    float shadow = calculateShadow(fs_in.FragPos, N, L);
    vec3 Lo     = (kD * baseColor.rgb / PI + specular) * lightColor * NdotL * (1.0 - shadow);
    vec3 result = ambient + Lo + emissiveFactor;

    
    result = ACESFilmic(result);
    result = pow(result, vec3(1.0 / 2.2));

    FragColor = vec4(result, baseColor.a);
}