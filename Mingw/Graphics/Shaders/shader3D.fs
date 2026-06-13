#version 460 core
#extension GL_ARB_gpu_shader_int64 : require
#extension GL_ARB_bindless_texture : require

// ============================================================================
// STRUCTURE DEFINITIONS
// ============================================================================

struct Material {
    vec4 baseColorFactor;
    vec4 emissiveFactor;
    
    sampler2D albedoHandle;
    sampler2D metallicHandle;
    sampler2D normalHandle;
    sampler2D emissiveHandle;
    
    float metallicFactor;
    float roughnessFactor;
    float normalScale;
    float occlusionStrength;
    float alphaCutoff;
    
    int doubleSided;
    int alphaMode;
    int hasTexture;
};

struct Light {
    vec3 position;
    float intensity;
    vec3 color;
    float radius;
};

// ============================================================================
// INPUT/OUTPUT
// ============================================================================

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

out vec4 FragColor;

// ============================================================================
// UNIFORMS
// ============================================================================

uniform vec3  uLightPos;
uniform vec3  uLightDir;
uniform vec3  uViewPos;
uniform vec3  uLightColor;
uniform float uLightIntensity;
uniform float uAmbientStrength;
uniform int   uLightMode;

// ============================================================================
// CONSTANTS
// ============================================================================

const float PI = 3.14159265359;
const float EPSILON = 1e-6;

// ============================================================================
// TEXTURE HELPER FUNCTIONS
// ============================================================================

vec4 sampleTexture(sampler2D texHandle, vec2 uv, int textureFlag, int flagBit) {
    if ((textureFlag & flagBit) != 0) {
        return texture(texHandle, uv);
    }
    return vec4(1.0);
}

vec3 sampleTextureRGB(sampler2D texHandle, vec2 uv, int textureFlag, int flagBit) {
    return sampleTexture(texHandle, uv, textureFlag, flagBit).rgb;
}

// ============================================================================
// NORMAL MAPPING
// ============================================================================

vec3 unpackNormal(vec3 packedNormal) {
    return normalize(packedNormal * 2.0 - 1.0);
}

vec3 perturbNormal(vec3 baseNormal, vec3 normalMapSample, float normalScale) {
    vec3 perturbedNormal = unpackNormal(normalMapSample);
    perturbedNormal.xy *= normalScale;
    return normalize(baseNormal + perturbedNormal);
}

// ============================================================================
// PBR HELPER FUNCTIONS
// ============================================================================

// Normal Distribution Function (GGX/Trowbridge-Reitz)
float distributionGGX(float NdotH, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
    return a2 / (PI * denom * denom + EPSILON);
}

// Geometry Function (Schlick-GGX)
float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0) * 0.5;
    float k = r * r / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k + EPSILON);
}

float geometrySmith(float NdotV, float NdotL, float roughness) {
    return geometrySchlickGGX(NdotV, roughness) * geometrySchlickGGX(NdotL, roughness);
}

// Fresnel-Schlick Approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// ============================================================================
// LIGHTING CALCULATIONS
// ============================================================================

vec3 calculatePBR(
    vec3 N,
    vec3 V,
    vec3 L,
    vec3 albedo,
    float metallic,
    float roughness,
    vec3 lightColor,
    float lightIntensity
) {
    vec3 H = normalize(V + L);
    
    float NdotV = clamp(dot(N, V), EPSILON, 1.0);
    float NdotL = clamp(dot(N, L), 0.0, 1.0);
    float NdotH = clamp(dot(N, H), 0.0, 1.0);
    float VdotH = clamp(dot(V, H), 0.0, 1.0);
    
    // Fresnel term
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = fresnelSchlick(VdotH, F0);
    
    // PBR components
    float D = distributionGGX(NdotH, roughness);
    float G = geometrySmith(NdotV, NdotL, roughness);
    
    // Cook-Torrance BRDF
    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);
    
    vec3 specular = (D * F * G) / (4.0 * NdotV * NdotL + EPSILON);
    vec3 diffuse = kD * albedo / PI;
    
    // Outgoing radiance
    return (diffuse + specular) * lightColor * lightIntensity * NdotL;
}

// ============================================================================
// MAIN SHADER
// ============================================================================

void main() {
    Material mat = uMaterials[fs_in.materialIndex];
    
    // Sample textures with proper bit flags
    vec4 albedoSample = sampleTexture(mat.albedoHandle, fs_in.texCoords, mat.hasTexture, 1);
    vec3 metallicRoughnessAO = sampleTextureRGB(mat.metallicHandle, fs_in.texCoords, mat.hasTexture, 2);
    vec3 normalMapSample = sampleTextureRGB(mat.normalHandle, fs_in.texCoords, mat.hasTexture, 4);
    vec3 emissiveSample = sampleTextureRGB(mat.emissiveHandle, fs_in.texCoords, mat.hasTexture, 8);
    
    // Combine material factors with vertex color
    vec4 finalAlbedo = mat.baseColorFactor * albedoSample * fs_in.color;
    
    // Alpha testing
    if (mat.alphaMode == 1 && finalAlbedo.a < mat.alphaCutoff) {
        discard;
    }
    
    // Extract PBR parameters
    float metallic = metallicRoughnessAO.r * mat.metallicFactor;
    float roughness = clamp(metallicRoughnessAO.g * mat.roughnessFactor, 0.04, 1.0);
    float ao = mix(1.0, metallicRoughnessAO.b, mat.occlusionStrength);
    
    // Normal mapping
    vec3 N = perturbNormal(normalize(fs_in.vNormals), normalMapSample, mat.normalScale);
    vec3 V = normalize(uViewPos - fs_in.FragPos);
    
    // Light direction and attenuation
    vec3 L;
    float attenuation = 1.0;
    
    if (uLightMode == 0) {
        // Directional light
        L = normalize(-uLightDir);
    } else {
        // Point light with quadratic falloff
        vec3 lightVector = uLightPos - fs_in.FragPos;
        float distance = length(lightVector);
        L = normalize(lightVector);
        attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
    }
    
    // PBR calculation
    vec3 radiance = calculatePBR(N, V, L, finalAlbedo.rgb, metallic, roughness, uLightColor, uLightIntensity);
    
    // Emissive contribution
    vec3 emissive = emissiveSample * mat.emissiveFactor.rgb;
    
    // Ambient and AO
    vec3 ambient = vec3(uAmbientStrength) * finalAlbedo.rgb * ao;
    
    // Final color
    vec3 finalColor = ambient + (radiance * attenuation) + emissive;
    
    // Tone mapping
    finalColor = finalColor / (finalColor + vec3(1.0));
    finalColor = pow(finalColor, vec3(1.0 / 2.2));
    
    FragColor = vec4(finalColor, finalAlbedo.a);
}
