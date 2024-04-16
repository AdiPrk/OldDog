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
    vec2 newTexCoords = TexCoords * repetition;
    fragColor = spriteColor * texture(textureHandle, vec3(newTexCoords, spriteIndex));

    vec2 col;
    float t = iTime;
    vec2 uv = (TexCoords - 0.5) * 2.0;
    float factor = 1.5;
    vec2 v1;
    for(int i=0;i<12;i++)
    {
        uv *= -factor*factor;
        //uv.x *= 2.0;
        v1 = uv.yx/factor;
        uv += sin(v1+col+t*10.0)/factor;
        col += vec2(sin(uv.x-uv.y+v1.x-col.y),sin(uv.y-uv.x+v1.y-col.x));
    }
    fragColor *= vec4(vec3(col.x+1.0,col.x-col.y/2.0,col.x*2.0)/2.0,1.0);
}