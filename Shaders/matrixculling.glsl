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
#define PROJECT_LIGHT 5
#define POINT_LIGHT 6
#define WORLD_POINT_LIGHT 7
#define COMMAND_POINT_LIGHT 8

layout(local_size_x = 64) in;

layout(binding = FRUSTRUM, shared) uniform FrustrumBuffer
{
    mat4 frustrumMatrix; //!< Is the projectionMatrix product viewMatrix
    vec4 planesFrustrum[6];
    uvec4 numberMeshesPointLights; //!< NumberMeshed : .x, .y = NumberPointLights
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
    if(gl_GlobalInvocationID.x < numberMeshesPointLights.x)
    {
        AABB3D newBox;

        toClipSpace[gl_GlobalInvocationID.x] = frustrumMatrix * toWorldSpace[gl_GlobalInvocationID.x];

        for(uint i = 0; i < 8; ++i)
            newBox.coord[i] = toWorldSpace[gl_GlobalInvocationID.x] * box[gl_GlobalInvocationID.x].coord[i];

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
}
