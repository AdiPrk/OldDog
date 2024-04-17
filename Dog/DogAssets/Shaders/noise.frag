#version 430 core

in vec2 TexCoords;
out vec4 fragColor;

uniform sampler2DArray textureHandle;
uniform unsigned int spriteIndex;
uniform vec4 spriteColor;
uniform vec2 repetition;

layout (std140) uniform Time
{
    float iTime;
};

layout (std140) uniform Resolution
{
	vec2 iResolution;
};

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = TexCoords;

    // Output to screen
    vec3 col = vec3(uv.y);
    
    fragColor = vec4(col,1.0);
}
