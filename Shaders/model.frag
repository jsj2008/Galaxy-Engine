#version 440 core
#extension GL_ARB_bindless_texture : enable

// Uniform
#define CONTEXT 0
#define FRUSTRUM 1

// Shader Storage
#define COMMAND 0
#define CLIP 1
#define WORLD 2
#define AABB 3
#define MATERIAL 4

in vec3 position;
in vec3 normal;
in vec2 texCoord;
flat in int materialIndex;
flat in int ID;

layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outPosition;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec2 outShininessAlbedo;

struct Material
{
    vec4 colorDiffuse;
    vec4 shininessAlbedo;
    sampler2D sampler;
    uvec2 useTexture;
};

layout(binding = MATERIAL, shared) readonly buffer MaterialBuffer
{
    Material material[];
};

void main(void)
{
    if(material[materialIndex].useTexture.x == 1)
        outDiffuse = textureLod(material[materialIndex].sampler, texCoord, textureQueryLod(material[materialIndex].sampler, texCoord).x).xyz;

    else
        outDiffuse = material[materialIndex].colorDiffuse.xyz;

    outPosition = position;
    outNormal = normalize(normal);
    outShininessAlbedo = material[materialIndex].shininessAlbedo.xy;
}
