#version 440 core

layout(local_size_x = 64)in;

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

struct Quad
{
    vec2 quad[4];
};

/**
 * @brief Describe some informations to give at Shader for PointLight lighting
 */
struct PointLight
{
    vec4 positionRadius; //!< .xyz = position, w = radius
    vec4 color; //!< .rgb = color
    ivec4 shadowInformation; //!< .x = index of Cube Shadow Map
};

layout(binding = FRUSTRUM, shared) uniform FrustrumBuffer
{
    mat4 frustrumMatrix; //!< Is the projectionMatrix product viewMatrix
    vec4 posCamera; //!< .xyz = posCamera or PosLight for shadowMaps for example
    vec4 planesFrustrum[6];
    uvec4 numberMeshesPointLights; //!< NumberMeshed : .x, .y = NumberPointLights
};

layout(binding = PROJECT_LIGHT) buffer QuadBuffer
{
    Quad quad[];
};

layout(binding = POINT_LIGHT) buffer PointLightBuffer
{
    PointLight pointLights[];
};

layout(binding = WORLD_POINT_LIGHT) buffer MatrixBuffer
{
    mat4 worldMatrix[];
};

const vec4 CUBE[] =
{
    vec4(-1.0, -1.0, -1.0, 1.0),
    vec4(-1.0, -1.0, 1.0, 1.0),
    vec4(-1.0, 1.0, -1.0, 1.0),
    vec4(-1.0, 1.0, 1.0, 1.0),
    vec4(1.0, -1.0, -1.0, 1.0),
    vec4(1.0, -1.0, 1.0, 1.0),
    vec4(1.0, 1.0, -1.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
};

void main(void)
{
    if(gl_GlobalInvocationID.x < numberMeshesPointLights.y)
    {
        mat4 toClipSpace = frustrumMatrix * worldMatrix[gl_GlobalInvocationID.x];
        vec2 mini = vec2(2, 2);
        vec2 maxi = vec2(-2, -2);

        /** We render the cube **/
        for(uint i = 0; i < 8; ++i)
        {
            vec4 clipPoint = toClipSpace * CUBE[i];

            // If you are outside the light
            if(clipPoint.w > 0)
            {
                clipPoint.xy /= clipPoint.w;

                mini = min(clipPoint.xy, mini);
                maxi = max(clipPoint.xy, maxi);
            }

            // Else, you are inside the light, so the light affect all screen
            else
            {
                mini = vec2(-1.0);
                maxi = vec2(1.0);
                break;
            }
        }

        quad[gl_GlobalInvocationID.x].quad[0] = vec2(mini.x, mini.y);
        quad[gl_GlobalInvocationID.x].quad[1] = vec2(mini.x, maxi.y);
        quad[gl_GlobalInvocationID.x].quad[2] = vec2(maxi.x, mini.y);
        quad[gl_GlobalInvocationID.x].quad[3] = vec2(maxi.x, maxi.y);
    }
}
