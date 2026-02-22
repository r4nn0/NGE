#version 450 core

out vec4 FragColor;

in DATA{
    vec4 color;
    vec2 texCoords;
    flat int textureSlot;
}fs_in;

uniform sampler2D texture[32];
vec4 base_color;


void main() {
    base_color=texture2D(texture[0], fs_in.texCoords);
    FragColor = vec4(fs_in.color.rgb, base_color.r);
}