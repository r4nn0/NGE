#version 450 core
layout (location=0) in vec3 lpos;
layout (location=1) in vec3 wpos;
layout (location=2) in vec4 col;
layout (location=3) in float scale;
layout (location=4) in float rot;
uniform mat4 proj_matrix;
uniform mat4 vw_matrix;

out vec4 color;

void main() {
    vec3 finalPos = (vec3(lpos.x, -lpos.y, lpos.z)) * scale;
    float c = cos(rot);
    float s = sin(rot);
    finalPos = vec3(finalPos.x*c - finalPos.y*s, finalPos.x*s + finalPos.y*c, finalPos.z) + wpos;
    
    gl_Position =proj_matrix*vw_matrix*vec4(finalPos,1);
    color = col;
}
