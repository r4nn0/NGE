#version 450 core
layout (location=0) in vec3 pos;
layout (location=1) in vec2 texCoords;
layout (location=2) in int id;

uniform mat4 proj_matrix;
uniform mat4 vw_matrix;

struct ssboData{
    vec3 pos;
    float rot;
    vec4 color;
    vec2 scale;
    int textureSlot;
    int pad;
};
layout(binding = 0, std430) readonly buffer SSBOData {
    ssboData uSSBOData[];
};
out DATA{
    vec4 color;
    vec2 texCoords;
    int textureSlot;
}vs_out;



void main() {
    vec3 finalPos = pos * vec3(uSSBOData[id].scale, 1.0);
    float c = cos(uSSBOData[id].rot);
    float s = sin(uSSBOData[id].rot);
    finalPos = vec3(finalPos.x*c - finalPos.y*s, finalPos.x*s + finalPos.y*c, finalPos.z) + uSSBOData[id].pos;
   
    
    gl_Position =proj_matrix*vw_matrix*vec4(finalPos,1);
    vs_out.color=uSSBOData[id].color;
    vs_out.texCoords=texCoords;
    vs_out.textureSlot=uSSBOData[id].textureSlot;
}
