
#include <spcLauncher/window/sdlinputsystem.hh>
#include <spcLauncher/window/sdlkeyboard.hh>
#include <spcLauncher/window/sdlmouse.hh>
#include <spcLauncher/launchermodule.hh>
#include <spcCore/coremodule.hh>
#include <spcCore/input/input.hh>
#include <spcCore/math/math.hh>
#include <spcCore/objectregistry.hh>
#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/idirectionallight.hh>
#include <spcCore/graphics/image.hh>
#include <spcCore/graphics/ipointlight.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/graphics/projector.hh>
#include <spcCore/graphics/shading/ishader.hh>
#include <spcCore/graphics/shading/ishaderattribute.hh>
#include <spcCore/graphics/material/material.hh>
#include <spcCore/graphics/material/materialinstance.hh>
#include <spcCore/graphics/scene/scenemesh.hh>
#include <spcCore/resource/assetmanager.hh>
#include <spcCore/resource/binarydata.hh>
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
    offset = idx + 1;
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

    spc::BinaryOutputStream bos;
    bos
      << (spc::Int8)1
      << (spc::UInt8)2
      << (spc::Int16)3
      << (spc::UInt16)4
      << (spc::Int32)5
      << (spc::UInt32)6
      << (spc::Int64)7
      << (spc::UInt64)8
      << (float)9.0f
      << (double)10.0
      << (bool)true
      << std::string("This is a text message");

    FILE* out = fopen("d:\\Dev\\bos.out", "wb");
    fwrite(bos.GetData(), sizeof(spc::UInt8), bos.GetSize(), out);
    fclose(out);

    printf("\n");
    spc::Int8 i8;
    spc::UInt8 u8;
    spc::Int16 i16;
    spc::UInt16 u16;
    spc::Int32 i32;
    spc::UInt32 u32;
    spc::Int64 i64;
    spc::UInt64 u64;
    float f;
    double d;
    bool b;
    std::string str;
    spc::BinaryInputStream bis(bos.GetData(), bos.GetSize());
    bis >> i8;
    bis >> u8;
    bis >> i16;
    bis >> u16;
    bis >> i32;
    bis >> u32;
    bis >> i64;
    bis >> u64;
    bis >> f;
    bis >> d;
    bis >> b;
    bis >> str;
    printf("%d %u\n", i8, u8);
    printf("%d %u\n", i16, u16);
    printf("%d %u\n", i32, u32);
    printf("%lld %llu\n", i64, u64);
    printf("%f %f\n", f, d);
    printf("%d\n", b);
    printf("%s\n", str.c_str());
    printf("\n");
  }

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
  if (!spc::opengl::OpenGLModule::Register(argc, argv))
  {
    printf("Unable to register opengl\n");
    return -1;
  }
  if (!spc::img::ImgLoaderModule::Register(argc, argv))
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
  bool vsync = false;
  SDL_GL_SetSwapInterval(vsync ? 1 : 0);

  SDL_ShowWindow(wnd);
  SDL_GL_MakeCurrent(wnd, context);



  if (!spc::CoreModule::Initialize(argc, argv))
  {
    printf("Unable to initialize core\n");
    return -1;
  }
  if (!spc::opengl::OpenGLModule::Initialize(argc, argv))
  {
    printf("Unable to initialize opengl\n");
    return -1;
  }
  if (!spc::img::ImgLoaderModule::Initialize(argc, argv))
  {
    printf("Unable to initialize png loader\n");
    return -1;
  }

  spc::iDevice* device = spc::ObjectRegistry::Get<spc::iDevice>();

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
  spc::iTexture2D* texture = device->CreateTexture(desc);
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
  std::vector<spc::Vector3f> positions;
  positions.push_back(spc::Vector3f(-5.0f, 0.0f, -5.0f));
  positions.push_back(spc::Vector3f(-5.0f, 0.0f, 5.0f));
  positions.push_back(spc::Vector3f(5.0f, 0.0f, -5.0f));
  positions.push_back(spc::Vector3f(5.0f, 0.0f, 5.0f));
  std::vector<spc::Vector3f> normals;
  normals.push_back(spc::Vector3f(0.0f, 1.0f, 0.0f));
  normals.push_back(spc::Vector3f(0.0f, 1.0f, 0.0f));
  normals.push_back(spc::Vector3f(0.0f, 1.0f, 0.0f));
  normals.push_back(spc::Vector3f(0.0f, 1.0f, 0.0f));
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
  spc::Color4f color(1.0f, 1.0f, 1.0f, 1.0f);
  colors.push_back(color);
  colors.push_back(color);
  colors.push_back(color);
  colors.push_back(color);
  generator->SetVertices(positions);
  generator->SetNormals(normals);
  generator->SetIndices(indices);
  generator->SetColors(colors);
  generator->SetUV0(uv);
  spc::iRenderMesh* renderMesh = generator->Generate();
  generator->Release();


  spc::SceneMesh* sceneMesh = new spc::SceneMesh();
  sceneMesh->SetMesh(renderMesh);
  sceneMesh->SetMaterial(material);

  int width, height;
  SDL_GetWindowSize(wnd, &width, &height);

  float aspect = (float)height / (float)width;
  spc::Matrix4f projection = device->GetPerspectiveProjection(-1.0f, 1.0f, -aspect, aspect, 1.0f, 100.0f, projection);
  //spc::Matrix4f projection = graphics->GetOrthographicProjection(-20.0f, 20.0f, -20.0f, 20.0f, -100.0f, 100.0f, projection);
  device->SetProjectionMatrix(projection);

  spc::Matrix4f view;
  view.SetLookAt(spc::Vector3f(20, 20, 20), spc::Vector3f(0, 0, 0), spc::Vector3f(0, 1, 0));
  device->SetViewMatrix(view);


  spc::Camera* camera = new spc::Camera();
  spc::Projector projector;
  //camera->SetSpot(spc::Vector3f(0, 0, 0));

  spc::iPointLight* pointLight0 = device->CreatePointLight();
  pointLight0->SetColor(spc::Color4f(1.0f, 0.0f, 0.0f, 1.0f));
  pointLight0->SetPosition(spc::Vector3f(-5.0f, 5.0f, -5.0f));
  pointLight0->SetRange(10.0f);

  spc::iPointLight* pointLight1 = device->CreatePointLight();
  pointLight1->SetColor(spc::Color4f(0.0f, 1.0f, 0.0f, 1.0f));
  pointLight1->SetPosition(spc::Vector3f(-5.0f, 5.0f, 5.0f));
  pointLight1->SetRange(10.0f);

  spc::iPointLight* pointLight2 = device->CreatePointLight();
  pointLight2->SetColor(spc::Color4f(0.0f, 0.0f, 1.0f, 1.0f));
  pointLight2->SetPosition(spc::Vector3f(5.0f, 5.0f, -5.0f));
  pointLight2->SetRange(10.0f);

  spc::iPointLight* pointLight3 = device->CreatePointLight();
  pointLight3->SetColor(spc::Color4f(1.0f, 0.0f, 1.0f, 1.0f));
  pointLight3->SetPosition(spc::Vector3f(5.0f, 5.0f, 5.0f));
  pointLight3->SetRange(10.0f);

  sceneMesh->ClearLights();
  sceneMesh->AddLight(pointLight0);
  sceneMesh->AddLight(pointLight1);
  sceneMesh->AddLight(pointLight2);
  sceneMesh->AddLight(pointLight3);


  float rot = 0.0f;

  spc::UInt32 nextSec = SDL_GetTicks() + 1000;
  spc::UInt32 frames = 0;
  while (true)
  {
    Uint32 time = SDL_GetTicks();
    if (time > nextSec)
    {
      nextSec += 1000;
      char buffer[1024];
      sprintf_s<1024>(buffer, "Spice Engine %d FPS", frames);
      SDL_SetWindowTitle(wnd, buffer);
      frames = 0;
    }
    else
    {
      frames++;
    }


    SDL_GL_MakeCurrent(wnd, context);
    UpdateEvents();

    if (spc::Input::IsKeyPressed(spc::Key::eK_Escape))
    {
      break;
    }

    if (spc::Input::IsKeyPressed(spc::Key::eK_V))
    {
      vsync = !vsync;
      SDL_GL_SetSwapInterval(vsync ? 1 : 0);
    }

    glViewport(0, 0, 1024, 768);
    device->Clear(true, spc::Color4f(0.0f, 0.0, 0.0, 0.0f), true, 1.0f, false, 0);


    spc::Matrix4f rotMatrix;
    rotMatrix.SetRotationY(rot);
    //sceneMesh->SetModelMatrix(rotMatrix);


    //camera->SetSpot(spc::Vector3f(0, 20 * -spc::spcCos(rot), 0.0f));
    //camera->SetEye(spc::Vector3f(20, 20 * spc::spcCos(rot), 20.0f));
    camera->SetEye(spc::Vector3f(5.0f * spc::spcCos(rot), 5.0f, spc::spcSin(rot) * 5.0f));
    camera->SetSpot(spc::Vector3f(0.0f, 0.0f, 0.0f));
    camera->Bind(device);

    projector.Bind(device);

    rot += 0.00005f;

    sceneMesh->Render(device, spc::eRP_Forward);

    SDL_GL_SwapWindow(wnd);

  }


  return 0;
}

