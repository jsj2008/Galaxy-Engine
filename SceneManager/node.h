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

    /**
     * @brief The Node class
     *
     * Provide one interface to Manage Dependance
     */
    class Node
    {
        /**
         * @brief Add one Node in the tree Parent
         * @param parent
         * @return A pointer on this Node
         */
        friend std::shared_ptr<Node> addNode(std::shared_ptr<Node> parent);

        /**
         * @brief Add one Model in the Node
         * @param parent
         * @param path : Path to find Asset
         * @return
         */
        friend std::shared_ptr<ModelNode> addModel(std::shared_ptr<Node> parent, std::string const &path);
        friend ModelNode;

    public:
        /**
         * @brief NodeConstructor
         * @param parent : a Pointer on a Node Parent
         */
        Node(glm::mat4 const &parent);

        /**
         * @brief Perform a rotation of a Node : All models and children are affected
         * @param angle : Angle of the rotation
         * @param axe : Axe of rotation
         */
        void rotate(float angle, glm::vec3 const &axe);

        /**
         * @brief Perform a translation of a Node : All models and children are affected
         * @param vec : Vector of translation
         */
        void translate(glm::vec3 const &vec);

        /**
         * @brief Perform a Homothetic transformation of a Node : All models and children are affected
         * @param factor : Factor of Homothetic transformation
         */
        void scale(float factor);

        /**
         * @brief Set the default value for the matrix
         */
        void identity(void);

        /**
         * @brief Push in a command Buffer news value to be render of all Models in a Node
         * @param frustrum
         */
        void pushModelsInPipeline(Frustrum const &frustrum);

    private:
        std::shared_ptr<Node> mParent; //!< Parent Node
        glm::mat4 mGlobalMatrix; //!< GlobalMatrix : Depend of Parent Node
        AABB3D mAABB; //!< Bounding Box of this Node : Depend of Child and Model Bounding Boxe

        float mGlobalScaleFactor; //!< Scale Factor

        std::vector<std::shared_ptr<Node>> mChildren; //!< Children Node
        std::vector<std::shared_ptr<ModelNode>> mModels; //!< Model owned by Node

        /**
         * @brief Apply scale Factor to all tree
         * @param factor
         */
        void mApplyScaleFactorToChildren(float factor);

        /**
         * @brief Apply matrix to all tree
         * @param matrix
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

    std::shared_ptr<Node> addNode(std::shared_ptr<Node> parent);
    std::shared_ptr<ModelNode> addModel(std::shared_ptr<Node> parent, std::string const &path);
}


#endif // NODE_H
