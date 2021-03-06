/*!
 * \file node.h
 * \brief One node of the SceneManager
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef NODE_H
#define NODE_H

#include "../include/include.h"
#include "../System/model.h"
#include "../Camera/camera.h"
#include "node.h"

namespace GXY
{
    class ModelNode;
    class PointLightNode;

    /**
     * @brief The Node class
     *
     * Provide one interface to Manage Dependance
     */
    class Node
    {
        /**
         * @brief Add one Node in the tree Parent
         * @param[in] parent
         * @return A pointer on this Node
         */
        friend std::shared_ptr<Node> addNode(std::shared_ptr<Node> const &parent);

        /**
         * @brief Add one Model in the Node
         * @param[in] parent
         * @param[in] path : Path to find Asset
         * @return
         */
        friend std::shared_ptr<ModelNode> addModel(std::shared_ptr<Node> const &parent, std::string const &path);

        friend std::shared_ptr<PointLightNode> addPointLight(const std::shared_ptr<Node> &parent);

        friend ModelNode;
        friend PointLightNode;

    public:
        /**
         * @brief NodeConstructor
         * @param[in] parent : a Pointer on a Node Parent
         */
        Node(glm::mat4 const &parent);

        /**
         * @brief Perform a rotation of a Node : All models and children are affected
         * @param[in] angle : Angle of the rotation
         * @param[in] axe : Axe of rotation
         */
        void rotate(float angle, glm::vec3 const &axe);

        /**
         * @brief Perform a translation of a Node : All models and children are affected
         * @param[in] vec : Vector of translation
         */
        void translate(glm::vec3 const &vec);

        /**
         * @brief Perform a Homothetic transformation of a Node : All models and children are affected
         * @param[in] factor : Factor of Homothetic transformation
         */
        void scale(float factor);

        /**
         * @brief Set the default value for the matrix
         */
        void identity(void);

        /**
         * @brief Push in a command Buffer news value to be render of all Models in a Node
         * @param[in] frustrum
         */
        void pushModelsInPipeline(Frustrum const &frustrum);

        void pushPointLightsInPipeline(Frustrum const &frustrum);

    private:
        std::shared_ptr<Node> mParent; //!< Parent Node
        glm::mat4 mGlobalMatrix; //!< GlobalMatrix : Depend of Parent Node
        AABB3D mAABB; //!< Bounding Box of this Node : Depend of Child and Model Bounding Boxe

        float mGlobalScaleFactor; //!< Scale Factor

        std::vector<std::shared_ptr<Node>> mChildren; //!< Children Node
        std::vector<std::shared_ptr<ModelNode>> mModels; //!< Model owned by Node
        std::vector<std::shared_ptr<PointLightNode>> mPointLights; //!< PointLight owned by Node

        /**
         * @brief Apply scale Factor to all tree
         * @param[in] factor
         */
        void mApplyScaleFactorToChildren(float factor);

        /**
         * @brief Apply matrix to all tree
         * @param[in] matrix
         */
        void mApplyMatrixToChildrenAndModels(glm::mat4 const &matrix);

        /**
         * @brief Perform changement on the AABB on go up in the tree.
         */
        void mActualizeBoundingBoxes();

        /**
         * @brief Compute all bounding box for the AABB.
         */
        void mBuildAABB();
    };

    std::shared_ptr<Node> addNode(std::shared_ptr<Node> const &parent);
    std::shared_ptr<ModelNode> addModel(std::shared_ptr<Node> const &parent, std::string const &path);
    std::shared_ptr<PointLightNode> addPointLight(std::shared_ptr<Node> const &parent);
}


#endif // NODE_H
