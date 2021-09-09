#version 330 core
layout (location=0) in vec4 pos;
layout (location=1) in vec4 color;
layout (location=2) in vec2 texCoords;

uniform mat4 proj_matrix;
uniform mat4 vw_matrix=mat4(1.0);
uniform mat4 ml_matrix=mat4(1.0);

//uniform int texID;

out DATA{
    vec4 pos;
    vec4 color;
    vec2 texCoords;
//    int texID;

}vs_out;

void main() {
    gl_Position =proj_matrix*vw_matrix*ml_matrix*pos;
    vs_out.pos = pos;
    vs_out.color=color;
    vs_out.texCoords=texCoords;
    //vs_out.texID=texID;
}
