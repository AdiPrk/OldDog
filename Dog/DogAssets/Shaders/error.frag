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

    fragColor *= vec4(cos(TexCoords.x * 10 + TexCoords.y * 10), 0.1, 0.1, 1.0);
    mat2 rotationMatrix = mat2(0.0, -1.0, 
                               1.0,  0.0);
    vec2 rotCoord = rotationMatrix * TexCoords;
    fragColor *= vec4(cos(TexCoords.y * 500), cos(rotCoord.x * 10 + rotCoord.y * 10), 0.1, 1.0);
}