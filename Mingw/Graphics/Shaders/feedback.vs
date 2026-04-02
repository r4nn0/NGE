#version 460 core

layout(location = 0) in vec3  pos;
layout(location = 1) in vec2  texCoords;
layout(location = 7) in uint  drawID;

uniform mat4 proj_matrix;
uniform mat4 vw_matrix;

struct DrawData{
    uint modelID;
    uint mtc;
    uint mwo;
    uint mpo;
    uint jo;
    uint vertexCount;
    uint nodeMatrixIndex;
    int  materialIndex;
    mat4 normalMat;
};

layout(binding = 0, std430) readonly buffer modelMatrices { mat4 ml_matrix[]; };
layout(binding = 4, std430) readonly buffer nodeMatrices  { mat4 uNodeMatrices[]; };
layout(binding = 6, std430) readonly buffer drawData      { DrawData uDrawData[]; };


out vec2 v_texCoords;
out flat int v_materialIndex;

void main() {
    mat4 nodeMatrix = uNodeMatrices[uDrawData[drawID].nodeMatrixIndex];
    
    gl_Position = proj_matrix * vw_matrix 
                * ml_matrix[uDrawData[drawID].modelID] 
                * nodeMatrix 
                * vec4(pos, 1.0);

    v_texCoords    = texCoords;
    v_materialIndex = uDrawData[drawID].materialIndex;
}