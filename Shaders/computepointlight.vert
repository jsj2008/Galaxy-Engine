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

layout(location = 0) in vec2 inPos;

flat out int ID;
out vec2 texCoord;

void main(void)
{
    ID = gl_DrawIDARB;
    texCoord = inPos * 0.5 + 0.5;
    gl_Position = vec4(inPos, 1.0, 1.0);
}
