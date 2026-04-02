#version 460 core

layout(location = 0) in vec3  aPos;
layout(location = 1) in vec2  aUV;
layout(location = 2) in vec3  aNormal;
layout(location = 3) in vec4  aColor;
layout(location = 4) in uvec4 aJoints;
layout(location = 5) in vec4  aWeights;

// Matrices
uniform mat4 uMVP;
uniform mat4 uModel;

// Skinning
uniform bool  uSkinned;
uniform mat4  uJointMatrices[128];

out vec2  vUV;
out vec3  vNormal;
out vec3  vWorldPos;
out vec4  vColor;

void main() {
    vec4 skinnedPos    = vec4(aPos, 1.0);
    vec3 skinnedNormal = aNormal;

    if (uSkinned) {
        mat4 skinMatrix =
            aWeights.x * uJointMatrices[aJoints.x] +
            aWeights.y * uJointMatrices[aJoints.y] +
            aWeights.z * uJointMatrices[aJoints.z] +
            aWeights.w * uJointMatrices[aJoints.w];

        skinnedPos    = skinMatrix * vec4(aPos, 1.0);
        skinnedNormal = mat3(transpose(inverse(skinMatrix))) * aNormal;
    }

    gl_Position = uMVP   * skinnedPos;
    vWorldPos   = vec3(uModel * skinnedPos);
    vNormal     = normalize(mat3(transpose(inverse(uModel))) * skinnedNormal);
    vUV         = aUV;
    vColor      = aColor;
}
