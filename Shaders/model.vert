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
#define COMMAND_POINT_LIGHT 8

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) flat in int inMaterialIndex;

out vec3 position;
out vec3 normal;
out vec2 texCoord;
flat out int materialIndex;
flat out int ID;

layout(binding = CLIP, shared) readonly buffer ClipSpaceBuffer
{
    mat4 toClipSpace[];
};

layout(binding = WORLD, shared) readonly buffer WorldSpaceBuffer
{
    mat4 toWorldSpace[];
};

void main(void)
{
    ID = gl_DrawIDARB;
    materialIndex = inMaterialIndex;
    texCoord = inTexCoord;
    normal = mat3(toWorldSpace[ID]) * inNormal;
    position = (toWorldSpace[ID] * vec4(inPos, 1.0)).xyz;
    gl_Position = toClipSpace[ID] * vec4(inPos, 1.0);
}
