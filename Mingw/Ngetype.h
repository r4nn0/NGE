#include <glm/glm.hpp>
#ifndef NGETYPE_H
#define NGETYPE_H
struct Vertex3D{
    glm::vec3 pos;
    glm::vec4 color;
    glm::vec2 texCoords;
    glm::vec3 normal;
    int textureSlot;
    unsigned modelID;
    glm::uvec4 joints;
    glm::vec4 weights;
    unsigned mtc;
    unsigned mwo;
    unsigned mpo;
    unsigned jo;

    unsigned vertexIndex;
    unsigned vertexCount;
    unsigned nodeMatrixIndex;
    int materialIndex;
};
struct Material {

    glm::vec4 baseColorFactor = glm::vec4(1.0f);            // 16 bytes
    glm::vec4 emissiveFactor = glm::vec4(1.0f);             // 16 bytes
    glm::vec2 metallicRoughnessOffset = glm::vec2(-1.0f);   //  8 bytes
    glm::vec2 normalOffset = glm::vec2(-1.0f);              //  8 bytes
    glm::vec2 occlusionOffset = glm::vec2(-1.0f);           //  8 bytes
    glm::vec2 emissiveOffset = glm::vec2(-1.0f);            //  8 bytes
    float metallicFactor = 1.0f;                            //  4 bytes
    float roughnessFactor = 1.0f;                           //  4 bytes
    float normalScale = 1.0f;                               //  4 bytes
    float occlusionStrength = 1.0f;                         //  4 bytes
    float alphaCutoff = 0.5f;                               //  4 bytes
    int doubleSided = 0;                                    //  4 bytes
    int alphaMode = 0;                                      //  4 bytes (0=OPAQUE, 1=MASK, 2=BLEND)
    int pad0;                                               //  4 bytes
};
#endif