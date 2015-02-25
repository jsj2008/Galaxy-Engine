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
    uvec4 numberMeshes; //!< NumberMeshed : .x
    vec4 planesFrustrum[6];
    vec4 inverseSizeFrameBufferAO; //!< .xy = 1 / sizeScreen, .zw = 1 / sizeAO;
};

struct DrawElementCommand
{
    uint count; //!< Number of index taken on the IndexBuffer
    uint primCount; //!< Number of instance : 1 in our engine
    uint firstIndex; //!< first Index if big IndexBuffer
    uint baseVertex; //!< first Vertex if big VertexBuffer
    uint baseInstance; //!< first Instance if several Instances : 0 in our engine
};

struct AABB3D
{
    vec4 coord[8]; //!< One box own 8 vertex
};

layout(binding = COMMAND, shared) writeonly buffer CommandBuffer
{
    DrawElementCommand command[];
};

layout(binding = CLIP, shared) writeonly buffer ClipSpaceBuffer
{
    mat4 toClipSpace[];
};

layout(binding = WORLD, shared) readonly buffer WorldSpaceBuffer
{
    mat4 toWorldSpace[];
};

layout(binding = AABB, shared) buffer AABBBuffer
{
    AABB3D box[];
};

void main(void)
{
    AABB3D newBox;
    if(gl_GlobalInvocationID.x < numberMeshes.x)
    {
        toClipSpace[gl_GlobalInvocationID.x] = frustrumMatrix * toWorldSpace[gl_GlobalInvocationID.x];

        for(uint i = 0; i < 8; ++i)
            newBox.coord[i] = toWorldSpace[gl_GlobalInvocationID.x] * box[gl_GlobalInvocationID.x].coord[i];
    }

    for(uint i = 0; i < 6; ++i)
    {
        bool isIn = false;

        for(uint j = 0; j < 8 && !isIn; ++j)
        {
            if(dot(planesFrustrum[i], newBox.coord[j]) > 0.0)
            {
                isIn = true;
                break;
            }
        }

        if(!isIn)
        {
            command[gl_GlobalInvocationID.x].primCount = 0;
            return;
        }
    }

    command[gl_GlobalInvocationID.x].primCount = 1;
}
