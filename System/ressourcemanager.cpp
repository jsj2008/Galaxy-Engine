/*!
 * \file ressourcemanager.cpp
 * \brief Forgive one interface to manage ressources
 * \author Antoine MORRIER
 * \version 1.0
 */

#include "ressourcemanager.h"

using namespace glm;
using namespace std;

namespace GXY
{
    RessourceManager::RessourceManager()
    {

    }

    shared_ptr<Texture> RessourceManager::getTexture(const string &path)
    {
        if(mTextures.find(path) == mTextures.end())
        {
            shared_ptr<Texture> texture = make_shared<Texture>(1);
            texture->image(0, path);
            mTextures[path] = texture;
        }

        return mTextures[path];
    }

    shared_ptr<Model> RessourceManager::getModel(const string &path)
    {
        if(mModels.find(path) == mModels.end())
        {
            shared_ptr<Model> model = make_shared<Model>();
            model->load(path);
            mModels[path] = model;
        }

        return mModels[path];
    }

    RessourceManager::~RessourceManager()
    {

    }
}
