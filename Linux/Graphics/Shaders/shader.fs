#version 330 core

out vec4 FragColor;

in DATA{
    vec4 color;
    vec2 texCoords;
    flat int textureSlot;
}fs_in;

uniform sampler2D texture[32];
vec4 base_color;
// const int tSlot[1] = int[1](int(fs_in.textureSlot));

void main() {
	base_color=vec4(1);
	//const int tslot = array[int[1](int(fs_in.textureSlot))[0]];
	if(fs_in.textureSlot==0) base_color=texture2D(texture[0],fs_in.texCoords);
	if(fs_in.textureSlot==1) base_color=texture2D(texture[1],fs_in.texCoords);
	if(fs_in.textureSlot==2) base_color=texture2D(texture[2],fs_in.texCoords);
	if(fs_in.textureSlot==3) base_color=texture2D(texture[3],fs_in.texCoords);
	if(fs_in.textureSlot==4) base_color=texture2D(texture[4],fs_in.texCoords);
	if(fs_in.textureSlot==5) base_color=texture2D(texture[5],fs_in.texCoords);
	if(fs_in.textureSlot==6) base_color=texture2D(texture[6],fs_in.texCoords);
	if(fs_in.textureSlot==7) base_color=texture2D(texture[7],fs_in.texCoords);
	if(fs_in.textureSlot==8) base_color=texture2D(texture[8],fs_in.texCoords);
	if(fs_in.textureSlot==9) base_color=texture2D(texture[9],fs_in.texCoords);
	if(fs_in.textureSlot==10) base_color=texture2D(texture[10],fs_in.texCoords);
	if(fs_in.textureSlot==11) base_color=texture2D(texture[11],fs_in.texCoords);
	if(fs_in.textureSlot==12) base_color=texture2D(texture[12],fs_in.texCoords);
	if(fs_in.textureSlot==13) base_color=texture2D(texture[13],fs_in.texCoords);
	if(fs_in.textureSlot==14) base_color=texture2D(texture[14],fs_in.texCoords);
	if(fs_in.textureSlot==15) base_color=texture2D(texture[15],fs_in.texCoords);
	if(fs_in.textureSlot==16) base_color=texture2D(texture[16],fs_in.texCoords);
	if(fs_in.textureSlot==17) base_color=texture2D(texture[17],fs_in.texCoords);
	if(fs_in.textureSlot==18) base_color=texture2D(texture[18],fs_in.texCoords);
	if(fs_in.textureSlot==19) base_color=texture2D(texture[19],fs_in.texCoords);
	if(fs_in.textureSlot==20) base_color=texture2D(texture[20],fs_in.texCoords);
	if(fs_in.textureSlot==21) base_color=texture2D(texture[21],fs_in.texCoords);
	if(fs_in.textureSlot==22) base_color=texture2D(texture[22],fs_in.texCoords);
	if(fs_in.textureSlot==23) base_color=texture2D(texture[23],fs_in.texCoords);
	if(fs_in.textureSlot==24) base_color=texture2D(texture[24],fs_in.texCoords);
	if(fs_in.textureSlot==25) base_color=texture2D(texture[25],fs_in.texCoords);
	if(fs_in.textureSlot==26) base_color=texture2D(texture[26],fs_in.texCoords);
	if(fs_in.textureSlot==27) base_color=texture2D(texture[27],fs_in.texCoords);
	if(fs_in.textureSlot==28) base_color=texture2D(texture[28],fs_in.texCoords);
	if(fs_in.textureSlot==29) base_color=texture2D(texture[29],fs_in.texCoords);
	if(fs_in.textureSlot==30) base_color=texture2D(texture[30],fs_in.texCoords);
	if(fs_in.textureSlot==31) base_color=texture2D(texture[31],fs_in.texCoords);
	
	FragColor = base_color*fs_in.color;
}




