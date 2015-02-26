#include "pointlightnode.h"
#include "../System/vertexarray.h"

using namespace std;
using namespace glm;

namespace GXY
{
    PointLightNode::PointLightNode(shared_ptr<Node> const &parent) : mParent(parent)
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

    void PointLightNode::pushInPipeline(void)
    {
        DrawArrayCommand command;
        PointLight light;
        bool reallocate = false;

        command.baseInstance = 0;
        command.count = 4;
        command.first = global->Lighting.commandPointLights->numElements() * 4;

        light.colorIntensity = vec4(mColor, mIntensity);
        light.positionRadius.w = mRadius * mParent->mGlobalScaleFactor;

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
            global->Lighting.commandPointLights->bindBase(SHADER_STORAGE, 8);
        }
    }

    PointLightNode::~PointLightNode()
    {

    }
}
