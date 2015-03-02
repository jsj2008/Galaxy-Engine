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

        void enableShadowMaps(s32 index);

        ~PointLightNode(void);

    private:
        std::shared_ptr<Node> mParent;
        glm::mat4 mMatrix;
        glm::vec3 mPosition;
        float mRadius;
        glm::vec3 mColor;
        float mIntensity;

        std::tuple<bool, bool, s32> mShadows;
        void mRenderShadowMaps(void);
    };
}

#endif // POINTLIGHTNODE_H
