
#include <spcLauncher/window/sdlinputsystem.hh>
#include <spcLauncher/window/sdlkeyboard.hh>
#include <spcLauncher/window/sdlmouse.hh>
#include <spcLauncher/launchermodule.hh>
#include <spcCore/coremodule.hh>
#include <spcCore/input/input.hh>
#include <spcCore/objectregistry.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/image.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/graphics/shading/ishader.hh>
#include <spcCore/graphics/shading/ishaderattribute.hh>
#include <spcCore/graphics/material/material.hh>
#include <spcCore/graphics/material/materialinstance.hh>
#include <spcCore/graphics/scene/scenemesh.hh>
#include <spcCore/resource/assetmanager.hh>
#include <spcCore/resource/vfs.hh>


#include <spcOpenGL/openglmodule.hh>
#include <GL/glew.h>

#include <spcImgLoader/imgloadermodule.hh>

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
  if (!spc::ImgLoaderModule::Register(argc, argv))
  {
    printf("Unable to register png loader\n");
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
  if (!spc::ImgLoaderModule::Initialize(argc, argv))
  {
    printf("Unable to initialize png loader\n");
    return -1;
  }

  spc::iDevice* graphics = spc::ObjectRegistry::Get<spc::iDevice>();

  spc::iShader* shader = spc::AssetManager::Get()->Load<spc::iShader>(spc::ResourceLocator("testprogram.xml"));
  shader->RegisterAttribute("Diffuse");
  shader->RegisterAttribute("Color");

  



  spc::Image* image = spc::AssetManager::Get()->Load<spc::Image>(spc::ResourceLocator("2k_earth_daymap.jpg"));
  if (!image)
  {
    image = spc::AssetManager::Get()->Load<spc::Image>(spc::ResourceLocator("snowflake_64.png"));
  }
  printf("Image: %p\n", image);

  spc::iTexture2D::Descriptor desc;
  desc.Format = image->GetPixelFormat();
  desc.Width = image->GetWidth();
  desc.Height = image->GetHeight();
  desc.MipMaps = false;
  spc::iTexture2D* texture = graphics->CreateTexture(desc);
  texture->Data(0, image);


  spc::Material* material = new spc::Material();
  material->SetShader(spc::eRP_Forward, shader);
  material->RegisterAttribute("Diffuse");
  material->RegisterAttribute("Color");
  material->Set(material->IndexOf("Diffuse"), texture);
  material->Set(material->IndexOf("Color"), spc::Color4f(1, 1, 1, 1));

  spc::MaterialInstance* instance = new spc::MaterialInstance();
  instance->SetMaterial(material);
  instance->Set(instance->IndexOf("Color"), spc::Color4f(0, 1, 1, 1));


  //
  // create a render mesh
  spc::iRenderMeshGenerator* generator = spc::ObjectRegistry::Get<spc::iRenderMeshGeneratorFactory>()->Create();
  std::vector<spc::Vector3f> position;
  position.push_back(spc::Vector3f(-5.0f, -5.0f, 5.0f));
  position.push_back(spc::Vector3f(-5.0f, 5.0f, 5.0f));
  position.push_back(spc::Vector3f(5.0f, -5.0f, 5.0f));
  position.push_back(spc::Vector3f(5.0f, 5.0f, 5.0f));
  std::vector<spc::Vector2f> uv;
  uv.push_back(spc::Vector2f(0.0f, 0.0f));
  uv.push_back(spc::Vector2f(0.0f, 1.0f));
  uv.push_back(spc::Vector2f(1.0f, 0.0f));
  uv.push_back(spc::Vector2f(1.0f, 1.0f));
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
  generator->SetUV0(uv);
  spc::iRenderMesh* renderMesh = generator->Generate();
  generator->Release();


  spc::SceneMesh* sceneMesh = new spc::SceneMesh();
  sceneMesh->SetMesh(renderMesh);
  sceneMesh->SetMaterial(material);



  //spc::Matrix4f projection = graphics->GetPerspectiveProjection(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f, projection);
  spc::Matrix4f projection = graphics->GetOrthographicProjection(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f, projection);
  graphics->SetProjectionMatrix(projection);

  spc::Matrix4f view;
  view.SetLookAt(spc::Vector3f(10, 10, 10), spc::Vector3f(0, 0, 0), spc::Vector3f(0, 0, 1));


  float rot = 0.0f;

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


    spc::Matrix4f rotMatrix;
    rotMatrix.SetRotationY(rot);
    rot += 0.01f;

    sceneMesh->SetModelMatrix(rotMatrix);
    sceneMesh->Render(graphics, spc::eRP_Forward);

    SDL_GL_SwapWindow(wnd);

  }


  return 0;
}

