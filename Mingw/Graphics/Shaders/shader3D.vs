#version 460 core
layout (location=0) in vec3 pos;
layout (location=1) in vec4 color;
layout (location=2) in vec2 texCoords;
layout (location=3) in vec3 vNormals;
layout (location=4) in float textureSlot;
layout (location=5) in float modelID;

#extension GL_ARB_bindless_texture : require
uniform mat4 proj_matrix;
uniform mat4 vw_matrix;
//uniform mat4 ml_matrix=mat4(1.0);

layout(binding = 0, std430) readonly buffer ssbo1 {
    mat4 ml_matrix[];
};

out DATA{
    vec3 FragPos;
    vec3 vNormals;
    vec4 color;
    vec2 texCoords;
    float textureSlot;
}vs_out;

void main() {
    gl_Position =proj_matrix*vw_matrix*ml_matrix[uint(modelID)]*vec4(pos,1.0f);
    
    vs_out.FragPos = vec3(ml_matrix[uint(modelID)]*vec4(pos, 1.0f));
    vs_out.vNormals = mat3(transpose(inverse(ml_matrix[uint(modelID)])))*vNormals;
    vs_out.color=color;
    vs_out.texCoords=texCoords;
    vs_out.textureSlot=textureSlot;
}
