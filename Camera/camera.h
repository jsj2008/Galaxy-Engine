/*!
 * \file camera.h
 * \brief Manage cameras
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "../include/include.h"
#include "../include/constant.h"
#include "../System/device.h"

namespace GXY
{
    /**
     * @brief The Frustrum class
     *
     * Describe a Frustrum, and provide tools to know if this Object is in or outside of this Frustrum
     */
    class Frustrum
    {
    public:
        /**
         * @brief Frustrum Constructor
         */
        Frustrum(void);

        bool boxInside(AABB3D const &box) const;
        bool sphereInside(Sphere const &sphere) const;

        /**
         * @brief Extract Planes from a "Perspective View Matrix"
         * @param angle : Focale y
         * @param ratio : Width / Height of this same Frustrum
         * @param nearD : Near plane
         * @param farD : Far plane
         * @param p : position
         * @param l : look
         * @param up : up vector
         */
        void extractPlane(float angle, float ratio, float nearD, float farD,
                          glm::vec3 const &p, glm::vec3 const &l, glm::vec3 const &up);

    public :
        Plane mPlanes[6]; //!< TOP, BOTTOM, RIGHT, LEFT, NEAR, PLANE
};

    /**
     * @brief The AbstractCamera class
     *
     * Provide a virtual class Camera to implement naturally other class
     */
    class AbstractCamera
    {
    public :
        /**
         * @brief AbstractCamera Constructor
         * @param pos : Position of Camera
         * @param look : Point looked by Camera
         * @param up : Up Vector
         * @param angle : Focale y
         * @param ratio : Width / Height of Frustrum
         * @param near : Near plane
         * @param far : Far plane
         */
        AbstractCamera(glm::vec3 const &pos, glm::vec3 const &look, CameraUp up,
                       float angle, float ratio, float near, float far);

        AbstractCamera(void){}

        /**
         * @brief Get Projection * View Matrix
         * @return Perspective * View Matrix
         */
        inline glm::mat4 const &toClipSpace(void) {return mToClipSpace;}

        /**
         * @brief Get Position of Camera
         * @return Position
         */
        inline glm::vec4 const &position(void) {return mPos;}

        /**
         * @brief Get point looked by Camera
         * @return Look
         */
        inline glm::vec3 const &look(void) {return mLook;}

        /**
         * @brief Get a Up vector
         * @return up
         */
        inline glm::vec3 const &up(void) const {return mUp;}

        /**
         * @brief get a Frustrum
         * @return
         */
        inline Frustrum const &frustrum(void) const {return mFrustrum;}

        /**
         * @brief Can Update movement for Camera
         */
        virtual void update(void) = 0;

        /**
         * @brief ~AbstractCamera
         */
        virtual ~AbstractCamera(void) {}

    protected :
        glm::mat4 mToClipSpace; //!< Matrix
        glm::vec4 mPos;
        glm::vec3 mLook, mUp; //!< lookAt
        CameraUp mUpType; //!< Up Type
        float mAngle, mRatio, mNear, mFar; //!< Data for Perspective
        Frustrum mFrustrum; //!< The Frustrum

        /**
         * @brief mComputeToClipSpace
         */
        void mComputeToClipSpace(void);
        void mGetUp(CameraUp up);
    };

    /**
     * @brief A Static Camera
     */
    class CameraStatic : public AbstractCamera
    {
    public:
        /**
         * @brief CameraStatic Constructor
         * @param pos : Position
         * @param look : Point Looked by camera
         * @param up : Up Vector
         * @param angle : Focale y
         * @param ratio : Width / Height of Frustrum
         * @param near : Near plane
         * @param far : Far plane
         */
        CameraStatic(glm::vec3 const &pos, glm::vec3 const &look, CameraUp up,
                     float angle, float ratio, float near, float far) :
            AbstractCamera(pos, look, up, angle, ratio, near, far){}

        CameraStatic(void){}

        void set(glm::vec3 const &pos, glm::vec3 const &look, CameraUp up,
                 float angle, float ratio, float near, float far){
            *this = CameraStatic(pos, look, up, angle, ratio, near, far);
        }

        /* useless update because it's static */
        inline virtual void update(void) {}
    };

    /**
     * @brief A FreeFly Camera like FPS Games
     */
    class CameraFPS : public AbstractCamera
    {
    public:
        /**
         * @brief CameraFPS
         * @param pos : Position
         * @param up : Up Vector
         * @param speed : Speed for movements
         * @param sensitivity : Speed for looking (mouse)
         * @param angle : Focale y
         * @param ratio : Width / Height of Frustrum
         * @param near : Near plane
         * @param far : Far plane
         */
        CameraFPS(glm::vec3 const &pos, CameraUp up,
                  float speed, float sensitivity,
                  float angle, float ratio, float near, float far);

        /**
         * @brief Set the Key Mapping configuration
         * @param conf : New configuration
         */
        void setKeyMap(std::map<CameraKeyMap, SDL_Keycode> const &conf);

        /**
         * @brief Change the speed of Camera
         * @param speed in unit / seconds
         */
        inline void setSpeed(float speed) {mSpeed = speed;};

        /**
         * @brief Change the sensitivity of Camera for Mouse
         * @param sensitivity
         */
        inline void setSensitivity(float sensitivity) {mSensitivity = sensitivity;}

        /**
         * @brief Update movements
         */
        virtual void update(void);

    protected:
        std::map<CameraKeyMap, SDL_Keycode> mKeyMap; //!< KeyMapping

        glm::vec3 mForward, mLeft; //!< vector forward and left for deplacement

        float mPhi, mTheta; //!< Angle for spherical coordinate

        float mSpeed, mSensitivity; //!< Variable for speed

        /**
         * @brief Compute the forward vector
         */
        void mComputeForwardVector(void);
    };
}

#endif // CAMERA_H
