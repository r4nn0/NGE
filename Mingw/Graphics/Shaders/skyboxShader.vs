#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aPos;
    // Strip translation from view matrix so skybox stays centered on camera
    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
    // Set z = w so depth is always 1.0 (skybox renders behind everything)
    gl_Position = pos.xyww;
}