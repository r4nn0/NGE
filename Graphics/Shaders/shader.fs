#version 440 compatibility

uniform sampler2D tex;
in vec2 fragTexCoord;
out vec4 FragColor;
vec4 base_col;

void main() {
    vec4 base_col = texture(tex, fragTexCoord);
    FragColor= base_col;
}
