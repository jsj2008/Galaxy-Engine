#version 440 core

// Uniform
#define CONTEXT 0

// Shader Storage
#define COMMAND 0
#define CLIP 1
#define WORLD 2
#define AABB 3
#define MATERIAL 4

layout(local_size_y = 64) in;

layout(binding = CONTEXT, shared) uniform ContextBuffer
{
    mat4 frustrumMatrix; //!< Is the projectionMatrix product viewMatrix
    uvec4 numberMeshes; //!< NumberMeshed : .x
    vec4 planesFrustrum[6];
    vec4 inverseSizeFrameBufferAO; //!< .xy = 1 / sizeScreen, .zw = 1 / sizeAO;
};

layout(binding = 0) uniform sampler2D noBlur;
layout(binding = 0, r16f) uniform image2D blurImage;

void main(void)
{
    float value = 0.0;
    const vec2 invSize = inverseSizeFrameBufferAO.zw;
    const ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    for(int i = -3; i < 4; ++i)
        value += texture(noBlur, (pos + ivec2(0, i)) * invSize).x;

    imageStore(blurImage, pos, vec4(value / 7.0));
}
