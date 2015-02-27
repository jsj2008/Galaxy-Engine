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
#define PROJECT_LIGHT 5
#define POINT_LIGHT 6
#define WORLD_POINT_LIGHT 7
#define COMMAND_POINT_LIGHT 8

in vec2 texCoord;

layout(binding = 0) uniform sampler2D diffuseSampler;
layout(binding = 1) uniform sampler2D aoSampler;
layout(binding = 2) uniform sampler2D directLightSampler;

out vec3 color;

void main(void)
{
    float ao = texture(aoSampler, texCoord).x;
    vec3 diffuseColor = texture(diffuseSampler, texCoord).xyz;
    vec3 directLightColor = texture(directLightSampler, texCoord).xyz;

    //color = diffuseColor;
    //color = vec3(ao);
    color = ao * diffuseColor * (directLightColor + 0.2);
}
