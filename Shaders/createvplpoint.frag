#version 440 core
#extension GL_ARB_bindless_texture : enable


layout(early_fragment_tests) in;

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
#define VPL_POINT_LIGHT 8

layout(binding = 0, offset = 0) uniform atomic_uint atomicIndex;

in vec3 position;

in vec3 normal;

in vec2 texCoord;

flat in int materialIndex;

struct Material
{
    vec4 colorDiffuse;
    vec4 shininessAlbedo;
    sampler2D sampler;
    uvec2 useTexture;
};

/**
 * @brief Describe some informations to give at Shader for Indirect PointLight Lighting
 */
struct PointLightVPL
{
    vec4 positionRadius; //*< .xyz = position, w = radius
    vec4 color; //*< .rgb = color
    vec4 normal; //*< .xyz : Normal oriented Hemisphere for diffuse indirect lighting
};

layout(binding = MATERIAL, shared) readonly buffer MaterialBuffer
{
    Material material[];
};

layout(binding = VPL_POINT_LIGHT, shared) buffer vplBuffer
{
    PointLightVPL vpl[];
};

layout(binding = FRUSTRUM, shared) uniform FrustrumBuffer
{
    mat4 frustrumMatrix; //!< Is the projectionMatrix product viewMatrix
    vec4 posCamera; //!< .xyz = posCamera or PosLight for shadowMaps for example
    vec4 planesFrustrum[6];
    uvec4 numberMeshesPointLights; //!< NumberMeshed : .x, .y = NumberPointLights
};

void main(void)
{
    uint index = atomicCounterIncrement(atomicIndex);
    PointLightVPL light;

    if(material[materialIndex].useTexture.x == 1)
        light.color.rgb = textureLod(material[materialIndex].sampler, texCoord, textureQueryLod(material[materialIndex].sampler, texCoord).x).xyz;

    else
        light.color.rgb = material[materialIndex].colorDiffuse.xyz;

    light.color *= 4.0 / (6.0 * 8.0 * 8.0);
    light.positionRadius.xyz = position;
    light.positionRadius.w = posCamera.w;
    light.normal.xyz     = normalize(normal);
    vpl[index] = light;
}
