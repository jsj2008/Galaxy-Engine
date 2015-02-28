/*!
 * \file node.cpp
 * \brief One node of the SceneManager
 * \author Antoine MORRIER
 * \version 1.0
 */

#include "node.h"
#include "../System/device.h"
#include "modelnode.h"
#include "pointlightnode.h"

using namespace glm;
using namespace std;

namespace GXY
{
    Node::Node(glm::mat4 const &parent) : mParent(nullptr), mGlobalMatrix(parent), mGlobalScaleFactor(1.0f)
    {
        mAABB.coord[0] = vec4(FLT_MAX, FLT_MAX, FLT_MAX, 1.0);
        mAABB.coord[1] = vec4(FLT_MAX, FLT_MAX, FLT_MIN, 1.0);
        mAABB.coord[2] = vec4(FLT_MAX, FLT_MIN, FLT_MAX, 1.0);
        mAABB.coord[3] = vec4(FLT_MAX, FLT_MIN, FLT_MIN, 1.0);
        mAABB.coord[4] = vec4(FLT_MIN, FLT_MAX, FLT_MAX, 1.0);
        mAABB.coord[5] = vec4(FLT_MIN, FLT_MAX, FLT_MIN, 1.0);
        mAABB.coord[6] = vec4(FLT_MIN, FLT_MIN, FLT_MAX, 1.0);
        mAABB.coord[7] = vec4(FLT_MIN, FLT_MIN, FLT_MIN, 1.0);
    }

    void Node::mActualizeBoundingBoxes()
    {
        vec3 newMin = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        vec3 newMax = vec3(FLT_MIN, FLT_MIN, FLT_MIN);

        for(auto model : mModels)
        {
            newMin = glm::min(newMin, model->AABB().coord[0].xyz());
            newMax = glm::max(newMax, model->AABB().coord[7].xyz());
        }

        for(auto child : mChildren)
        {
            newMin = glm::min(newMin, child->mAABB.coord[0].xyz());
            newMax = glm::max(newMax, child->mAABB.coord[7].xyz());
        }

        mAABB.coord[0] = vec4(newMin.x, newMin.y, newMin.z, 1.0);
        mAABB.coord[1] = vec4(newMin.x, newMin.y, newMax.z, 1.0);
        mAABB.coord[2] = vec4(newMin.x, newMax.y, newMin.z, 1.0);
        mAABB.coord[3] = vec4(newMin.x, newMax.y, newMax.z, 1.0);
        mAABB.coord[4] = vec4(newMax.x, newMin.y, newMin.z, 1.0);
        mAABB.coord[5] = vec4(newMax.x, newMin.y, newMax.z, 1.0);
        mAABB.coord[6] = vec4(newMax.x, newMax.y, newMin.z, 1.0);
        mAABB.coord[7] = vec4(newMax.x, newMax.y, newMax.z, 1.0);

        if(mParent != nullptr)
            mParent->mActualizeBoundingBoxes();
    }

    void Node::mBuildAABB()
    {
        for(auto child : mChildren)
            child->mBuildAABB();

        // Is a Leaf
        if(mChildren.empty())
            mActualizeBoundingBoxes();
    }

    void Node::mApplyMatrixToChildrenAndModels(const mat4 &matrix)
    {
        mGlobalMatrix *= matrix;

        for(auto model : mModels)
            model->updateAABB();

        for(auto pointLight : mPointLights)
            pointLight->updateMatrix();

        for(auto child : mChildren)
            child->mApplyMatrixToChildrenAndModels(matrix);
    }

    void Node::mApplyScaleFactorToChildren(float factor)
    {
        mGlobalScaleFactor *= factor;

        for(auto child : mChildren)
            child->mApplyScaleFactorToChildren(factor);
    }

    void Node::identity(void)
    {
        mApplyScaleFactorToChildren(mParent->mGlobalScaleFactor / mGlobalScaleFactor);

        mat4 matrix = inverse(mGlobalMatrix) * (mParent != nullptr ? mParent->mGlobalMatrix : mat4(1.0f));

        mApplyMatrixToChildrenAndModels(matrix);

        mBuildAABB();
    }

    void Node::rotate(float angle, const vec3 &axe)
    {
        mat4 matrix = glm::rotate(mat4(1.0f), angle, axe);

        mApplyMatrixToChildrenAndModels(matrix);

        mBuildAABB();
    }

    void Node::translate(const vec3 &vec)
    {
        mat4 matrix = glm::translate(mat4(1.0f), vec);

        mApplyMatrixToChildrenAndModels(matrix);

        mBuildAABB();
    }

    void Node::scale(float factor)
    {
        mat4 matrix = glm::scale(mat4(1.0f), vec3(factor));

        mGlobalScaleFactor *= factor;
        mApplyScaleFactorToChildren(factor);
        mApplyMatrixToChildrenAndModels(matrix);

        mBuildAABB();
    }

    shared_ptr<Node> addNode(shared_ptr<Node> const &parent)
    {
        shared_ptr<Node> newNode = make_shared<Node>(parent->mGlobalMatrix);

        newNode->mGlobalScaleFactor = parent->mGlobalScaleFactor;
        newNode->mParent = parent;

        parent->mChildren.push_back(newNode);

        return newNode;
    }

    shared_ptr<ModelNode> addModel(shared_ptr<Node> const &parent, std::string const &path)
    {
        std::shared_ptr<ModelNode> toPush = make_shared<ModelNode>(path, parent);

        parent->mModels.push_back(toPush);

        parent->mActualizeBoundingBoxes();

        return toPush;
    }

    shared_ptr<PointLightNode> addPointLight(const std::shared_ptr<Node> &parent)
    {
        std::shared_ptr<PointLightNode> toPush = make_shared<PointLightNode>(parent);

        parent->mPointLights.push_back(toPush);

        return toPush;
    }

    void Node::pushModelsInPipeline(Frustrum const &frustrum)
    {
        if(frustrum.boxInside(mAABB) == false)
            return;

        for(auto model : mModels)
            model->pushInPipeline(frustrum);

        for(auto child : mChildren)
            child->pushModelsInPipeline(frustrum);
    }

    void Node::pushPointLightsInPipeline(Frustrum const &frustrum)
    {
        for(auto pointLight : mPointLights)
            pointLight->pushInPipeline(frustrum);

        for(auto child : mChildren)
            child->pushPointLightsInPipeline(frustrum);
    }
}
