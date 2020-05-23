#version 330 core
in vec2 aPos;

out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos,0.0, 1.0);
    vertexColor = vec4(0.0, 1.0, 0.0, 1.0);
}
