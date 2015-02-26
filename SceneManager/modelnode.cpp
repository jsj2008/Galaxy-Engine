#include "modelnode.h"

#include "../System/device.h"

using namespace glm;
using namespace std;

namespace GXY
{
    ModelNode::ModelNode(std::string const &path, std::shared_ptr<Node> const &parent) :
        mParent(parent), mModel(global->ressourceManager->getModel(path)), mMatrix(mat4(1.0f))
    {
        mAABB = computeAABB3D(mModel->AABB(), mParent->mGlobalMatrix);
    }

    void ModelNode::identity()
    {
        mMatrix = mat4(1.0f);
        mAABB = computeAABB3D(mModel->AABB(), mParent->mGlobalMatrix);
        mParent->mActualizeBoundingBoxes();
    }

    void ModelNode::rotate(float angle, const vec3 &axe)
    {
        mMatrix = glm::rotate(mMatrix, angle, axe);

        mAABB = computeAABB3D(mModel->AABB(), mParent->mGlobalMatrix * mMatrix);
        mParent->mActualizeBoundingBoxes();
    }

    void ModelNode::translate(const vec3 &vec)
    {
        mMatrix = glm::translate(mMatrix, vec);

        mAABB = computeAABB3D(mModel->AABB(), mParent->mGlobalMatrix * mMatrix);
        mParent->mActualizeBoundingBoxes();
    }

    void ModelNode::scale(float factor)
    {
        mMatrix = glm::scale(mMatrix, vec3(factor));

        mAABB = computeAABB3D(mModel->AABB(), mParent->mGlobalMatrix * mMatrix);
        mParent->mActualizeBoundingBoxes();
    }

    void ModelNode::pushInPipeline()
    {
        mModel->pushInPipeline(mParent->mGlobalMatrix * mMatrix);
    }
}
