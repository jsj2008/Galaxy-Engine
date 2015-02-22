#version 440 core

// Uniform
#define CONTEXT 0

// Shader Storage
#define COMMAND 0
#define CLIP 1
#define WORLD 2
#define AABB 3
#define MATERIAL 4

layout(local_size_x = 128) in;

layout(binding = CONTEXT, shared) uniform ContextBuffer
{
    mat4 frustrumMatrix; //!< Is the projectionMatrix product viewMatrix
    uvec4 mNumberMeshes; //!< NumberMeshed : .x
    vec4 planesFrustrum[6];
    vec4 inverseSizeFrameBufferAO; //!< .xy = 1 / sizeScreen, .zw = 1 / sizeAO;
};

void main(void)
{

}
