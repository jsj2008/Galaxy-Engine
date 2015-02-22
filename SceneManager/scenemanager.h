/*!
 * \file scenemanager.h
 * \brief Manage the scene
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "node.h"
#include "../System/device.h"
#include "../System/shader.h"
#include "../System/buffer.h"
#include "../System/vertexarray.h"
#include "../Camera/camera.h"
#include "../System/framebuffer.h"

namespace  GXY
{
/**
     * @brief The SceneManager class
     *
     * Provide an interface to manage one scene with Dependance and others.
     */
    class SceneManager
    {
    public :
        /**
         * @brief SceneManager Constructor
         */
        SceneManager(void);

        /**
         * @brief Render all
         */
        void render(void);

        /**
         * @brief Initialize Rendering
         */
        void initialize(void);

        /**
         * @brief Render all Models
         */
        void renderModels(void);

        /**
         * @brief Render the Ambient Occlusion
         */
        void renderAmbientOcclusion(void);

        /**
         * @brief Render the Quad with Post Processing
         */
        void renderFinal(void);

        /**
         * @brief Create a CameraStatic
         * @param pos : Position of CameraStatic
         * @param look : Point looked by CameraStatic
         */
        void createCameraStatic(glm::vec3 const &pos, glm::vec3 const &look);

        /**
         * @brief Create a CameraFPS
         * @param pos : Position of CameraFPS
         * @param speed : Speed of CameraFPS in unit by seconds
         * @param sensitivity : Sensitivity of CameraFPS
         */
        void createCameraFPS(glm::vec3 const &pos, float speed, float sensitivity);

        /**
         * @brief Get the Root Node
         * @return A pointer on the Root Node
         */
        inline std::shared_ptr<Node> getRootNode(void) {return mRootNode;}

    private:
        std::shared_ptr<Node> mRootNode; //*< The Root Node
        std::shared_ptr<AbstractCamera> mCamera; //*< The Camera

        std::shared_ptr<FrameBuffer> mGeometryFrameBuffer; //*< The FrameBuffer used to render Geometry
        std::shared_ptr<Texture> mImageAmbientOcclusion; //*< AO, Horizontal Pass, Vertical Pass
    };
}

#endif // SCENEMANAGER_H
