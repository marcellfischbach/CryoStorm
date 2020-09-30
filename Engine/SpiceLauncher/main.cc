
#include <SpiceLauncher/window/sdlinputsystem.hh>
#include <SpiceLauncher/window/sdlkeyboard.hh>
#include <SpiceLauncher/window/sdlmouse.hh>
#include <SpiceLauncher/launchermodule.hh>
#include <SpiceCore/coremodule.hh>
#include <SpiceCore/input/input.hh>
#include <SpiceCore/objectregistry.hh>
#include <SpiceCore/graphics/igraphics.hh>
#include <SpiceCore/graphics/shading/ishaderloader.hh>
#include <SpiceCore/resource/vfs.hh>

#include <SpiceOpenGL/openglmodule.hh>

#include <iostream>
#include <SDL.h>

Spice::SDLKeyboard keyboard;
Spice::SDLMouse mouse;


void UpdateEvents()
{
  keyboard.Update();
  mouse.Update();
  SDL_Event evt;
  while (SDL_PollEvent(&evt))
  {
    switch (evt.type)
    {
    case SDL_KEYDOWN:
      keyboard.Update(evt.key.keysym.scancode, true);
      break;
    case SDL_KEYUP:
      keyboard.Update(evt.key.keysym.scancode, false);
      break;
    case SDL_MOUSEBUTTONDOWN:
      mouse.Update(evt.button.button, true);
      break;
    case SDL_MOUSEBUTTONUP:
      mouse.Update(evt.button.button, false);
      break;
    case SDL_MOUSEWHEEL:
      mouse.Update(evt.wheel.y, evt.wheel.x);
      break;
    case SDL_MOUSEMOTION:
      mouse.Update(evt.motion.x, evt.motion.y, evt.motion.xrel, evt.motion.yrel);
      break;

    }
  }

}

int main(int argc, char** argv)
{
  if (!Spice::LauncherModule::Register(argc, argv))
  {
    printf("Unable to register launcher\n");
    return -1;
  }
  if (!Spice::CoreModule::Register(argc, argv))
  {
    printf("Unable to register core\n");
    return -1;
  }
  if (!Spice::OpenGLModule::Register(argc, argv))
  {
    printf("Unable to register opengl\n");
    return -1;
  }

  Spice::VFS::Get()->SetBasePath("D:\\DEV\\SpiceEngine\\data");

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window* wnd = SDL_CreateWindow("Spice", 25, 25, 1024, 768, SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(wnd);
  SDL_ShowWindow(wnd);
  SDL_GL_MakeCurrent(wnd, context);



  if (!Spice::CoreModule::Initialize(argc, argv))
  {
    printf("Unable to initialize core\n");
    return -1;
  }
  if (!Spice::OpenGLModule::Initialize(argc, argv))
  {
    printf("Unable to initialize opengl\n");
    return -1;
  }

  Spice::iShaderLoader* shaderLoader = Spice::ObjectRegistry::Get<Spice::iShaderLoader>();
  shaderLoader->Load(Spice::ResourceLocator("testprogram.xml"));

  Spice::iGraphics* graphics = Spice::ObjectRegistry::Get<Spice::iGraphics>();

  while (true)
  {
    SDL_GL_MakeCurrent(wnd, context);
    UpdateEvents();

    if (Spice::Input::IsKeyPressed(Spice::Key::eK_Escape))
    {
      break;
    }

    graphics->Clear(true, Spice::Color4f(0.5f, 0.0, 0.0, 0.0f), true, 1.0f, false, 0);

    SDL_GL_SwapWindow(wnd);
  }


  return 0;
}
