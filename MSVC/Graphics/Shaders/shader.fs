#version 330 core
out vec4 FragColor;



in DATA{
    vec4 pos;
    vec4 color;
    vec2 texCoords;
    //int texCount;
}fs_in;
//uniform sampler2D tex[texCount];
uniform sampler2D tex;

vec4 base_color;
void main() {

    base_color=texture2D(tex,fs_in.texCoords);
    FragColor = fs_in.color * base_color;
}




