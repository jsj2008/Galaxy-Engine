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

layout(location = 0) in vec2 inPos;

out vec2 texCoord;

void main(void)
{
    gl_Position = vec4(inPos, 0.0, 1.0);
    texCoord = inPos * 0.5 + 0.5;
}
