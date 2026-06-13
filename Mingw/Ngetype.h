#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#ifndef NGETYPE_H
#define NGETYPE_H
struct Vertex3D{
    glm::vec3 pos;
    glm::vec2 texCoords;
    unsigned color;
    unsigned normal;
    unsigned joints;
    unsigned weights;
    unsigned vertexIndex;
    unsigned drawID;
};
struct DrawData{
    unsigned modelID;
    unsigned mtc;
    unsigned mwo;
    unsigned mpo;
    unsigned jo;
    unsigned vertexCount;
    unsigned nodeMatrixIndex;
    int      materialIndex;
    glm::mat4 normalMat;
};
struct Material {


    glm::vec4 baseColorFactor = glm::vec4(1.0f);            // 16 bytes
    glm::vec4 emissiveFactor = glm::vec4(1.0f);             // 16 bytes
    
    // Bindless texture handles (stored as low/high 32-bit pairs for GLSL uvec2 compatibility)
    uint64_t albedoHandle   = 0;
    uint64_t metallicHandle   = 0;
    uint64_t normalHandle   = 0;
    uint64_t emissiveHandle   = 0;

    float metallicFactor = 1.0f;                            
    float roughnessFactor = 1.0f;                           
    float normalScale = 1.0f;                               
    float occlusionStrength = 1.0f;                         
    float alphaCutoff = 0.5f;                               
    
    int doubleSided = 0;                                    
    int alphaMode = 0;
    int hasTexture;
};
#endif