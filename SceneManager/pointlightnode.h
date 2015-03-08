#ifndef POINTLIGHTNODE_H
#define POINTLIGHTNODE_H

#include "../include/include.h"
#include "node.h"

namespace GXY
{
    /**
     * @brief The PointLightNode class
     *
     * Describes one PointLight in a Node
     */
    class PointLightNode
    {
    public:
        /**
         * @brief PointLightNode constructor
         * @param[in] node : The parent of this PointLightNode
         */
        PointLightNode(std::shared_ptr<Node> const &node);

        /**
         * @brief Set a position of a Light
         * @param[in] position : new position
         */
        void setPosition(glm::vec3 const &position);

        /**
         * @brief Set a radius of a Light
         * @param[in] radius : new radius
         */
        void setRadius(float radius);

        /**
         * @brief Set a color of a Light
         * @param[in] color : new color
         */
        inline void setColor(glm::vec3 const &color) {mColor = color;}

        /**
         * @brief setIntensity
         * @param[in] intensity : new intensity
         */
        inline void setIntensity(float intensity) {mIntensity = intensity;}

        /**
         * @brief If a transformation of a Node is performed, matrix is recomputed
         */
        void updateMatrix(void);

        /**
         * @brief Push Light in Pipeline of Light
         * @param[in] frustrum : Frustrum's Camera
         */
        void pushInPipeline(Frustrum const &frustrum);

        void enableShadowMaps(s32 index);
        void enableVirtualLight(void);

        ~PointLightNode(void);

    private:
        std::shared_ptr<Node> mParent;
        glm::mat4 mMatrix;
        glm::vec3 mPosition;
        float mRadius;
        glm::vec3 mColor;
        float mIntensity;

        std::tuple<bool, bool, s32> mShadows;
        std::tuple<bool, bool> mVirtualLight;

        void mRenderShadowMaps(void);
        void mCreateVirtualLights(void);
    };
}

#endif // POINTLIGHTNODE_H
