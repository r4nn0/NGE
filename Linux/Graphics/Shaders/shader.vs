#version 330 core
layout (location=0) in vec2 pos;
layout (location=1) in vec4 color;
layout (location=2) in vec2 texCoords;
layout (location=3) in int textureSlot;

uniform mat4 proj_matrix;
uniform mat4 vw_matrix=mat4(1.0);
uniform mat4 ml_matrix=mat4(1.0);


out DATA{
    vec4 color;
    vec2 texCoords;
    flat int textureSlot;
}vs_out;

void main() {
    gl_Position =proj_matrix*vw_matrix*ml_matrix*vec4(pos.x,pos.y,1,1);
    vs_out.color=color;
    vs_out.texCoords=texCoords;
    vs_out.textureSlot=textureSlot;
}
