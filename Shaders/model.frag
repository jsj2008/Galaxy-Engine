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
#define PROJECT_LIGHT 5
#define POINT_LIGHT 6
#define WORLD_POINT_LIGHT 7

in vec3 position;

in vec3 normal;
in vec3 tangent;
in vec3 biTangent;

in vec2 texCoord;

flat in int materialIndex;
flat in int ID;

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

layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outPosition;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outTangent;
layout(location = 4) out vec3 outBiTangent;
layout(location = 5) out vec2 outShininessAlbedo;

void main(void)
{
    if(material[materialIndex].useTexture.x == 1)
        outDiffuse = textureLod(material[materialIndex].sampler, texCoord, textureQueryLod(material[materialIndex].sampler, texCoord).x).xyz;

    else
        outDiffuse = material[materialIndex].colorDiffuse.xyz;

    outPosition = position;
    outNormal = normalize(normal);
    outTangent = normalize(tangent);
    outBiTangent = normalize(biTangent);
    outShininessAlbedo = material[materialIndex].shininessAlbedo.xy;
    outShininessAlbedo.x += 1.0;// to avoid bug, but I don't know why.
}
