#version 440 core

// Uniform
#define CONTEXT 0

// Shader Storage
#define COMMAND 0
#define CLIP 1
#define WORLD 2
#define AABB 3
#define MATERIAL 4

layout(local_size_x = 8, local_size_y = 8) in;

layout(binding = CONTEXT, shared) uniform ContextBuffer
{
    mat4 frustrumMatrix; //!< Is the projectionMatrix product viewMatrix
    uvec4 mNumberMeshes; //!< NumberMeshed : .x
    vec4 planesFrustrum[6];
    vec4 inverseSizeFrameBufferAO; //!< .xy = 1 / sizeScreen, .zw = 1 / sizeAO;
};

// Poisson disk
const vec2 samples[16] = {
vec2(-0.114895, -0.222034),
vec2(-0.0587226, -0.243005),
vec2(0.249325, 0.0183541),
vec2(0.13406, 0.211016),
vec2(-0.382147, -0.322433),
vec2(0.193765, -0.460928),
vec2(0.459788, -0.196457),
vec2(-0.0730352, 0.494637),
vec2(-0.323177, -0.676799),
vec2(0.198718, -0.723195),
vec2(0.722377, -0.201672),
vec2(0.396227, 0.636792),
vec2(-0.372639, -0.927976),
vec2(0.474483, -0.880265),
vec2(0.911652, 0.410962),
vec2(-0.0112949, 0.999936),
};

layout(binding = 0) uniform sampler2D samplerPosition;
layout(binding = 1) uniform sampler2D samplerNormal;

layout(binding = 0, r16f) uniform image2D AOImage;

void main(void)
{
    const vec2 invSize = inverseSizeFrameBufferAO.zw;
    const ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    const vec2 posSampler = pos * invSize;

    const vec3 positionAO = texture(samplerPosition, posSampler).xyz;
    const vec3 N = texture(samplerNormal, posSampler).xyz;

    const vec2 radius = vec2(4.0 * invSize.xy); // 4 pixels
    float total = 0.0;
    float ao = 0.0;

    for(uint i = 0; i < 16; i ++)
    {
        const vec2 texSampler = posSampler + samples[i] * radius;
        const vec3 dirRay = texture(samplerPosition, texSampler).xyz - positionAO;
        const vec3 normalRay = normalize(dirRay);
        const float lengthRay = dot(dirRay, dirRay);
        const float cosTheta = dot(normalRay, N);

        if(cosTheta > 0.01 && lengthRay < 100.0)
        {
            const float theta = acos(cosTheta);
            total += 1.0;
            ao += sin(2.0 * theta);
        }
    }

    imageStore(AOImage, pos, vec4(1.0 - ao / (2.0 * total + 0.01)));
}
