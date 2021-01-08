#version 440 compatibility

layout(location=0)in vec4 coord;
layout(location=1)in vec2 vertTexCoord;
out vec2 fragTexCoord;

void main() {
    fragTexCoord=vertTexCoord;
    gl_Position = coord;
}
