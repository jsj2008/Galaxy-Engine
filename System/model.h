/*!
 * \file model.h
 * \brief Load Assets into Vram
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef MODEL_H
#define MODEL_H

#include "../include/include.h"

/**
 * @namespace GXY
 * @brief Namespace for Galaxy Engine
 */
namespace GXY
{
    /**
     * @brief Provide loading assets
     * 
     * Vertex, Index are stocked in principal buffer on Global structure
     * Commands and Bounding Boxes are stocked on vector
     */
    class Model
    {
    public:
        /**
         * @brief Model Constructor
         */
        Model();

        /**
         * @brief Load one asset
         * @param[in] path : Path to load the Asset
         */
        void load(std::string const &path);
        
        /**
         * @brief Get the global bounding boxes
         * @return AABB3D
         */        
        AABB3D const &AABB(void) const{return mAABB;}

        /**
         * @brief Push data in different Buffer
         * @param[in] transform : World Matrix
         */
        void pushInPipeline(glm::mat4 const &transform);

        /** 
         * @brief Model Destructor
         */
        ~Model();

    private:
        AABB3D mAABB; //!< The Total Bounding Box
        std::vector<AABB3D> mMeshesAABB; //!< Bounding Boxes
        std::vector<DrawElementCommand> mMeshesCommand; //!< Command rendering
    };

}

#endif // MODEL_H
