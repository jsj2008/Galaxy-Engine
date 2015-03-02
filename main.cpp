#include "include/include.h"
#include "SceneManager/scenemanager.h"
#include "Debug/debug.h"
#include "SceneManager/modelnode.h"
#include "SceneManager/pointlightnode.h"

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

        sceneManager.createCameraFPS(vec3(0.1, 500.0, .0), 300, 0.5); // Create a FPS Camera

        shared_ptr<Node> rootNode = sceneManager.getRootNode(); // Get the Root Node

        shared_ptr<PointLightNode> light1 = addPointLight(rootNode);
        shared_ptr<PointLightNode> light2 = addPointLight(rootNode);

        shared_ptr<ModelNode> model = addModel(rootNode, "models/OBJ/crytek-sponza/sponza.obj"); // Add a Model Sponza Atrium

        light1->setColor(vec3(1.0, 1.0, 1.0));
        light1->setPosition(vec3(0.0, 500.0, .0));
        light1->setRadius(1000);
        light1->setIntensity(1.0);
        light1->enableShadowMaps(1);

        light2->setColor(vec3(.0, 0.0, 1.0));
        light2->setPosition(vec3(500.0, 200.0, 500.0));
        light2->setRadius(1000.0);
        light2->setIntensity(1.0);
        light2->enableShadowMaps(0);

        while(device.run())
        {
            device.begin(); // Clear Window

            u64 t = 0;
            u32 query;

            glGenQueries(1, &query);
            glBeginQuery(GL_TIME_ELAPSED, query);
            sceneManager.render(); // Render Scene
            glEndQuery(GL_TIME_ELAPSED);

            while(t == 0)
                glGetQueryObjectui64v(query, GL_QUERY_RESULT_AVAILABLE, &t);

            glGetQueryObjectui64v(query, GL_QUERY_RESULT, &t);

            cout << (float)t / 1000000 << std::endl;

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
