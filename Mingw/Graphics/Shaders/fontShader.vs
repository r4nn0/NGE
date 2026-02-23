#version 450 core
layout (location=0) in vec3 pos;
layout (location=1) in vec2 texCoords;
layout (location=2) in vec4 color;
layout (location=3) in int textureSlot;

uniform mat4 proj_matrix;
uniform mat4 vw_matrix;



out DATA{
    vec4 color;
    vec2 texCoords;
    int textureSlot;
}vs_out;

void main() {
    gl_Position =proj_matrix*vw_matrix*vec4(pos.x,pos.y,pos.z,1);
    vs_out.color=color;
    vs_out.texCoords=texCoords;
    vs_out.textureSlot=textureSlot;
}
