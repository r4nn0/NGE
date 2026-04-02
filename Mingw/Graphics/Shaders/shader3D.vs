#version 460 core

layout(location = 0) in vec3  pos;
layout(location = 1) in vec2  texCoords;
layout(location = 2) in vec4  color;      // auto unpacked to 0-1 float by GL_TRUE
layout(location = 3) in vec2  normal;     // XY components, reconstruct Z
layout(location = 4) in uvec4 aJoints;    // auto unpacked from bytes
layout(location = 5) in vec4  aWeights;   // auto unpacked to 0-1 float by GL_TRUE
layout(location = 6) in uint  vertexIndex;
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
    vec4 color;
    vec2 texCoords;
    flat int materialIndex;
}vs_out;

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
    
    if (aWeights.x + aWeights.y + aWeights.z + aWeights.w > 0.0) {
        mat4 skinMatrix =
            aWeights.x * uJointMatrices[uDrawData[drawID].jo + aJoints.x] +
            aWeights.y * uJointMatrices[uDrawData[drawID].jo + aJoints.y] +
            aWeights.z * uJointMatrices[uDrawData[drawID].jo + aJoints.z] +
            aWeights.w * uJointMatrices[uDrawData[drawID].jo + aJoints.w];
    }
    
    finalPosition = skinMatrix * vec4(morphedPosition, 1.0);
    
    mat4 nodeMatrix = uNodeMatrices[uDrawData[drawID].nodeMatrixIndex];
    

    gl_Position = proj_matrix * vw_matrix * ml_matrix[uDrawData[drawID].modelID] * nodeMatrix * finalPosition;
    vs_out.FragPos = vec3(ml_matrix[uDrawData[drawID].modelID] * nodeMatrix * finalPosition);
    vec2 nXY = normal * 2.0 - 1.0;
    float nZ = sqrt(max(0.0, 1.0 - dot(nXY, nXY)));
    vec3 decodedNormal = normalize(vec3(nXY, nZ));
    vs_out.vNormals = mat3(uDrawData[drawID].normalMat) * decodedNormal;
    vs_out.color = color;
    vs_out.texCoords = texCoords;
    vs_out.materialIndex = uDrawData[drawID].materialIndex;
}
