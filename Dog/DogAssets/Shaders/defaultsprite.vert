#version 430 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 projectionView;
};

uniform mat4 model;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projectionView * model * vec4(vertex.xy, 0.0, 1.0);
}