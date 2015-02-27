/*!
 * \file device.h
 * \brief Create a Device with OpenGL Context
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef DEVICE_H
#define DEVICE_H

#include "../include/include.h"
#include "../Debug/debug.h"
#include "ressourcemanager.h"


namespace GXY
{
    class Device;
    class VertexArray;
    class Shader;
    class SceneManager;

    /**
     * @brief The Global struct
     *
     * This structure provide many Global variables to manage the Engine,
     * Shaders, Buffers and other important things
     */
    struct Global
    {
        Device *device; //!< A pointer on current Device
        std::shared_ptr<RessourceManager> ressourceManager; //!< A pointer on RessourceManager
        SceneManager *sceneManager; //!< A pointer on SceneManager

        struct
        {
            std::shared_ptr<Buffer<Context>> contextBuffer; //!< A pointer on Buffer which manage Context
            std::shared_ptr<Buffer<FrustrumUniform>> frustrumBuffer; //!< A pointer on Buffer which manage Frustrum
        }Uniform;

        struct
        {
            std::shared_ptr<VertexArray> vao; //!< A pointer on the VertexArray for normal rendering
            std::shared_ptr<VertexArray> vaoDepth; //!< A pointer on the VertexArray for depth rendering

            std::shared_ptr<Buffer<Vertex>> vertex; //!< A pointer on the principal Vertex Buffer for normal rendering
            std::shared_ptr<Buffer<glm::vec3>> vertexDepth; //!< A pointer on the Vertex Buffer for depth rendering

            std::shared_ptr<Buffer<u32>> index; //!< A pointer on the principal Element Buffer for rendering

            std::shared_ptr<Buffer<DrawElementCommand>> command; //!< A pointer on the DrawElementCommand Buffer for rendering
            std::shared_ptr<Buffer<AABB3D>> aabb3D; //!< A pointer on Buffer which own Bounding Boxes for culling

            std::shared_ptr<Buffer<Material>> material; //!< A pointer on the Material Buffer which own all materials

            std::shared_ptr<Buffer<glm::mat4>> toClipSpace; //!< A pointer on matrixClipSpace buffer
            std::shared_ptr<Buffer<glm::mat4>> toWorldSpace; //!< A pointer on matrixWorldSpace buffer
        }Model;

        struct
        {
            std::shared_ptr<VertexArray> vaoPointLight; //!< A pointer on the VertexArray for render "Quads Lights"

            std::shared_ptr<Buffer<glm::vec2>> quadsPointLights; //!< A pointer on the Buffer which contains Quads Lights
            std::shared_ptr<Buffer<DrawArrayCommand>> commandPointLights; //!< A Pointer on the Buffer which contains Commands for PointLight
            std::shared_ptr<Buffer<PointLight>> pointLight; //!< A Pointer on the PointLight Buffer

            std::shared_ptr<Buffer<glm::mat4>> toWorldSpace; //!< A pointer on matrixWorldSpace buffer

            std::shared_ptr<FrameBuffer> pointLightShadowMaps; //!< A Pointer on FrameBuffer which contains Cube_Map_Array
        }Lighting;

        struct
        {
            std::shared_ptr<VertexArray> vao; //!< A pointer on the VertexArray for Quad
            std::shared_ptr<Buffer<glm::vec2>> vertex; //!< A pointer on the Buffer which own vec2 for Quad
        }Quad;

        struct
        {
            std::shared_ptr<VertexArray> vao; //!< A pointer on the VertexArray for Cube
            std::shared_ptr<Buffer<glm::vec3>> vertex; //!< A pointer on the Buffer which own vec3 for Cube
            std::shared_ptr<Buffer<u32>> index; //!< A pointer on the Buffer which own indices for Cube
        }Cube;

        struct
        {
            std::shared_ptr<Shader> matrixCulling; //!< A pointer on the Shader used to compute Matrix ClipSpace and perform frustrum culling
            std::shared_ptr<Shader> depth; //!< A pointer on The Shader used to depth pass.
            std::shared_ptr<Shader> model; //!< A pointer on the Shader used to render Model

            std::shared_ptr<Shader> ambientOcclusion; //!< A pointer on the Shaser used to compute Ambient Occlusion
            std::shared_ptr<Shader> blurHorizontalPass; //!< A pointer on the Shader used for Gaussian blur : Horizontal pass
            std::shared_ptr<Shader> blurVerticalPass; //!< A pointer on the Shader used for Gaussian blur : Vertical pass

            std::shared_ptr<Shader> projectPointLights;
            std::shared_ptr<Shader> computePointLights;
            std::shared_ptr<Shader> depthPointLight;

            std::shared_ptr<Shader> final; //!< A pointer on the Shader used to render final Quad
        }Shaders;
    };

    /**
     * @brief A pointer on the Global structure
     */
    extern Global *global;

    /**
     * @brief Forgive one interface for KeyBoard
     */
    class KeyBoard
    {
        friend Device;

    public:
        /**
         * @brief KeyBoard constructor
         */
        KeyBoard(void){};

        /**
         * @brief Ask if one key is pressed
         * @param k : The key that you want know if it is pressed
         * @return true if it's pressed, else false
         */
        inline bool key(SDL_Keycode const &k) {return mKeys[k];}

    private:
        std::map<SDL_Keycode, bool> mKeys; //!< An associative table between KeyCode and bool
    };

    /**
     * @brief Forgive one interface for Mouse
     */
    class Mouse
    {
        friend Device;

    public:
        /**
         * @brief Mouse Constructor
         */
        Mouse(void){}

        /**
         * @brief Get position of Mouse : x
         * @return x of Mouse
         */
        inline s32 x(void) {return mX;}

        /**
         * @brief Get position of Mouse : y
         * @return y of Mouse
         */
        inline s32 y(void) {return mY;}

        /**
         * @brief Get the relative position of Mouse : x
         * @return x relative of Mouse
         */
        inline s32 xRel(void) {return mXRel;}

        /**
         * @brief Get the relative position of Mouse : y
         * @return y relative of Mouse
         */
        inline s32 yRel(void) {return mYRel;}

        /**
         * @brief Ask if one button is pressed
         * @param button : The button that you want know if it is pressed
         * @return true if it's pressed, else false
         */
        inline bool button(u32 button) {return mButtons[button];}

        /**
         * @brief Let to show or not the cursor of Mouse
         * @param show : Enable or disable the showing of Cursor
         */
        inline void showCursor(bool show) const {SDL_ShowCursor(show);}

        /**
         * @brief Let to capture in the center of Window the cursor
         * @param capture : capture or not
         */
        inline void captureCursor(bool capture) const{SDL_SetRelativeMouseMode(capture ? SDL_TRUE : SDL_FALSE);}

    private:
        s32 mX, mY, mXRel, mYRel; //!< Respectively position of Mouse, and relative Position of Mouse
        std::map<u32, bool> mButtons; //!< An associative table between Button of Mouse and bool
    };

    /**
     * @brief The Device class
     *
     * Provide a window with OpenGL Context, events
     * control of FPS, ...
     */

    class Device
    {
    public :
        /**
         * @brief Device Constructor
         * @param width : Width of Window
         * @param height : Height of Window
         * @param fullScreen : It's in full screen (true), or not (false)
         * @param title : Title of Window (Galaxy-Engine by Default)
         * @param clearColor : The color used to clear screen (black by default)
         * @param pathIcon : A path to one icon
         */
        Device(s32 width, s32 height,
               bool fullScreen = false,
               std::string const &title = "Galaxy-Engine",
               glm::vec3 const &clearColor = glm::vec3(0.0, 0.0, 0.0),
               std::string const &pathIcon = "");

        /**
         * @brief Clear buffers and update events
         */
        void begin(void);

        /**
         * @brief Swap buffers and control frame per second
         */
        void end(void);

        /**
         * @brief Change the clear Color
         * @param r : Value for red
         * @param g : Value for green
         * @param b : Value for blue
         */
        void setClearColor(float r, float g, float b);

        /**
         * @brief Change the clear Color
         * @param color : The new color
         */
        void setClearColor(glm::vec3 const &color);

        /**
         * @brief Change the title of Window
         * @param title : The new title
         */
        void setTitle(std::string const &title);

        /**
         * @brief Get width of Window
         * @return width of Window
         */
        inline s32 width(void) const {return mWidth;}

        /**
         * @brief Get height of Window
         * @return height of Window
         */
        inline s32 height(void) const {return mHeight;}

        /**
         * @brief Ask if the program should be run or no
         * @return true : you have to continue, else, you have to quit
         */
        inline bool run(void) const {return !mQuit;}

        /**
         * @brief Get the KeyBoard
         * @return A pointer on the KeyBoard
         */
        inline std::shared_ptr<KeyBoard> keyBoard(void) {return mKeyBoard;}

        /**
         * @brief get the Mouse
         * @return A pointer on the Mouse
         */
        inline std::shared_ptr<Mouse> mouse(void) {return mMouse;}

        /* @return : frame per second */
        inline u32 getFPS(void) const {return mFPS;}

        /**
         * @brief Let to enable the control of FPS
         * @param enable : Enable or Disable the control of FPS
         */
        inline void setControlFps(bool enable) {mControlFps = enable;}

        /**
         * @brief Clear only the DepthBuffer
         */
        inline void clearDepthBuffer(void) const {glClear(GL_DEPTH_BUFFER_BIT);}

        /**
         * @brief Clear only the StencilBuffer
         */
        inline void clearStencilBuffer(void) const {glClear(GL_STENCIL_BUFFER_BIT);}

        /**
         * @brief Clear only the ColorBuffer
         */
        inline void clearColorBuffer(void) const {glClear(GL_COLOR_BUFFER_BIT);}

        /**
         * @brief Clear the DepthBuffer and the ColorBuffer
         */
        inline void clearDepthColorBuffer(void) const {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}

        /**
         * @brief Set the View Port size, used for frame buffer
         */
        inline void setViewPort(void) const {glViewport(0, 0, mWidth, mHeight);}

        /**
          * @brief The device destructor
          */
        ~Device(void);

    private :
        std::shared_ptr<SDL_Window> mWindow; //!< The pointer of Window
        SDL_GLContext mContext; //!< The context of OpenGL

        s32 mWidth, mHeight; //!< Respectively Width and height of Window

        bool mQuit; //!< Should the program run or not?

        std::shared_ptr<KeyBoard> mKeyBoard; //!< A pointer on the KeyBoard
        std::shared_ptr<Mouse> mMouse; //!< A pointer on the Mouse

        // control FPS :
        u32 mNImageInOneSecond; //!< Number images in one second
        u32 mLastTimeUpdate; //!< The last time we get the tick
        u32 mFPS; //!< The number of FPS
        bool mControlFps; //!< Should the program control FPS or not
        u32 mLastTimeUpdateForControlFps;

        /**
         * @brief mUpdate
         */
        void mUpdate(void);
    };

    /**
     * @brief Get the first power of 2 bigger than number
     * @param number
     * @return the first power of 2 bigger than number
     */
    u32 powerOf2(u32 number);
}

#endif // DEVICE_H
