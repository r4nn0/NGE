#version 330 core

out vec4 FragColor;

in DATA{
    vec4 pos;
    vec4 color;
    vec2 texCoords;
    float textureSlot;
}fs_in;

uniform sampler2D texture[32];
vec4 base_color;


void main() {
    if(fs_in.textureSlot>=0){
        base_color=texture2D(texture[int(fs_in.textureSlot)], fs_in.texCoords);
        FragColor = fs_in.color * base_color;
	}
	else
		FragColor = fs_in.color;
}




