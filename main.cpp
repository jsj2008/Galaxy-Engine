#include "include/include.h"
#include "SceneManager/scenemanager.h"
#include "Debug/debug.h"

using namespace std;
using namespace glm;
using namespace GXY;

int main(int argc, char *argv[])
{
    try
    {
        Device device(800, 600); // Create a Device

        // Capture and hide cursor
        device.mouse()->captureCursor(true);
        device.mouse()->showCursor(false);

        SceneManager sceneManager; // Create a Scene Manager

        sceneManager.createCameraFPS(vec3(0, 500, 0.1), 300, 0.5); // Create a FPS Camera

        shared_ptr<Node> rootNode = sceneManager.getRootNode(); // Get the Root Node

        shared_ptr<ModelNode> model = addModel(rootNode, "models/OBJ/crytek-sponza/sponza.obj"); // Add a Model Sponza Atrium

        while(device.run())
        {
            device.begin(); // Clear Window

            sceneManager.render(); // Render Scene

            device.end(); // Swap Buffer
        }

    }

    catch(Except &exc)
    {
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
