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
        Device device(800, 600);

        device.mouse()->captureCursor(true);
        device.mouse()->showCursor(false);

        SceneManager sceneManager;

        sceneManager.createCameraFPS(vec3(0, 500, 0.1), 300, 0.5);

        shared_ptr<Node> rootNode = sceneManager.getRootNode();

        shared_ptr<ModelNode> model = addModel(rootNode, "models/OBJ/crytek-sponza/sponza.obj");

        while(device.run())
        {
            device.begin();

            sceneManager.render();

            device.end();
        }

    }

    catch(Except &exc)
    {
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
