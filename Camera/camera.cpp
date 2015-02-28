/*!
 * \file camera.cpp
 * \brief Manage cameras
 * \author Antoine MORRIER
 * \version 1.0
 */

#include "camera.h"

using namespace glm;

namespace GXY
{
    Frustrum::Frustrum(void)
    {

    }

    void Frustrum::extractPlane(float fov, float ratio, float nearD, float farD, const vec3 &p, const vec3 &l, const vec3 &up)
    {
        // tan(fov / 2)
        float tang = (float)tan(fov / 2.0f);
        float nH = tang * nearD;
        float fH = tang * farD;

        float nW = nH * ratio;
        float fW = fH * ratio;

        // Axis of cam
        vec3 z = normalize(p - l);
        vec3 x = normalize(cross(up, z));
        vec3 y = cross(z, x);

        // compute the centers of the near and far planes
        vec3 nc = p - z * nearD;
        vec3 fc = p - z * farD;

        // compute the 4 corners of the frustum on the near plane
        vec3 ntl = nc + y * nH  - x * nW;
        vec3 ntr = nc + y * nH  + x * nW;
        vec3 nbl = nc - y * nH  - x * nW;
        vec3 nbr = nc - y * nH  + x * nW;

            // compute the 4 corners of the frustum on the far plane
        vec3 ftl = fc + y * fH  - x * fW;
        vec3 ftr = fc + y * fH  + x * fW;
        vec3 fbl = fc - y * fH  - x * fW;
        vec3 fbr = fc - y * fH  + x * fW;

        setPlane(mPlanes[0], ntr,ntl,ftl);
        setPlane(mPlanes[1], nbl,nbr,fbr);
        setPlane(mPlanes[2], ntl,nbl,fbl);
        setPlane(mPlanes[3], nbr,ntr,fbr);
        setPlane(mPlanes[4], ntl,ntr,nbr);
        setPlane(mPlanes[5], ftr,ftl,fbl);
    }

    bool Frustrum::boxInside(const AABB3D &box) const
    {
        for(u32 i = 0; i < 6; ++i)
        {
            bool in = false;

            for(u32 j = 0; j < 8 && !in; ++j)
            {
                if(distancePlane(mPlanes[i], box.coord[j].xyz()) > 0.0)
                {
                    in = true;
                    break;
                }
            }

            if(!in)
                return false;
        }
        return true;
    }

    bool Frustrum::sphereInside(const Sphere &sphere) const
    {
        for(u32 i = 0; i < 6; ++i)
            if(distancePlane(mPlanes[i], sphere.position) < -sphere.radius)
                return false;

        return true;
    }

    AbstractCamera::AbstractCamera(vec3 const &pos, vec3 const &look, CameraUp up, float angle, float ratio, float near, float far) :
        mPos(pos, far), mLook(look), mUpType(up), mAngle(angle), mRatio(ratio), mNear(near), mFar(far)
    {
        mGetUp(up);
        mComputeToClipSpace();
    }

    void AbstractCamera::mGetUp(CameraUp up)
    {
        mUpType = up;

        if(mUpType == CameraUp::CAM_UP_X)
            mUp = vec3(1.0f, 0.0f, 0.0f);

        else if(mUpType == CameraUp::CAM_UP_Y)
            mUp = vec3(0.0f, 1.0f, 0.0f);

        else if(mUpType == CameraUp::CAM_UP_Z)
            mUp = vec3(0.0f, 0.0f, 1.0f);

        else if(mUpType == CameraUp::CAM_DOWN_X)
            mUp = vec3(-1.0, 0.0, 0.0);

        else if(mUpType == CameraUp::CAM_DOWN_Y)
            mUp = vec3(0.0, -1.0, 0.0);

        else if(mUpType == CameraUp::CAM_DOWN_Z)
            mUp = vec3(0.0, 0.0, -1.0);
    }

    void AbstractCamera::mComputeToClipSpace(void)
    {
        mToClipSpace = perspective(mAngle, mRatio, mNear, mFar) * lookAt(mPos.xyz(), mLook, mUp);
        mFrustrum.extractPlane(mAngle, mRatio, mNear, mFar, mPos.xyz(), mLook, mUp);
    }

    CameraFPS::CameraFPS(vec3 const &pos, CameraUp up,
                         float speed, float sensitivity, float angle, float ratio, float near, float far) :
        AbstractCamera(pos, vec3(0.0f, 0.0f, 0.0f), up, angle, ratio, near, far), mPhi(0.0), mTheta(0.0),
        mSpeed(speed), mSensitivity(sensitivity)
    {
        mKeyMap[CameraKeyMap::CAM_FORWARD] = SDLK_z;
        mKeyMap[CameraKeyMap::CAM_BACKWARD] = SDLK_s;
        mKeyMap[CameraKeyMap::CAM_LEFT] = SDLK_q;
        mKeyMap[CameraKeyMap::CAM_RIGHT] = SDLK_d;
    }

    void CameraFPS::mComputeForwardVector(void)
    {
        float phiRad = mPhi * M_PI / 180.0f;
        float thetaRad = mTheta * M_PI / 180.0f;

        float cPhi = cos(phiRad);
        float cTheta = cos(thetaRad);
        float sPhi = sin(phiRad);
        float sTheta = sin(thetaRad);

        if(mUpType == CameraUp::CAM_UP_X)
        {
            mForward.x = sPhi;
            mForward.y = cPhi * cTheta;
            mForward.z = cPhi * sTheta;
        }

        else if(mUpType == CameraUp::CAM_UP_Y)
        {
            mForward.y = sPhi;
            mForward.z = cPhi * cTheta;
            mForward.x = cPhi * sTheta;
        }

        else
        {
            mForward.z = sPhi;
            mForward.x = cPhi * cTheta;
            mForward.y = cPhi * sTheta;
        }
    }

    void CameraFPS::update(void)
    {
        // Mouse
        mPhi   -= global->device->mouse()->yRel() * mSensitivity;
        mTheta -= global->device->mouse()->xRel() * mSensitivity;

        if(mPhi > 89.0f)
            mPhi = 89.0f;

        else if(mPhi < -89.0f)
            mPhi = -89.0f;

        if(mTheta > 360.0f)
            mTheta = 0.0f;

        else if(mTheta < -360.0f)
            mTheta = 0.0f;

        mComputeForwardVector();

        // KeyBoard

        float speed = mSpeed / (float) global->device->getFPS();

        if(global->device->keyBoard()->key(mKeyMap[CameraKeyMap::CAM_FORWARD]) == true)
            mPos += vec4(mForward * speed, 0.0);

        if(global->device->keyBoard()->key(mKeyMap[CameraKeyMap::CAM_BACKWARD]) == true)
            mPos -= vec4(mForward * speed, 0.0);

        if(global->device->keyBoard()->key(mKeyMap[CameraKeyMap::CAM_LEFT]) == true)
            mPos += vec4(mLeft * speed, 0.0);

        if(global->device->keyBoard()->key(mKeyMap[CameraKeyMap::CAM_RIGHT]) == true)
            mPos -= vec4(mLeft * speed, 0.0);

        mLeft = cross(mUp, mForward);

        mLook = mPos.xyz() + mForward;

        mComputeToClipSpace();
    }
}
