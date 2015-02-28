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

layout(binding = CLIP, shared) readonly buffer ClipSpaceBuffer
{
    mat4 toClipSpace[];
};

void main(void)
{
    gl_Position = toClipSpace[gl_DrawIDARB] * vec4(inPos, 1.0);
}
