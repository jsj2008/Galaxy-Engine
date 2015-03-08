/*!
 * \file ressourcemanager.h
 * \brief Forgive one interface to manage ressources
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef RESSOURCEMANAGER_H
#define RESSOURCEMANAGER_H

#include "../include/include.h"
#include "texture.h"
#include "model.h"

namespace GXY
{
    /**
    * @brief Provide one interface to manage all ressources
    */
    class RessourceManager
    {
    public:
        /**
         * @brief RessourceManager Constructor
         */
        RessourceManager();

        /**
         * @brief Get a Pointer on a Texture asked to be load
         * @param[in] path : Path to find a Texture
         * @return a pointer on a Texture
         */
        std::shared_ptr<Texture> getTexture(std::string const &path);

        /**
         * @brief Get a Pointer on a Model asked to be load
         * @param[in] path : Path to find a Model
         * @return a pointer on a Model
         */
        std::shared_ptr<Model> getModel(std::string const &path);

        /**
          * @brief RessourceManagerDestructor
          */
        ~RessourceManager();

    private:
        std::map<std::string, std::shared_ptr<Texture>> mTextures; //!< Associative table between path and Texture
        std::map<std::string, std::shared_ptr<Model>>mModels; //!< Associative table between path and Model
    };

}

#endif // RESSOURCEMANAGER_H
