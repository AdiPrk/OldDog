#version 430 core
#extension GL_ARB_bindless_texture : require

layout (std140) uniform Time
{
	float iTime;	
};

layout (std140) uniform Resolution
{
	vec2 iResolution;
};

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
    //fragColor.rgb = vec3(cos(iTime));
}






