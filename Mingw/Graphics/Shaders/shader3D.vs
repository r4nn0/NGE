#version 460 core
layout(location = 0) in vec4 tangent;
layout(location = 1) in vec3  pos;
layout(location = 2) in vec3  normal;     // XY components, reconstruct Z
layout(location = 3) in vec2  texCoords;
layout(location = 4) in vec4  color;      // auto unpacked to 0-1 float by GL_TRUE
layout(location = 5) in uvec4 aJoints;    // auto unpacked from bytes
layout(location = 6) in vec4  aWeights;   // auto unpacked to 0-1 float by GL_TRUE
layout(location = 7) in uint  vertexIndex;
layout(location = 8) in uint  drawID;

uniform mat4 proj_matrix;
uniform mat4 vw_matrix;

struct DrawData{
    int modelID;
    uint mtc;
    int mwo;
    uint mpo;
    int jo;
    uint vertexCount;
    uint nodeMatrixIndex;
    int  materialIndex;
    mat4 normalMat;
};

layout(binding = 0, std430) readonly buffer modelMatrices {
    mat4 ml_matrix[];
};
layout(binding = 1, std430) readonly buffer morphPositions{
    vec3 uMorphPositions[];
};
layout(binding = 2, std430) readonly buffer jointMatrices{
    mat4 uJointMatrices[];
};
layout(binding = 3, std430) readonly buffer morphWeights{
    float uMorphWeights[];
};
layout(binding = 4, std430) readonly buffer nodeMatrices{
    mat4 uNodeMatrices[];
};
layout(binding = 6, std430) readonly buffer drawData{
    DrawData uDrawData[];
};

out DATA{
    vec3 FragPos;
    vec3 vNormals;
    vec4 vTangent;
    vec4 color;
    vec2 texCoords;
    flat int materialIndex;
} vs_out;

void main() {
    
    vec3 morphedPosition = pos;
    vec4 finalPosition = vec4(1.0);
    mat4 skinMatrix = mat4(1.0);
    for (uint i = 0; i < uDrawData[drawID].mtc; i++) {
        uint morphPosIndex = uDrawData[drawID].mpo + (i*uDrawData[drawID].vertexCount) + vertexIndex;
        float weight = uMorphWeights[uDrawData[drawID].mwo + i];
        vec3 delta  = uMorphPositions[morphPosIndex];
        morphedPosition += delta * weight;
    }
    vec3 N = normal;
    if (aWeights.x + aWeights.y + aWeights.z + aWeights.w > 0.0) {
        skinMatrix =
            aWeights.x * uJointMatrices[uDrawData[drawID].jo + aJoints.x] +
            aWeights.y * uJointMatrices[uDrawData[drawID].jo + aJoints.y] +
            aWeights.z * uJointMatrices[uDrawData[drawID].jo + aJoints.z] +
            aWeights.w * uJointMatrices[uDrawData[drawID].jo + aJoints.w];
        N = mat3(skinMatrix) * N;
    }
    
    finalPosition = skinMatrix * vec4(morphedPosition, 1.0);
    
    mat4 nodeMatrix = uNodeMatrices[uDrawData[drawID].nodeMatrixIndex];
    
    mat4 worldMatrix = ml_matrix[uDrawData[drawID].modelID] * nodeMatrix;
    
    gl_Position = proj_matrix * vw_matrix * worldMatrix * finalPosition;
    vs_out.FragPos = vec3(worldMatrix * finalPosition);
    
    mat3 normalMatrix = mat3(uDrawData[drawID].normalMat);
    vs_out.vNormals = normalize(normalMatrix * N);
    vs_out.vTangent = vec4(normalize(normalMatrix * tangent.xyz), tangent.w);
    vs_out.color = color;
    vs_out.texCoords = texCoords;
    vs_out.materialIndex = uDrawData[drawID].materialIndex;
}
