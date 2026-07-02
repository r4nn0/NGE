#version 450 core
layout (location=0) in vec3 wpos;
layout (location=1) in vec3 lpos;
layout (location=2) in float rot;
layout (location=3) in vec2 scale;
layout (location=4) in vec4 color;

uniform mat4 proj_matrix;
uniform mat4 vw_matrix;

out vec4 col;




void main() {
    vec3 finalPos = lpos * vec3(scale.x, scale.y, 1);
    float c = cos(rot);
    float s = sin(rot);
    finalPos = vec3(finalPos.x*c - finalPos.y*s, finalPos.x*s + finalPos.y*c, finalPos.z) + wpos;
   
    
    gl_Position =proj_matrix*vw_matrix*vec4(finalPos,1);
    col=color;
}
