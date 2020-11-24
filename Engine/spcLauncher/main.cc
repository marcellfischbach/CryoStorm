
#include <spcLauncher/window/sdlinputsystem.hh>
#include <spcLauncher/window/sdlkeyboard.hh>
#include <spcLauncher/window/sdlmouse.hh>
#include <spcLauncher/launchermodule.hh>
#include <spcCore/coremodule.hh>
#include <spcCore/input/input.hh>
#include <spcCore/objectregistry.hh>
#include <spcCore/graphics/igraphics.hh>
#include <spcCore/graphics/image.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/graphics/shading/ishader.hh>
#include <spcCore/resource/assetmanager.hh>
#include <spcCore/resource/vfs.hh>


#include <spcOpenGL/openglmodule.hh>
#include <GL/glew.h>

#include <spcAssimp/assimpmodule.hh>

#include <iostream>
#include <SDL.h>
#include <regex>
#include <string>


spc::SDLKeyboard keyboard;
spc::SDLMouse mouse;


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

std::vector<std::string> split(const std::string& string)
{
  std::vector<std::string> res;
  size_t offset = 0;
  size_t idx = 0;
  while ((idx = string.find('\n', offset)) != std::string::npos)
  {
    std::string part = string.substr(offset, idx - offset);
    res.push_back(part);
    offset = idx+1;
  }
  std::string part = string.substr(offset, string.length() - offset);
  res.push_back(part);
  
  return res;
}

std::string merge(const std::vector<std::string>& lines)
{
  std::string res;
  for (const std::string& str : lines)
  {
    res += str + "\n";
  }
  return res;
}


int main(int argc, char** argv)
{

  {

    std::string sub = std::string("") + std::string(" layout (location = eVS_Vertices) out vec4 narf;\nDas sind\nmehrere Zeilen\n");
    std::vector<std::string> lines = split(sub);
    for (const std::string& line : lines)
    {
      std::cout << "Lines: " << line << std::endl;
    }
    std::smatch sm;
    std::regex reg("(.*layout\\s*\\(location\\s*=\\s*)([a-zA-Z0-9_]*)(\\s*\\).*)");
    if (std::regex_match(sub, sm, reg))
    {
      for (std::string part : sm)
      {
        std::cout << "Part [" << part << "]" << std::endl;
      }

      for (int i = 0; i < sm.size(); i++)
      {
        std::cout << "Part_" << i << " = [" << sm[i] << "]" << std::endl;

      }
    }
    else
    {
      std::cout << "Don't match" << std::endl;
    }
  }

 
  if (false)
  {
    return 0;
  }
  if (!spc::LauncherModule::Register(argc, argv))
  {
    printf("Unable to register launcher\n");
    return -1;
  }
  if (!spc::CoreModule::Register(argc, argv))
  {
    printf("Unable to register core\n");
    return -1;
  }
  if (!spc::OpenGLModule::Register(argc, argv))
  {
    printf("Unable to register opengl\n");
    return -1;
  }
  if (!spc::AssimpModule::Register(argc, argv))
  {
    printf("Unable to register assimp\n");
    return -1;
  }

  spc::VFS::Get()->SetBasePath("D:\\DEV\\SpiceEngine\\data");

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



  if (!spc::CoreModule::Initialize(argc, argv))
  {
    printf("Unable to initialize core\n");
    return -1;
  }
  if (!spc::OpenGLModule::Initialize(argc, argv))
  {
    printf("Unable to initialize opengl\n");
    return -1;
  }
  if (!spc::AssimpModule::Initialize(argc, argv))
  {
    printf("Unable to initialize assimp\n");
    return -1;
  }

  spc::iShader* shader = spc::AssetManager::Get()->Load<spc::iShader>(spc::ResourceLocator("testprogram.xml"));

  //spc::Image* image = spc::AssetManager::Get()->Load<spc::Image>(spc::ResourceLocator("snowflake_64.png"));

  spc::iGraphics* graphics = spc::ObjectRegistry::Get<spc::iGraphics>();


  //
  // create a render mesh
  spc::iRenderMeshGenerator* generator = spc::ObjectRegistry::Get<spc::iRenderMeshGeneratorFactory>()->Create();
  std::vector<spc::Vector3f> position;
  position.push_back(spc::Vector3f(-0.5f, -0.5f, 0.0f));
  position.push_back(spc::Vector3f(-0.5f, 0.5f, 0.0f));
  position.push_back(spc::Vector3f(0.5f, -0.5f, 0.0f));
  position.push_back(spc::Vector3f(0.5f, 0.5f, 0.0f));
  std::vector<spc::UInt32> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(3);
  indices.push_back(0);
  indices.push_back(3);
  indices.push_back(2);
  std::vector<spc::Color4f> colors;
  colors.push_back(spc::Color4f(0.0f, 0.0f, 0.0f, 1.0f));
  colors.push_back(spc::Color4f(0.0f, 1.0f, 0.0f, 1.0f));
  colors.push_back(spc::Color4f(0.0f, 0.0f, 1.0f, 1.0f));
  colors.push_back(spc::Color4f(0.0f, 1.0f, 1.0f, 1.0f));
  generator->SetVertices(position);
  generator->SetIndices(indices);
  generator->SetColors(colors);
  spc::iRenderMesh* renderMesh = generator->Generate();
  generator->Release();






  while (true)
  {
    SDL_GL_MakeCurrent(wnd, context);
    UpdateEvents();

    if (spc::Input::IsKeyPressed(spc::Key::eK_Escape))
    {
      break;
    }

    glViewport(0, 0, 1024, 768);
    graphics->Clear(true, spc::Color4f(0.5f, 0.0, 0.0, 0.0f), true, 1.0f, false, 0);
    graphics->SetShader(shader);
    renderMesh->Render(graphics, spc::eRP_Forward);

    SDL_GL_SwapWindow(wnd);
  }


  return 0;
}

