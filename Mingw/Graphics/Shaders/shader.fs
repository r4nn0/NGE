#version 330 core
out vec4 FragColor;

in DATA{
    vec4 pos;
    vec4 color;
    vec2 texCoords;
    float texID;
}fs_in;

uniform sampler2D textures[32];

vec4 base_color;
void main() {
    int i=int(fs_in.texID);
    base_color=texture2D(textures[i],fs_in.texCoords);
    FragColor = fs_in.color * base_color;
}



