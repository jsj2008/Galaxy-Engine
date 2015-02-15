#version 440 core

// Uniform
#define CONTEXT 0
#define FRUSTRUM 1

// Shader Storage
#define COMMAND 0
#define CLIP 1
#define WORLD 2
#define AABB 3
#define MATERIAL 4

in vec2 texCoord;

layout(binding = 0) uniform sampler2D diffuseSampler;

out vec3 color;

void main(void)
{
    color = texture(diffuseSampler, texCoord).xyz;
}
