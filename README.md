# THIS CODE IS OLD, SOME VERY BAD USE OF SYNCHRONIZATION OR ARB_BUFFER_STORAGE ARE IN. PLEASE, PREFER LOOK OpenGL TOols or Dark-Light

It's a Graphic Engine Created by Antoine MORRIER, student in Télécom SudParis.
This Engine will be able to render global illumination on real time using OpenGL 4.4+ currently.

Features

Sceneries

This Engine works with Nodes, so all members depend on one other member, for example, glasses in a storage cupboard depend of this storage cupboard.

Occlusion

Ambient Occlusion is one analytical ambient occlusion with Poisson Sampling. Ray marched analytical ambient occlusion is in developpment

Lighting

Direct Lighting is performed by computing the rendering equation and blending over color frame buffer

Shadows

Shadows are computed by Exponential Shadow Maps without kernel. Kernel is in development

Illumination

Global Illumination is performed by implemented Instant radiosity with virtual point light Reflective Shadow Maps is in developpment

EnvironmentMap

Reflections, Refractions, Caustics are in development
