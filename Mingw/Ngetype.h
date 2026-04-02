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

    glm::vec2 regionSize              = glm::vec2(0.0f);
    glm::vec2 albedoRegionOrigin      = glm::vec2(0.0f);
    glm::vec2 normalRegionOrigin      = glm::vec2(0.0f);
    glm::vec2 metallicRegionOrigin   = glm::vec2(0.0f);
    glm::vec2 emissiveRegionOrigin    = glm::vec2(0.0f);

    float metallicFactor = 1.0f;                            //  4 bytes
    float roughnessFactor = 1.0f;                           //  4 bytes
    float normalScale = 1.0f;                               //  4 bytes
    float occlusionStrength = 1.0f;                         //  4 bytes
    float alphaCutoff = 0.5f;                               //  4 bytes

    
    /*glm::vec2 metallicRoughnessOffset = glm::vec2(-1.0f);   //  8 bytes
    glm::vec2 normalOffset = glm::vec2(-1.0f);              //  8 bytes
    glm::vec2 occlusionOffset = glm::vec2(-1.0f);           //  8 bytes
    glm::vec2 emissiveOffset = glm::vec2(-1.0f);            //  8 bytes*/
    int albedoImageIndex            = -1;
    int metallicRoughnessImageIndex = -1;
    int normalImageIndex            = -1;
    int emissiveImageIndex          = -1;
    int doubleSided = 0;                                    //  4 bytes
    int alphaMode = 0;                                      //  4 bytes (0=OPAQUE, 1=MASK, 2=BLEND)
    int pad1;
    int pad2;
    int pad3;
};
#endif