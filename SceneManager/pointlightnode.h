#ifndef POINTLIGHTNODE_H
#define POINTLIGHTNODE_H

#include "../include/include.h"
#include "node.h"

namespace GXY
{
    class PointLightNode
    {
    public:
        PointLightNode(std::shared_ptr<Node> const &node);

        void setPosition(glm::vec3 const &position);
        void setRadius(float radius);
        inline void setColor(glm::vec3 const &color) {mColor = color;}
        inline void setIntensity(float intensity) {mIntensity = intensity;}

        void updateMatrix(void);

        void pushInPipeline(Frustrum const &frustrum);

        void enableShadow(void){mShadowMap = 0;}

        ~PointLightNode(void);

    private:
        std::shared_ptr<Node> mParent;
        glm::mat4 mMatrix;
        glm::vec3 mPosition;
        float mRadius;
        glm::vec3 mColor;
        float mIntensity;
        s32 mShadowMap;
        std::shared_ptr<CameraStatic> mPov[6];
    };
}

#endif // POINTLIGHTNODE_H
