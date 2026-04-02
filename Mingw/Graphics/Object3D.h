#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include "../Ngetype.h"
#include "tiny_gltf.h"
#include "Sprite.h"
#include "VTexRegistery.h"
class Object3D
{
public:
    Object3D();
    Object3D(const char*);
    bool wasRendered;
    tinygltf::Model model;
    const tinygltf::Scene* scene = nullptr;
    struct Primitive
    {
        std::vector<Vertex3D> vertices;
        std::vector<unsigned int> indices;
        int materialIndex = -1;

        int nodeIndex = -1;
        int skinIndex = -1;
        
        std::vector<std::vector<glm::vec3>> morphPositions;
        std::vector<std::vector<glm::vec3>> morphNormals;
    };

    std::vector<Primitive> primitives;

    std::vector<Material> materials;

    std::vector<glm::vec3> nodeTranslation;
    std::vector<glm::quat> nodeRotation;
    std::vector<glm::vec3> nodeScale;

    // final matrices
    std::vector<glm::mat4> nodeLocalMatrix;
    std::vector<glm::mat4> nodeGlobalMatrix;

    struct Skin{
        int skinIndex;

        std::vector<glm::mat4> inverseBindMatrices;

        std::vector<glm::mat4> finalJointMatrices;
    };

    std::vector<Skin> skins;

    std::vector<std::vector<float>> morphWeights;

    int currentAnimation = 0;

    float animationTime = 0.0f;

    bool playing = true;

    bool hasTexture;
    struct TextureRef {
        
        int vtId = -1;
    };
    std::vector<TextureRef> textureRefs;
    /*
    std::map<int ,glm::vec2> texScaleFactor;
    std::map<int, glm::vec2> texOffset;
    
    */

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    bool LoadModel(const char*);
    void InitializeNodes();
    void LoadMeshes();
    void LoadNode(int);
    void LoadPrimitive(const tinygltf::Primitive&, int, int);
    void LoadVertices(const tinygltf::Primitive&, Primitive&);
    void LoadIndices(const tinygltf::Primitive&, Primitive&);
    void LoadMorphTargets(const tinygltf::Primitive&, Primitive&);
    void LoadSkins();
    void InitializeMorphWeights();
    void LoadTextures();
    void CalculateAnimationDuration();
    void UpdateAnimation(float);
    void UpdateNodeMatrices();
    void UpdateNodeRecursive(int, const glm::mat4&);
    void UpdateSkinMatrices();
    std::vector<float> animationDurations;
    std::vector<glm::vec4> readVecFloat(const tinygltf::Primitive& prim, const std::string& attr, int expectedComponents);
    void samplePlane2D();
};
extern std::map<std::string, Object3D*> ObjectsTotal;
#endif // OBJECT3D_H