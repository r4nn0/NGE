#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 7) in uint drawID;

uniform mat4 uLightSpaceMatrix;

struct DrawData {
    uint modelID;
    uint mtc;
    uint mwo;
    uint mpo;
    uint jo;
    uint vertexCount;
    uint nodeMatrixIndex;
    int  materialIndex;
};

layout(binding = 0, std430) readonly buffer modelMatrices { mat4 ml_matrix[]; };
layout(binding = 4, std430) readonly buffer nodeMatrices  { mat4 uNodeMatrices[]; };
layout(binding = 6, std430) readonly buffer drawData      { DrawData uDrawData[]; };

void main() {
    mat4 nodeMatrix = uNodeMatrices[uDrawData[drawID].nodeMatrixIndex];

    gl_Position = uLightSpaceMatrix 
                * ml_matrix[uDrawData[drawID].modelID] 
                * nodeMatrix 
                * vec4(pos, 1.0);
}