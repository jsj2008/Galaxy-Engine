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

flat in int ID;
in vec2 texCoord;

layout(binding = 0) uniform sampler2D samplerPosition;
layout(binding = 1) uniform sampler2D samplerNormal;
layout(binding = 2) uniform sampler2D samplerShininessAlbedo;

layout(binding = 3) uniform samplerCubeArray samplerPointLightShadowMaps;

/**
 * @brief Describe some informations to give at Shader for PointLight lighting
 */
struct PointLight
{
    vec4 positionRadius; //!< .xyz = position, w = radius
    vec4 color; //!< .rgb = color, a = intensity
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
    vec3 colorLight = pointLights[ID].color.rgb;
    int shadowMap = pointLights[ID].shadowInformation.x;
    float radiusSquare = positionLightRadius.w * positionLightRadius.w;

    vec3 vertexToLight = positionLightRadius.xyz - position;
    float distanceLightVertex = length(vertexToLight);
    vec3 vertexToLightNormalized = normalize(vertexToLight);

    float attenuation = max(0.0, 1.0 - distanceLightVertex / positionLightRadius.w);

    float lambertCoeff = dot(normal, vertexToLightNormalized) * attenuation;

    if(lambertCoeff > 0.0)
    {
        vec3 dirEye = normalize(posCamera.xyz - position);

        float shine = max(0.0, pow(dot(dirEye, reflect(-vertexToLightNormalized, normal)), shininess));
        color = colorLight.rgb * (lambertCoeff);

        if(shadowMap > -1)
        {
            float z = texture(samplerPointLightShadowMaps, vec4(-vertexToLight, shadowMap)).x;
            float d = distanceLightVertex / positionLightRadius.w;

            color *= min(exp(-positionLightRadius.w * 0.0625 * (d - z)), 1.0);
        }
    }

    else
        color = vec3(0.0);
}
