# CrimsonEdge

**CrimsonEdge** is game engine that currently focuses mostly on rending and physics simulation.

All implementations are written by a single developer, so development is quite slow but steadily ongoing. So bear with 
me if the project is not state of the art. 


# Components

The basic design of the engine consists of a core module (**ceCore** located at *Engine/ceCore*). The core modules
consists of

- *EntityGraph* for logical entities (game logic lives here)
- *SceneGraph* holding all rendered elements (generated on-the-fly by the *EntityGraph*)
- *AssetManagement* standard way for loading data from the filesystem and managing reusable elements
- *Graphics* mostly interfaces that must be implemented by a rendering module (**csOpenGL**)
- *Physics* mostly interfaces that must be implemented by a physics module (**ceBullet**)

At the moment there is only one rendering implementations in OpenGLv4 (**csOpenGL** located at *Engine/csOpenGL*) and
one physics implementation using BulletPhysics (**ceBullet** located at *Engine/ceBullet*). But the engine is designed to
include any other API (e.g. D3D, Vulkan, PhysX, ...).

The entire engine is written the C++ and GLSL for OpenGL-Shading. The build process is managed by CMake.
The engine comes with a MOC (MetaObjectCompiler) that is used to generate a basic form of reflection on top of C++-Classes.
The MOC (**csMOC**) generates C++-Code holding classes for wrapping the *real* C++ classes.
This code generation process is integrated within the CMake build process and is tested to be functional with

- Build from CMD
- VisualStudio
- CLion
- QtCreator

## Installation

Because the engine is still in early development there is no *real* installation. Everything must be compiled.

## Compilation

**INFORMATION** All development is done on windows. So it is very doubtful that this engine will compile on Linux or
Mac. But that is a future topic.

The build process is made up via CMake. In order to be able to compile the engine, the following external libraries
must be within the build path

- **SDL2** for window api
- **OpenGL** for graphics rendering
- **GLEW** for OpenGL extensions
- **PNG**, **JPEG** for texture loading
- **Bullet** for physics simulation
- **Assimp** for loading assets like FBX

When compiled without any special parameters this will generate the following executable files

- \<out>/bin/ceAssimpLoader.dll
- \<out>/bin/ceBullet.dll
- \<out>/bin/ceCore.dll
- \<out>/bin/ceImgLoader.dll
- \<out>/bin/ceLauncher.exe
- \<out>/bin/csMOC.exe
- \<out>/bin/csOpenGL.dll

## Launching the engine

There is no scene loading at the moment so the test scene is embedded in code within the **ceLauncher**.
To start the engine call the **ceLauncher** module either from within the IDE or from command line with the
start parameter

    --data <path_to_engine_source_code>/data

The engines launcher window should open on your main screen and shows a test scene with 10.000 sphere and a terrain.
You can navigate with mouse and standard *WASD* control. Exit the application with *Escape*

## Features

Bear in mind that this project is a one-man-show, so the features are quite limited at the moment.

The rendering engine uses two different pipelines

- ForwardRendering
- DeferredRendering

In both rendering pipelines shadow mapping for directional lights and for point lights are supported.

In the forward pipeline only up to four lights per objects are possible and only up to four lights (for the entire
scene) are capable of casting shadows. Each light can be set to cast shadows but during frame generation only the most
significant four are selected for really casting shadows.

In the deferred pipeline there are no such limitations. There is no technical limit for the number contributing lights.
This includes shadow casting lights. Each light can cast shadow and each shadow casting light will really cast shadows.
But this has a serious impact on the rendering performance.

## Planned features

Ideas on how implementation is integrated into the engine exists for the following features

- csPostProcessing
  - Bloom
  - ScreenSpaceAmbientOcclusion
  - ScreenSpaceReflection
  - Vignette 
- Audio processing
- More physics (character controller, trigger, ropes/chains, ...)
- csShaderGraph as an alternative to GLSL
- SceneLoading
- Editor - a complete editor for creating scenes and setup for a game
