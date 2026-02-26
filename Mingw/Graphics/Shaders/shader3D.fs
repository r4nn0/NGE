#version 460 core

out vec4 FragColor;


struct Material {
    vec4    baseColorFactor;
    vec4    emissiveFactor;
    vec2    metallicRoughnessOffset;
    vec2    normalOffset;
    vec2    occlusionOffset;
    vec2    emissiveOffset; 
    float   metallicFactor;
    float   roughnessFactor;
    float   normalScale;
    float   occlusionStrength;
    float   alphaCutoff;
    int     doubleSided;
    int     alphaMode;
    int     pad0;
};

layout(binding = 5, std430) readonly buffer materials{
    Material uMaterials[];
};


in DATA{
    vec3 FragPos;
    vec3 vNormals;
    vec4 color;
    vec2 texCoords;
    flat int textureSlot;
    flat int materialIndex;
}fs_in;

uniform sampler2D texture[32];
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform float lightCutoff = 30.0;
uniform float lightOuterCutoff = 45.0;
const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness){
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness){
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return GeometrySchlickGGX(NdotV, roughness) * 
           GeometrySchlickGGX(NdotL, roughness);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {
    vec4 baseColor;
    if(fs_in.textureSlot >= 0)
        baseColor = texture2D(texture[fs_in.textureSlot], fs_in.texCoords) * fs_in.color;
    else
        baseColor = fs_in.color;


    vec4 baseColorFactor = vec4(1.0);
    float metallic  = 0.0;
    float roughness = 0.5;
    float occlusionStrength = 1.0;
    float normalScale = 1.0;
    vec3  emissiveFactor    = vec3(0.0);
    vec2  occlusionOffset           = vec2(-1.0); 
    vec2  emissiveOffset            = vec2(-1.0); 
    vec2 metallicRoughnessOffset    = vec2(-1.0);
    vec2 normalOffset               = vec2(-1.0);
    vec3 N = normalize(fs_in.vNormals);
    vec3 ambient = vec3(0.03) * baseColor.rgb;
    Material mat;
    if(fs_in.materialIndex>=0){
        mat = uMaterials[fs_in.materialIndex];
        baseColorFactor = mat.baseColorFactor;

        if(mat.alphaMode == 1 && baseColor.a < mat.alphaCutoff)
            discard;

        metallic  = mat.metallicFactor;
        roughness = mat.roughnessFactor;
        if(mat.metallicRoughnessOffset.x >= 0.0){
            vec2 mrUV = fs_in.texCoords + mat.metallicRoughnessOffset;
            vec4 mrSample = texture2D(texture[fs_in.textureSlot], mrUV);
            roughness *= mrSample.g;
            metallic  *= mrSample.b;
        }
        roughness = clamp(roughness, 0.04, 1.0);
        metallic  = clamp(metallic,  0.0,  1.0);

        if(mat.normalOffset.x >= 0.0){
            vec2 normalUV = fs_in.texCoords + mat.normalOffset;
            vec3 normalSample = texture2D(texture[fs_in.textureSlot], normalUV).rgb;
            normalSample = normalSample * 2.0 - 1.0;
            normalSample.xy *= mat.normalScale;
            N = normalize(normalSample);
        }

        baseColor*=baseColorFactor;
        ambient = vec3(0.03) * baseColor.rgb;
        if(mat.occlusionOffset.x >= 0.0){
            vec2 aoUV = fs_in.texCoords + mat.occlusionOffset;
            float ao = texture2D(texture[fs_in.textureSlot], aoUV).r;
            ambient *= ao * mat.occlusionStrength;
        }

        emissiveFactor = mat.emissiveFactor.rgb;
        if(mat.emissiveOffset.x >= 0.0){
            vec2 emissiveUV = fs_in.texCoords + mat.emissiveOffset;
            emissiveFactor *= texture2D(texture[fs_in.textureSlot], emissiveUV).rgb;
        }
    }
    vec3 V  = normalize(viewPos - fs_in.FragPos);
    //vec3 L  = normalize(lightPos - fs_in.FragPos);
    vec3 L  = normalize(-lightDir);
    vec3 H  = normalize(V + L);

    vec3 F0 = mix(vec3(0.04), baseColor.rgb, metallic);

    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3  F   = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;

    vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);

    float theta         = dot(L, normalize(-lightDir));
    float inner         = cos(radians(lightCutoff));
    float outer         = cos(radians(lightOuterCutoff));
    float epsilon       = inner - outer;
    float coneIntensity = clamp((theta - outer) / epsilon, 0.0, 1.0);
    float dist = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);
    //vec3 Lo = (kD * baseColor.rgb / PI + specular) * lightColor * NdotL * coneIntensity * attenuation;
    vec3 Lo = (kD * baseColor.rgb / PI + specular) * lightColor * NdotL;
    vec3 result = ambient + Lo + emissiveFactor;

    result = result / (result + vec3(1.0)); 
    result = pow(result, vec3(1.0/2.2));    
    //FragColor = vec4(vec3(NdotL), 1.0);
    FragColor = vec4(result, baseColor.a);
        
}