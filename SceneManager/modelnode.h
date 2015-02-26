/*!
 * \file modelnode.h
 * \brief Describe a Model in a Node
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef MODELNODE_H
#define MODELNODE_H

#include "../include/include.h"
#include "../System/model.h"
#include "node.h"

namespace GXY
{
    /**
     * @brief The ModelNode class
     *
     * Describe A Model in a node.
     * This Model is associate with one matrix
     */
    class ModelNode
    {
    public:
        /**
         * @brief ModelNode Constructor
         * @param path : Path to find Asset
         * @param parent : Node Parent
         */
        ModelNode(std::string const &path, std::shared_ptr<Node> const &parent);

        /**
         * @brief Perform a rotation of a Model inside a Node
         * @param angle : Angle of the rotation
         * @param axe : Axe of rotation
         */
        void rotate(float angle, glm::vec3 const &axe);

        /**
         * @brief Perform a translation of a Model inside a Node
         * @param vec : Vector of translation
         */
        void translate(glm::vec3 const &vec);

        /**
         * @brief Perform a Homothetic transformation of a Model inside a Node
         * @param factor : Factor of Homothetic transformation
         */
        void scale(float factor);

        /**
         * @brief Set the default value for the matrix
         */
        void identity(void);

        /**
         * @brief updateAABB : If Node undergoes a transformation, this function should be call
         * @param globalMatrix
         */
        inline void updateAABB(void) {mAABB = computeAABB3D(mModel->AABB(), mParent->mGlobalMatrix * mMatrix);}

        /**
         * @brief AABB : Get the Bounding Box of this object in the Real World
         * @return AABB3D
         */
        inline AABB3D const &AABB(void) const {return mAABB;}

        /**
         * @brief Push in a command Buffer news value to be render
         */
        void pushInPipeline(void);

    private:
        std::shared_ptr<Node> mParent; //!< Node Parent
        std::shared_ptr<Model> mModel; //!< Pointer on a Model
        glm::mat4 mMatrix; //!< Matrix relative to a Node
        AABB3D mAABB; //!< Bounding Box in a Real World
    };
}

#endif // MODELNODE_H
