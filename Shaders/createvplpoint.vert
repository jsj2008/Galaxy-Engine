#version 440 core
#extension GL_ARB_shader_draw_parameters : enable

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

layout(location = 0) in vec3 inPos;

layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inBiTangent;

layout(location = 4) in vec2 inTexCoord;

layout(location = 5) flat in int inMaterialIndex;

layout(binding = CLIP, shared) readonly buffer ClipSpaceBuffer
{
    mat4 toClipSpace[];
};

layout(binding = WORLD, shared) readonly buffer WorldSpaceBuffer
{
    mat4 toWorldSpace[];
};

out vec3 position;

out vec3 normal;
out vec3 tangent;
out vec3 biTangent;

out vec2 texCoord;

flat out int materialIndex;

void main(void)
{
    materialIndex = inMaterialIndex;
    texCoord = inTexCoord;
    mat3 normalMatrix = transpose(inverse(mat3(toWorldSpace[gl_DrawIDARB])));
    normal = normalMatrix * inNormal;
    tangent = normalMatrix * inTangent;
    biTangent = normalMatrix * inBiTangent;

    position = (toWorldSpace[gl_DrawIDARB] * vec4(inPos, 1.0)).xyz;
    gl_Position = toClipSpace[gl_DrawIDARB] * vec4(inPos, 1.0);
}
