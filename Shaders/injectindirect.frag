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
#define VPL_POINT_LIGHT 8

in vec2 texCoord;

layout(binding = 0) uniform sampler2D samplerPosition;
layout(binding = 1) uniform sampler2D samplerNormal;

layout(binding = 0, offset = 0) uniform atomic_uint atomicIndex;

/**
 * @brief Describe some informations to give at Shader for Indirect PointLight Lighting
 */
struct PointLightVPL
{
    vec4 positionRadius; //*< .xyz = position, w = radius
    vec4 color; //*< .rgb = color
    vec4 normal; //*< .xyz : Normal oriented Hemisphere for diffuse indirect lighting
};

layout(binding = VPL_POINT_LIGHT, shared) buffer vplBuffer
{
    PointLightVPL vpl[];
};

out vec3 color;

void main(void)
{
    vec3 position = texture(samplerPosition, texCoord).xyz;
    vec3 normal = texture(samplerNormal, texCoord).xyz;
    uint totalVPL = atomicCounter(atomicIndex);

    color = vec3(0.0);

    for(uint i = 0; i < totalVPL; ++i)
    {
        vec4 positionLightRadius = vpl[i].positionRadius;
        vec3 colorLight = vpl[i].color.rgb;
        vec3 orientedLight = vpl[i].normal.xyz;

        vec3 vertexToLight = positionLightRadius.xyz - position;
        float distanceLightVertex = length(vertexToLight);
        vec3 vertexToLightNormalized = normalize(vertexToLight);

        float attenuation = max(0.0, 1.0 - distanceLightVertex / positionLightRadius.w);

        float lambertCoeff = max(0.0, dot(normal, vertexToLightNormalized)) * attenuation * max(0.0, dot(orientedLight, -vertexToLightNormalized));
        color += colorLight.rgb * (lambertCoeff);
    }

    color /= 3.14;
}
