#version 430 core
#extension GL_ARB_bindless_texture : require

in vec2 TexCoords;
out vec4 fragColor;

uniform sampler2DArray textureHandle;
uniform unsigned int spriteIndex;
uniform vec4 spriteColor;
uniform vec2 repetition;

void main()
{    
    vec2 newTexCoords = TexCoords * repetition;
    fragColor = spriteColor * texture(textureHandle, vec3(newTexCoords, spriteIndex));
}