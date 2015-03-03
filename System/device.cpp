/*!
 * \file device.cpp
 * \brief Create a Device with OpenGL Context
 * \author Antoine MORRIER
 * \version 1.0
 */

#include "device.h"
#include "vertexarray.h"
#include "buffer.h"
#include "shader.h"
#include "framebuffer.h"

namespace GXY
{
    using namespace std;
    using namespace glm;

    Global *global;

    u32 powerOf2(u32 number)
    {
        if(number <= 4)
            return 4;

        else if(number <= 8)
            return 8;

        else if(number <= 16)
            return 16;

        else if(number <= 32)
            return 32;

        else if(number <= 64)
            return 64;

        else if(number <= 128)
            return 128;

        else if(number <= 256)
            return 256;

        else if(number <= 512)
            return 512;

        else if(number <= 1024)
            return 1024;

        else if(number <= 2048)
            return 2048;

        else if(number <= 4096)
            return 4096;

        else return 8192;
    }

    void createGlobalCube(void)
    {
        global->Cube.vao = make_shared<VertexArray>();
        global->Cube.index = make_shared<Buffer<u32>>();
        global->Cube.vertex = make_shared<Buffer<vec3>>();

        global->Cube.vao->create();

        global->Cube.index->allocate(36);
        global->Cube.vertex->allocate(36);

        memcpy(global->Cube.index->map(), ElementCubeAABB3D, sizeof ElementCubeAABB3D);
        memcpy(global->Cube.vertex->map(), CubeAABB3D, sizeof CubeAABB3D);
        global->Cube.vao->configure(*global->Cube.vertex);
        global->Cube.vao->bindElementBuffer(*global->Cube.index);
    }

    void createGlobalQuad(void)
    {
        global->Quad.vao = make_shared<VertexArray>();
        global->Quad.vertex = make_shared<Buffer<vec2>>();

        global->Quad.vao->create();

        global->Quad.vertex->allocate(4);

        memcpy(global->Quad.vertex->map(), QuadAABB2D, sizeof QuadAABB2D);
        global->Quad.vao->configure(*global->Quad.vertex);
    }

    void createGlobalModel(void)
    {
        global->Model.aabb3D = make_shared<Buffer<AABB3D>>();
        global->Model.command = make_shared<Buffer<DrawElementCommand>>();
        global->Model.index = make_shared<Buffer<u32>>();
        global->Model.material = make_shared<Buffer<Material>>();
        global->Model.toClipSpace = make_shared<Buffer<mat4>>();
        global->Model.toWorldSpace = make_shared<Buffer<mat4>>();
        global->Model.vertex = make_shared<Buffer<Vertex>>();
        global->Model.vertexDepth = make_shared<Buffer<vec3>>();

        global->Model.vao = make_shared<VertexArray>();
        global->Model.vaoDepth = make_shared<VertexArray>();
    }

    void createGlobalLighting(void)
    {
        global->Lighting.commandPointLights = make_shared<Buffer<DrawArrayCommand>>();
        global->Lighting.pointLight = make_shared<Buffer<PointLight>>();
        global->Lighting.quadsPointLights = make_shared<Buffer<vec2>>();
        global->Lighting.toWorldSpace = make_shared<Buffer<mat4>>();
        global->Lighting.vplCounter = make_shared<Buffer<u32>>();

        global->Lighting.vaoPointLight = make_shared<VertexArray>();

        global->Lighting.pointLightShadowMaps = make_shared<FrameBuffer>();
        global->Lighting.vplPointLightCreation = make_shared<FrameBuffer>();

        u32 size = powerOf2(std::max(global->device->width(), global->device->height())) / 2;
        global->Lighting.pointLightShadowMaps->create();
        global->Lighting.pointLightShadowMaps->createCubeMapArray(MAX_POINT_LIGHT_SHADOW, size, size, {R32F}, true);

        global->Lighting.vplCounter->allocate(1);
        global->Lighting.vplCounter->map()[0] = 0;
        global->Lighting.vplCounter->bindBase(ATOMIC, 0);
        glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);

        global->Lighting.vplPointLightCreation->create();
        global->Lighting.vplPointLightCreation->createCubeMap(16, 16, {R16F}, true);
    }

    void createGlobalShader(void)
    {
        global->Shaders.matrixCulling = make_shared<Shader>();
        global->Shaders.depth = make_shared<Shader>();
        global->Shaders.model = make_shared<Shader>("Shaders/model.vert", "Shaders/model.frag");

        global->Shaders.ambientOcclusion = make_shared<Shader>();
        global->Shaders.blurHorizontalPass = make_shared<Shader>();
        global->Shaders.blurVerticalPass = make_shared<Shader>();

        global->Shaders.projectPointLights = make_shared<Shader>();
        global->Shaders.computePointLights = make_shared<Shader>("Shaders/computepointlight.vert", "Shaders/computepointlight.frag");
        global->Shaders.depthPointLight = make_shared<Shader>("Shaders/pointlightdepth.vert", "Shaders/pointlightdepth.frag");

        global->Shaders.final = make_shared<Shader>("Shaders/final.vert", "Shaders/final.frag");

        global->Shaders.matrixCulling->compileFile("Shaders/matrixculling.glsl", COMPUTE);
        global->Shaders.depth->compileFile("Shaders/depth.vert", VERTEX);

        global->Shaders.ambientOcclusion->compileFile("Shaders/ambientocclusion.glsl", COMPUTE);
        global->Shaders.blurHorizontalPass->compileFile("Shaders/blurH.glsl", COMPUTE);
        global->Shaders.blurVerticalPass->compileFile("Shaders/blurV.glsl", COMPUTE);

        global->Shaders.projectPointLights->compileFile("Shaders/projectpointlight.glsl", COMPUTE);

        global->Shaders.matrixCulling->link();
        global->Shaders.depth->link();

        global->Shaders.ambientOcclusion->link();
        global->Shaders.blurHorizontalPass->link();
        global->Shaders.blurVerticalPass->link();

        global->Shaders.projectPointLights->link();
    }

    void createGlobalUniform(void)
    {
        default_random_engine gen;
        uniform_real_distribution<float> z(0.0, 1.0);
        uniform_real_distribution<float> xy(-1.0, 1.0);

        global->Uniform.contextBuffer = make_shared<Buffer<Context>>();
        global->Uniform.frustrumBuffer = make_shared<Buffer<FrustrumUniform>>();
        global->Uniform.randomNormal = make_shared<Buffer<vec4>>();

        global->Uniform.contextBuffer->allocate(1);
        global->Uniform.frustrumBuffer->allocate(1);
        global->Uniform.randomNormal->allocate(16);

        global->Uniform.contextBuffer->bindBase(UNIFORM, 0);
        global->Uniform.frustrumBuffer->bindBase(UNIFORM, 1);
        global->Uniform.randomNormal->bindBase(UNIFORM, 2);

        for(u32 i = 0; i < 16; ++i)
            global->Uniform.randomNormal->map()[i] = vec4(normalize(vec3(xy(gen), xy(gen), z(gen))), 0.0);
    }

    void createGlobal(Device *device)
    {
        global->device = device;
        global->ressourceManager = make_shared<RessourceManager>();

        createGlobalQuad();
        createGlobalCube();
        createGlobalModel();
        createGlobalLighting();
        createGlobalShader();
        createGlobalUniform();
    }

    Device::Device(int width, int height, bool fullScreen,
                   string const &title, vec3 const &clearColor,
                   string const &pathIcon) :
        mNImageInOneSecond(0), mLastTimeUpdate(0), mFPS(0), mControlFps(false)
    {
        // Initialisation SDL
        if(SDL_Init(SDL_INIT_VIDEO) == -1)
            throw Except(string("SDL2 Init Error : ") + SDL_GetError());

        atexit(SDL_Quit);

        // Screen
        u32 flags = fullScreen ? SDL_WINDOW_FULLSCREEN : 0;
        flags |= SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

        mWidth = width;
        mHeight = height;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        mWindow = shared_ptr<SDL_Window>(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED, width, height,
                                         flags),
                                         SDL_DestroyWindow);

        if(mWindow == nullptr)
            throw Except(string("Error Create Window : ") + SDL_GetError());


        // Création du context OpenGL
        mContext = SDL_GL_CreateContext(mWindow.get());

        if(mContext == nullptr)
        {
            SDL_DestroyWindow(mWindow.get());
            throw Except(string("Error Create GL_Context : ") + SDL_GetError());
        }

        // GLEW
        glewExperimental = true;
        u32 codeGlew = glewInit();
        if(codeGlew != GLEW_OK)
        {
            SDL_GL_DeleteContext(mContext);
            SDL_DestroyWindow(mWindow.get());
            throw Except(string("Glew Init Error : ") + (char const*)glewGetErrorString(codeGlew));
        }

        mQuit = false;
        mMouse = make_shared<Mouse>();
        mKeyBoard = make_shared<KeyBoard>();

        if(pathIcon != "")
        {
            SDL_Surface *icon = IMG_Load(pathIcon.c_str());

            if(icon == nullptr)
                cerr << "Error to load : " << " " << pathIcon << endl;

            else
            {
                SDL_SetWindowIcon(mWindow.get(), icon);
                SDL_FreeSurface(icon);
            }
        }

        setClearColor(clearColor);

        mLastTimeUpdateForControlFps = SDL_GetTicks();

        glEnable(GL_DEPTH_TEST);

        glGetError(); // flush error

        global = new Global;

        createGlobal(this);
    }

    void Device::mUpdate(void)
    {
        SDL_Event event;

        mMouse->mXRel = mMouse->mYRel = 0;

        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                // Clavier
                case SDL_KEYDOWN :
                    mKeyBoard->mKeys[event.key.keysym.sym] = true;
                break;

                case SDL_KEYUP :
                    mKeyBoard->mKeys[event.key.keysym.sym] = false;
                break;

                // Souris
                case SDL_MOUSEMOTION :
                    mMouse->mX = event.motion.x;
                    mMouse->mY = mHeight - event.motion.y - 1;
                    mMouse->mXRel = event.motion.xrel;
                    mMouse->mYRel = event.motion.yrel;
                break;

                case SDL_MOUSEBUTTONDOWN :
                    mMouse->mButtons[event.button.button] = true;
                break;

                case SDL_MOUSEBUTTONUP :
                    mMouse->mButtons[event.button.button] = false;
                break;

                // quitter
                case SDL_WINDOWEVENT :
                    switch(event.window.event)
                    {
                        case SDL_WINDOWEVENT_CLOSE :
                            mQuit = true;
                        break;

                        case SDL_WINDOWEVENT_RESIZED:
                            mWidth = event.window.data1;
                            mHeight = event.window.data2;

                            setViewPort();
                        break;

                        default : break;
                    }
                break;

                default : break;
            }
        }

        // Alt F4 = quitter
        if(mKeyBoard->key(SDLK_LALT) && mKeyBoard->key(SDLK_F4))
            mQuit = true;
    }

    void Device::begin(void)
    {
        mUpdate();
        clearDepthColorBuffer();
    }

    void Device::end(void)
    {
        ++mNImageInOneSecond;

        // pour récupérer le nombre de FPS réel
        if(SDL_GetTicks() - mLastTimeUpdate >= 1000)
        {
            mFPS = mNImageInOneSecond;
            mNImageInOneSecond = 0;
            mLastTimeUpdate = SDL_GetTicks();
        }

        if(mControlFps == true)
        {
            // Pour récupérer le temps à attendre si control de fps actif
            u32 delay = SDL_GetTicks() - mLastTimeUpdateForControlFps;
            if(delay < 20)
                SDL_Delay(20 - delay);
            mLastTimeUpdateForControlFps = SDL_GetTicks();
        }

        synchronize();
        SDL_GL_SwapWindow(mWindow.get());
    }

    void Device::setClearColor(vec3 const &color)
    {
        glClearColor(color.r, color.g, color.b, 1.0f);
    }

    void Device::setClearColor(float r, float g, float b)
    {
        glClearColor(r, g, b, 1.0f);
    }

    void Device::setTitle(string const &title)
    {
        SDL_SetWindowTitle(mWindow.get(), title.c_str());
    }

    Device::~Device(void)
    {
        delete global;
        SDL_GL_DeleteContext(mContext);
    }
}
