/*!
 * \file model.cpp
 * \brief Load Assets into Vram
 * \author Antoine MORRIER
 * \version 1.0
 */

#include "model.h"
#include "device.h"
#include "vertexarray.h"
#include "shader.h"

using namespace std;
using namespace glm;
using namespace Assimp;

namespace GXY
{    
    string getDir(string const &path)
    {
        size_t last = path.find_last_of('/');

        // we are in the good dirent
        if(last == string::npos)
            return string();

        string dir = path;
        dir.erase(last, dir.size());
        return dir;
    }

    Model::Model()
    {

    }

    void Model::load(const string &path)
    {
        Importer imp;

        vec3 minTotal(FLT_MAX, FLT_MAX, FLT_MAX);
        vec3 maxTotal(FLT_MIN, FLT_MIN, FLT_MIN);

        bool isReallocateVertex = false;
        bool isReallocateIndex = false;
        bool isReallocateMaterial = false;

        aiScene const *scene = imp.ReadFile(path, aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);

        if(scene == NULL)
            throw Except("Impossible to open : " + path);

        // For command rendering, to know where we have to take data
        u32 baseVertex = global->Model.vertex->numElements();
        u32 baseIndex = global->Model.index->numElements();
        u32 baseMaterial = global->Model.material->numElements();

        // Loading materials
        for(u32 i = 0; i < scene->mNumMaterials; ++i)
        {
            aiMaterial *current = scene->mMaterials[i];
            aiString texPath;
            Material material;

            if(current->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
            {
                string CtexPath = texPath.C_Str();

                if(CtexPath[0] == '.' && (CtexPath[1] == '\\' || CtexPath[1] == '/'))
                {
                    CtexPath.erase(0, 1);
                    CtexPath[0] = '/';
                }

                if(CtexPath.find('\\') != string::npos)
                    CtexPath.insert(0, "/");

                auto it = CtexPath.begin();

                while(it != CtexPath.end())
                {
                    if(*it == '\\')
                        *it = '/';
                    ++it;
                }

                string totalPath = getDir(path) + CtexPath;
                u64 handle = global->ressourceManager->getTexture(totalPath)->getHandle(0);
                memcpy(&material.textureHandleUseTexture, &handle, sizeof handle);
                material.textureHandleUseTexture.z = 1.0;
            }

            else
            {
                aiColor3D c;

                material.textureHandleUseTexture.z = 0.0;

                if(current->Get(AI_MATKEY_COLOR_DIFFUSE, c) == AI_SUCCESS)
                    material.colorDiffuse = vec4(c.r, c.g, c.b, 1.0);
            }

            material.shininessAlbedo = vec4(0.0, 1.0, 0.0, 0.0);
            global->Model.material->push(material, isReallocateMaterial);
        }

        mMeshesAABB.resize(scene->mNumMeshes);
        mMeshesCommand.resize(scene->mNumMeshes);

        for(u32 i = 0; i < scene->mNumMeshes; ++i)
        {
            aiMesh *mesh = scene->mMeshes[i];

            vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
            vec3 max(FLT_MIN, FLT_MIN, FLT_MIN);

            // Vertex
            for(u32 j = 0; j < mesh->mNumVertices ; ++j)
            {
                Vertex vertex;
                vec3 position;

                position = vertex.position = vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
                vertex.normal = vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);

                if(mesh->HasTextureCoords(0))
                    vertex.texCoord = vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);

                vertex.materialIndex = mesh->mMaterialIndex + baseMaterial;

                global->Model.vertex->push(vertex, isReallocateVertex);
                global->Model.vertexDepth->push(position, isReallocateVertex);

                min = glm::min(min, position);
                max = glm::max(max, position);

                minTotal = glm::min(minTotal, position);
                maxTotal = glm::max(maxTotal, position);
            }

            // Index
            for(u32 j = 0; j < mesh->mNumFaces; ++j)
                for(u32 k = 0; k < 3; ++k)
                    global->Model.index->push(mesh->mFaces[j].mIndices[k], isReallocateIndex);

            // Command
            mMeshesCommand[i].count = mesh->mNumFaces * 3;
            mMeshesCommand[i].primCount = 1;
            mMeshesCommand[i].firstIndex = baseIndex;
            mMeshesCommand[i].baseVertex = baseVertex;
            mMeshesCommand[i].baseInstance = 0;

            // Update
            baseIndex += mesh->mNumFaces * 3;
            baseVertex += mesh->mNumVertices;

            mMeshesAABB[i].coord[0] = vec4(min.x, min.y, min.z, 1.0);
            mMeshesAABB[i].coord[1] = vec4(min.x, min.y, max.z, 1.0);
            mMeshesAABB[i].coord[2] = vec4(min.x, max.y, min.z, 1.0);
            mMeshesAABB[i].coord[3] = vec4(min.x, max.y, max.z, 1.0);
            mMeshesAABB[i].coord[4] = vec4(max.x, min.y, min.z, 1.0);
            mMeshesAABB[i].coord[5] = vec4(max.x, min.y, max.z, 1.0);
            mMeshesAABB[i].coord[6] = vec4(max.x, max.y, min.z, 1.0);
            mMeshesAABB[i].coord[7] = vec4(max.x, max.y, max.z, 1.0);
        }

        if(isReallocateVertex || isReallocateIndex)
        {
            global->Model.vao->create();
            global->Model.vaoDepth->create();

            global->Model.vao->configure(*global->Model.vertex);
            global->Model.vaoDepth->configure(*global->Model.vertexDepth);

            global->Model.vao->bindElementBuffer(*global->Model.index);
            global->Model.vaoDepth->bindElementBuffer(*global->Model.index);
        }

        if(isReallocateMaterial)
            global->Model.material->bindBase(SHADER_STORAGE, 4);

        // AABB
        mAABB.coord[0] = vec4(minTotal.x, minTotal.y, minTotal.z, 1.0);
        mAABB.coord[1] = vec4(minTotal.x, minTotal.y, maxTotal.z, 1.0);
        mAABB.coord[2] = vec4(minTotal.x, maxTotal.y, minTotal.z, 1.0);
        mAABB.coord[3] = vec4(minTotal.x, maxTotal.y, maxTotal.z, 1.0);
        mAABB.coord[4] = vec4(maxTotal.x, minTotal.y, minTotal.z, 1.0);
        mAABB.coord[5] = vec4(maxTotal.x, minTotal.y, maxTotal.z, 1.0);
        mAABB.coord[6] = vec4(maxTotal.x, maxTotal.y, minTotal.z, 1.0);
        mAABB.coord[7] = vec4(maxTotal.x, maxTotal.y, maxTotal.z, 1.0);

        imp.FreeScene();
    }

    void Model::pushInPipeline(mat4 const &transform)
    {
        bool isReallocate = false; // All buffers have the same size

        for(u32 i = 0; i < mMeshesAABB.size(); ++i)
        {
            global->Model.command->push(mMeshesCommand[i], isReallocate);
            global->Model.toWorldSpace->push(transform, isReallocate);
            global->Model.aabb3D->push(mMeshesAABB[i], isReallocate);
        }

        if(isReallocate)
        {
            global->Model.command->bindBase(SHADER_STORAGE, 0);
            global->Model.toWorldSpace->bindBase(SHADER_STORAGE, 2);
            global->Model.aabb3D->bindBase(SHADER_STORAGE, 3);

            // We reallocate toClipSpace as well
            global->Model.toClipSpace->allocate(global->Model.toWorldSpace->numMaxElements());
            global->Model.toClipSpace->bindBase(SHADER_STORAGE, 1);
        }
    }

    Model::~Model()
    {

    }
}
