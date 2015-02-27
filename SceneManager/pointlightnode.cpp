#include "pointlightnode.h"
#include "../System/vertexarray.h"
#include "../System/framebuffer.h"
#include "scenemanager.h"

using namespace std;
using namespace glm;

namespace GXY
{
    PointLightNode::PointLightNode(shared_ptr<Node> const &parent) : mParent(parent), mShadowMap(-1)
    {
        for(auto &pov : mPov)
            pov = make_shared<CameraStatic>();
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
        static vec3 const look[] = {vec3(1.0, 0.0, 0.0),
                                    vec3(-1.0, 0.0, 0.0),
                                    vec3(0.0, 1.0, 0.0),
                                    vec3(0.0, -1.0, 0.0),
                                    vec3(0.0, 0.0, 1.0),
                                    vec3(0.0, 0.0, -1.0)};

        static CameraUp const up[] = {CAM_DOWN_Y,
                                 CAM_DOWN_Y,
                                 CAM_UP_Z,
                                 CAM_DOWN_Z,
                                 CAM_DOWN_Y,
                                 CAM_DOWN_Y};

        DrawArrayCommand command;
        PointLight light;
        bool reallocate = false;

        command.baseInstance = 0;
        command.count = 4;
        command.first = global->Lighting.commandPointLights->numElements() * 4;

        light.colorIntensity = vec4(mColor, mIntensity);
        light.positionRadius = vec4((mMatrix * vec4(0, 0, 0, 1)).xyz(), mRadius * mParent->mGlobalScaleFactor);
        light.shadowInformation.x = mShadowMap;

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

        if(mShadowMap > -1)
        {
            for(u32 i = 0; i < 6; ++i)
            {
                mPov[i]->set(vec3(0.0, 500.0, 0.0), vec3(0.0, 500.0, 0.0) + look[i], up[i], radians(90.0f), 1.0f, 1.0f, light.positionRadius.w);

                global->Lighting.pointLightShadowMaps->attachCubeMapArray(CubeMap(POS_X + i), 0);
                global->Lighting.pointLightShadowMaps->bind();
                global->device->setClearColor(1.0, 1.0, 1.0);
                global->device->clearDepthColorBuffer();

                global->sceneManager->pushModelsInPipeline(mPov[i]);

                global->Shaders.depthPointLight->use();

                global->sceneManager->renderDepthPass();
                global->sceneManager->renderModels();
                global->device->setClearColor(0, 0, 0);

                synchronize();
            }
        }
    }

    PointLightNode::~PointLightNode()
    {

    }
}
