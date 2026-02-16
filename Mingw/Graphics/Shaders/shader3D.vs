#version 460 core
#extension GL_ARB_bindless_texture : require
layout (location=0) in vec3 pos;
layout (location=1) in vec4 color;
layout (location=2) in vec2 texCoords;
layout (location=3) in vec3 vNormals;
layout (location=4) in int textureSlot;
layout (location=5) in uint modelID;


layout(location = 6) in uvec4 aJoints;
layout(location = 7) in vec4 aWeights;


layout(location = 8) in uint morphTargetsCount;
layout(location = 9) in uint morphWeightOffset;
layout(location = 10) in uint morphPositionOffset;
layout(location = 11) in uint jointOffset;
layout(location = 12) in uint vertextIndex;
layout(location = 13) in uint vertexCount;
layout(location = 14) in uint nodeMatrixIndex;

uniform mat4 proj_matrix;
uniform mat4 vw_matrix;
//uniform mat4 ml_matrix=mat4(1.0);

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


out DATA{
    vec3 FragPos;
    vec3 vNormals;
    vec4 color;
    vec2 texCoords;
    int textureSlot;
}vs_out;

void main() {
    
    vec3 morphedPosition = pos;
    
    for (uint i = 0; i < morphTargetsCount; i++) {
        uint morphPosIndex = morphPositionOffset + (i*vertexCount) + vertextIndex;
        float weight = uMorphWeights[morphWeightOffset + i];
        vec3 delta  = uMorphPositions[morphPosIndex];
        morphedPosition += delta * weight;
    }
    
    vec4 finalPosition;
    
    if (aWeights.x + aWeights.y + aWeights.z + aWeights.w > 0.0) {
        mat4 skinMatrix =
            aWeights.x * uJointMatrices[jointOffset + aJoints.x] +
            aWeights.y * uJointMatrices[jointOffset + aJoints.y] +
            aWeights.z * uJointMatrices[jointOffset + aJoints.z] +
            aWeights.w * uJointMatrices[jointOffset + aJoints.w];
        
        finalPosition = skinMatrix * vec4(morphedPosition, 1.0);
    } else {
        finalPosition = vec4(morphedPosition, 1.0);
    }
    mat4 nodeMatrix = uNodeMatrices[nodeMatrixIndex];
    float totalLength = length(nodeMatrix[0]) + length(nodeMatrix[1]) + 
                        length(nodeMatrix[2]) + length(nodeMatrix[3]);

    if (totalLength < 0.001) {
        nodeMatrix = mat4(1.0);
    }

    gl_Position = proj_matrix * vw_matrix * ml_matrix[modelID] * nodeMatrix * finalPosition;
    //gl_Position = proj_matrix * vw_matrix * ml_matrix[modelID] * vec4(pos, 1.0f);
    vs_out.FragPos = vec3(ml_matrix[modelID] * finalPosition);
    vs_out.vNormals = mat3(transpose(inverse(ml_matrix[modelID]))) * vNormals;
    vs_out.color = color;
    vs_out.texCoords = texCoords;
    vs_out.textureSlot = textureSlot;
}
