#shadertype vertex

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

#shadertype fragment

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

float near = 0.1; 
float far  = 100.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{    
    vec2 newTexCoords = TexCoords * repetition;
    fragColor = spriteColor * texture(textureHandle, vec3(newTexCoords, spriteIndex));
    
    if (fragColor.a < 0.1) {
        discard;
	}
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //fragColor = vec4(vec3(depth), 1.0);
    
    //fragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    //float depth = gl_FragCoord.z;
    //if (depth >= 0.9) {
    //depth *= depth;
    //fragColor.rgb = vec3(depth) / 5.;
    //}
    //else {
    //fragColor.r = 1.0;
    //}
    
}