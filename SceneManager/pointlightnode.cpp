#include "pointlightnode.h"
#include "../System/vertexarray.h"
#include "../System/framebuffer.h"
#include "scenemanager.h"

using namespace std;
using namespace glm;

namespace GXY
{
    PointLightNode::PointLightNode(shared_ptr<Node> const &parent) :
        mParent(parent), mShadows(false, false, -1), mVirtualLight(false, false)
    {

    }

    void PointLightNode::setPosition(const vec3 &position)
    {
        mPosition = position;
        mMatrix = scale(translate(mParent->mGlobalMatrix, mPosition), vec3(mRadius));
    }

    void PointLightNode::setRadius(float radius)
    {
        mRadius = radius;
        mMatrix = scale(translate(mParent->mGlobalMatrix, mPosition), vec3(mRadius));
    }

    void PointLightNode::updateMatrix(void)
    {
        mMatrix = scale(translate(mParent->mGlobalMatrix, mPosition), vec3(mRadius));
    }

    void PointLightNode::pushInPipeline(Frustrum const &frustrum)
    {
        DrawArrayCommand command;
        PointLight light;
        Sphere sphere;
        bool reallocate = false;

        command.baseInstance = 0;
        command.instanceCount = 1;
        command.count = 4;
        command.first = global->Lighting.commandPointLights->numElements() * 4;

        light.colorIntensity = vec4(mColor, mIntensity);
        light.positionRadius = vec4(mMatrix[3].xyz(), mRadius * mParent->mGlobalScaleFactor);
        light.shadowInformation.x = get<2>(mShadows);

        sphere.position = light.positionRadius.xyz();
        sphere.radius = light.positionRadius.w;

        if(!frustrum.sphereInside(sphere))
            return;

        global->Lighting.commandPointLights->push(command, reallocate);
        global->Lighting.pointLight->push(light, reallocate);
        global->Lighting.toWorldSpace->push(mMatrix, reallocate);

        if((global->Lighting.commandPointLights->numElements() * 4 > global->Lighting.quadsPointLights->numMaxElements()))
        {
            global->Lighting.quadsPointLights->allocate(4 * global->Lighting.commandPointLights->numElements());

            global->Lighting.vaoPointLight->create();
            global->Lighting.vaoPointLight->configure(*global->Lighting.quadsPointLights);
        }

        if(reallocate)
        {
            global->Lighting.quadsPointLights->bindBase(SHADER_STORAGE, 5);
            global->Lighting.pointLight->bindBase(SHADER_STORAGE, 6);
            global->Lighting.toWorldSpace->bindBase(SHADER_STORAGE, 7);
        }

        if(get<0>(mShadows) == true)
            if(get<1>(mShadows) == false)
                mRenderShadowMaps();

        if(get<0>(mVirtualLight) == true)
            if(get<1>(mVirtualLight) == false)
                mCreateVirtualLights();
    }

    void PointLightNode::enableShadowMaps(s32 index)
    {
        get<0>(mShadows) = true;
        get<1>(mShadows) = false;
        get<2>(mShadows) = index;
    }

    void PointLightNode::enableVirtualLight(void)
    {
        get<0>(mVirtualLight) = true;
        get<1>(mVirtualLight) = false;
    }

    void PointLightNode::mRenderShadowMaps(void)
    {
        get<1>(mShadows) = true;

        global->device->setClearColor(1.0, 1.0, 1.0);
            renderIntoCubeMapArray(global->Lighting.pointLightShadowMaps, get<2>(mShadows), vec4(mMatrix[3].xyz(), mRadius * mParent->mGlobalScaleFactor), global->Shaders.depthPointLight);
        global->device->setClearColor(0.0, 0.0, 0.0);
    }

    void PointLightNode::mCreateVirtualLights(void)
    {
        get<1>(mVirtualLight) = true;


    }

    PointLightNode::~PointLightNode()
    {

    }
}
