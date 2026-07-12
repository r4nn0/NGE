#version 460 core
#extension GL_ARB_gpu_shader_int64 : require
#extension GL_ARB_bindless_texture : require


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
    int alphaMode;   // 0 = opaque, 1 = mask, 2 = blend
    int hasTexture;  // bit 1 = albedo, bit 2 = metallicRoughness(+AO), bit 4 = normal, bit 8 = emissive
};

layout(binding = 5, std430) readonly buffer materials {
    Material uMaterials[];
};

in DATA {
    vec3 FragPos;
    vec3 vNormals;
    vec4 vTangent;
    vec4 color;
    vec2 texCoords;
    flat int materialIndex;
} fs_in;

out vec4 FragColor;


uniform int   uLightMode;
uniform vec3  uLightDir;
uniform vec3  uLightPos;
uniform vec3  uViewPos;
uniform vec3  uLightColor;
uniform float uLightIntensity;
uniform float uAmbientStrength;
uniform float uLightRadius;
uniform float uSpotInnerCutOff;
uniform float uSpotOuterCutOff;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return a2 / max(denom, 0.0000001);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float denom = NdotV * (1.0 - k) + k;
    return NdotV / max(denom, 0.0000001);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 getNormalFromMap(Material mat, vec3 N, vec2 uv, vec4 tangentIn)
{
    vec3 tangentNormal = texture(mat.normalHandle, uv).xyz * 2.0 - 1.0;
    tangentNormal.xy *= mat.normalScale;

    vec3 T = normalize(tangentIn.xyz - dot(tangentIn.xyz, N) * N);
    vec3 B = cross(N, T) * tangentIn.w;

    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangentNormal);
}


void main()
{
    Material material = uMaterials[fs_in.materialIndex];

    vec4 albedo = material.baseColorFactor * fs_in.color;

    if ((material.hasTexture & 1) != 0)
    {
        vec4 albedoSample = texture(material.albedoHandle, fs_in.texCoords);
        albedoSample.rgb = pow(albedoSample.rgb, vec3(2.2));
        albedo *= albedoSample;
    }

    if (material.alphaMode == 1 && albedo.a < material.alphaCutoff)
    {
        discard;
    }

    float metallic  = material.metallicFactor;
    float roughness = material.roughnessFactor;
    float occlusion = 1.0;

    if ((material.hasTexture & 2) != 0)
    {
        vec3 orm = texture(material.metallicHandle, fs_in.texCoords).rgb;
        occlusion = mix(1.0, orm.r, material.occlusionStrength);
        roughness *= orm.g;
        metallic  *= orm.b;
    }
    roughness = clamp(roughness, 0.04, 1.0);
    metallic  = clamp(metallic, 0.0, 1.0);

    vec3 N = normalize(fs_in.vNormals);
    if (!gl_FrontFacing && material.doubleSided != 0)
    {
        N = -N;
    }

    if ((material.hasTexture & 4) != 0)
    {
        N = getNormalFromMap(material, N, fs_in.texCoords, fs_in.vTangent);
    }

    vec3 emissive = material.emissiveFactor.rgb;
    if ((material.hasTexture & 8) != 0)
    {
        vec3 emissiveSample = texture(material.emissiveHandle, fs_in.texCoords).rgb;
        emissive *= pow(emissiveSample, vec3(2.2));
    }

    vec3 albedoLinear = albedo.rgb;
    vec3 V = normalize(uViewPos - fs_in.FragPos);
    vec3 F0 = mix(vec3(0.04), albedoLinear, metallic);

    vec3 L = normalize(uLightPos - fs_in.FragPos);
    float attenuation = 1.0;

    if (uLightMode == 0)
    {
        L = normalize(-uLightDir);
    }
    else if (uLightMode == 1)
    {
        
        vec3 toLight   = uLightPos - fs_in.FragPos;
        float distance = length(toLight);
        L = toLight / max(distance, 0.0001);

        float distRatio = distance / max(uLightRadius, 0.0001);
        float rangeAtten = clamp(1.0 - distRatio * distRatio * distRatio * distRatio, 0.0, 1.0);
        attenuation = (rangeAtten * rangeAtten) / (distance * distance + 1.0);
    }
    else
    {
        
        vec3 toLight   = uLightPos - fs_in.FragPos;
        float distance = length(toLight);
        L = toLight / max(distance, 0.0001);

        float distRatio  = distance / max(uLightRadius, 0.0001);
        float rangeAtten = clamp(1.0 - distRatio * distRatio * distRatio * distRatio, 0.0, 1.0);
        rangeAtten = (rangeAtten * rangeAtten) / (distance * distance + 1.0);

        float theta   = dot(L, normalize(-uLightDir));
        float epsilon = max(uSpotInnerCutOff - uSpotOuterCutOff, 0.0001);
        float spotAtten = clamp((theta - uSpotOuterCutOff) / epsilon, 0.0, 1.0);

        attenuation = rangeAtten * spotAtten;
    }

    vec3 H = normalize(V + L);
    vec3 radiance = uLightColor * uLightIntensity * attenuation;

    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0001);

    vec3  numerator   = NDF * G * F;
    float denominator = 4.0 * NdotV * NdotL + 0.0001;
    vec3  specular    = numerator / denominator;

    vec3 diffuse = kD * albedoLinear / PI;

    vec3 Lo = (diffuse + specular) * radiance * NdotL;


    vec3 ambient = uAmbientStrength * albedoLinear * occlusion;

    vec3 color = ambient + Lo + emissive;

    //color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, albedo.a);
}
