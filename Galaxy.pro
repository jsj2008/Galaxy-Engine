TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++14

QMAKE_CXXFLAGS += -std=c++1y
QMAKE_CXXFLAGS_RELEASE += -O3

LIBS += -lSDL2
LIBS += -lSDL2_image
LIBS += -lGLEW
LIBS += -lGL
LIBS += -lassimp

SOURCES += main.cpp \
    System/device.cpp \
    System/framebuffer.cpp \
    System/shader.cpp \
    System/texture.cpp \
    System/vertexarray.cpp \
    System/ressourcemanager.cpp \
    Camera/camera.cpp \
    SceneManager/scenemanager.cpp \
    SceneManager/node.cpp \
    System/model.cpp \
    SceneManager/modelnode.cpp \
    SceneManager/pointlightnode.cpp

HEADERS += \
    System/buffer.h \
    System/device.h \
    System/framebuffer.h \
    System/shader.h \
    System/texture.h \
    System/vertexarray.h \
    include/constant.h \
    include/include.h \
    Debug/debug.h \
    System/ressourcemanager.h \
    Camera/camera.h \
    SceneManager/scenemanager.h \
    SceneManager/node.h \
    System/model.h \
    SceneManager/modelnode.h \
    SceneManager/pointlightnode.h

DISTFILES += \
    Shaders/final.frag \
    Shaders/final.vert \
    Shaders/model.vert \
    Shaders/model.frag \
    Shaders/matrixculling.glsl \
    Shaders/depth.vert \
    Shaders/ambientocclusion.glsl \
    Shaders/blurV.glsl \
    Shaders/blurH.glsl \
    Shaders/projectpointlight.glsl \
    Shaders/computepointlight.vert \
    Shaders/computepointlight.frag \
    Shaders/pointlightdepth.vert \
    Shaders/pointlightdepth.frag \
    Shaders/createvplpoint.vert \
    Shaders/createvplpoint.frag \
    Shaders/injectindirect.frag

