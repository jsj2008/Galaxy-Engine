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

flat in int ID;
in vec2 texCoord;

layout(binding = 0) uniform sampler2D samplerPosition;
layout(binding = 1) uniform sampler2D samplerNormal;
layout(binding = 2) uniform sampler2D samplerShininessAlbedo;

/**
 * @brief Describe some informations to give at Shader for PointLight lighting
 */
struct PointLight
{
    vec4 positionRadius; //!< .xyz = position, w = radius
    vec4 colorIntensity; //!< .rgb = color, a = intensity
    ivec4 shadowInformation; //!< .x = index of Cube Shadow Map
};

layout(binding = FRUSTRUM, shared) uniform FrustrumBuffer
{
    mat4 frustrumMatrix; //!< Is the projectionMatrix product viewMatrix
    vec4 posCamera; //!< .xyz = posCamera or PosLight for shadowMaps for example
    vec4 planesFrustrum[6];
    uvec4 numberMeshesPointLights; //!< NumberMeshed : .x, .y = NumberPointLights
};

layout(binding = POINT_LIGHT) buffer PointLightBuffer
{
    PointLight pointLights[];
};

out vec3 color;

void main(void)
{
    vec3 position = texture(samplerPosition, texCoord).xyz;
    vec3 normal = texture(samplerNormal, texCoord).xyz;
    float shininess = texture(samplerShininessAlbedo, texCoord).x;

    vec4 positionLightRadius = pointLights[ID].positionRadius;
    vec4 colorIntensity = pointLights[ID].colorIntensity;
    float radiusSquare = positionLightRadius.w * positionLightRadius.w;

    vec3 vertexToLight = positionLightRadius.xyz - position;
    float distanceLightVertexSquare = dot(vertexToLight, vertexToLight);
    vec3 vertexToLightNormalized = normalize(vertexToLight);

    float attenuationQuadratic = (64.0 * colorIntensity.w - 1.0) / radiusSquare;

    float attenuation = colorIntensity.w / (1.0 + attenuationQuadratic * distanceLightVertexSquare);

    float lambertCoeff = dot(normal, vertexToLightNormalized);

    if(lambertCoeff > 0.0)
    {
        vec3 dirEye = normalize(posCamera.xyz - position);

        float shine = max(0.0, pow(dot(dirEye, reflect(-vertexToLightNormalized, normal)), shininess));
        color = attenuation * colorIntensity.xyz * (shine + lambertCoeff);
    }

    else
        color = vec3(0.0);
}
