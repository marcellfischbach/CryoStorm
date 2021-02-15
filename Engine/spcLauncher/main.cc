
#include <spcLauncher/window/sdlinputsystem.hh>
#include <spcLauncher/window/sdlkeyboard.hh>
#include <spcLauncher/window/sdlmouse.hh>
#include <spcLauncher/launchermodule.hh>
#include <spcCore/coremodule.hh>
#include <spcCore/entity/entity.hh>
#include <spcCore/entity/entitystate.hh>
#include <spcCore/entity/lightstate.hh>
#include <spcCore/entity/spatialstate.hh>
#include <spcCore/entity/staticmeshstate.hh>
#include <spcCore/entity/world.hh>
#include <spcCore/input/input.hh>
#include <spcCore/math/math.hh>
#include <spcCore/objectregistry.hh>
#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/image.hh>
#include <spcCore/graphics/ipointlight.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/graphics/isampler.hh>
#include <spcCore/graphics/samplers.hh>
#include <spcCore/graphics/mesh.hh>
#include <spcCore/graphics/projector.hh>
#include <spcCore/graphics/shading/ishader.hh>
#include <spcCore/graphics/shading/ishaderattribute.hh>
#include <spcCore/graphics/material/material.hh>
#include <spcCore/graphics/material/materialinstance.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/graphics/scene/gfxmesh.hh>
#include <spcCore/resource/assetmanager.hh>
#include <spcCore/resource/vfs.hh>

#include <spcAssimpLoader/assimploadermodule.hh>
#include <spcOpenGL/openglmodule.hh>
#include <GL/glew.h>

#include <spcImgLoader/imgloadermodule.hh>

#include <iostream>
#include <SDL.h>
#include <regex>
#include <string>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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
      case SDL_KEYDOWN:keyboard.Update(evt.key.keysym.scancode, true);
        break;
      case SDL_KEYUP:keyboard.Update(evt.key.keysym.scancode, false);
        break;
      case SDL_MOUSEBUTTONDOWN:mouse.Update(evt.button.button, true);
        break;
      case SDL_MOUSEBUTTONUP:mouse.Update(evt.button.button, false);
        break;
      case SDL_MOUSEWHEEL:mouse.Update(evt.wheel.y, evt.wheel.x);
        break;
      case SDL_MOUSEMOTION:mouse.Update(evt.motion.x, evt.motion.y, evt.motion.xrel, evt.motion.yrel);
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

bool register_modules(int argc, char** argv)
{

  if (!spc::LauncherModule::Register(argc, argv))
  {
    printf("Unable to register launcher\n");
    return false;
  }
  if (!spc::CoreModule::Register(argc, argv))
  {
    printf("Unable to register core\n");
    return false;
  }
  if (!spc::opengl::OpenGLModule::Register(argc, argv))
  {
    printf("Unable to register opengl\n");
    return false;
  }
  if (!spc::assimp::AssimpLoaderModule::Register(argc, argv))
  {
    printf("Unable to register assimp loader\n");
    return false;
  }
  if (!spc::img::ImgLoaderModule::Register(argc, argv))
  {
    printf("Unable to register png loader\n");
    return false;
  }

  return true;
}
SDL_Window* wnd;
SDL_GLContext context;


bool initialize_modules(int argc, char** argv)
{
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

  wnd = SDL_CreateWindow("Spice", 25, 25, 1024, 768, SDL_WINDOW_OPENGL);
  context = SDL_GL_CreateContext(wnd);
  bool vsync = true;
  SDL_GL_SetSwapInterval(vsync ? 1 : 0);

  SDL_ShowWindow(wnd);
  SDL_GL_MakeCurrent(wnd, context);

  if (!spc::CoreModule::Initialize(argc, argv))
  {
    printf("Unable to initialize core\n");
    return false;
  }
  if (!spc::opengl::OpenGLModule::Initialize(argc, argv))
  {
    printf("Unable to initialize opengl\n");
    return false;
  }
  if (!spc::assimp::AssimpLoaderModule::Initialize(argc, argv))
  {
    printf("Unable to initialize assimp loader\n");
    return false;
  }
  if (!spc::img::ImgLoaderModule::Initialize(argc, argv))
  {
    printf("Unable to initialize png loader\n");
    return false;
  }
  return true;
}

spc::iRenderMesh* create_plane_mesh()
{
  //
  // create a render mesh
  spc::iRenderMeshGenerator* generator = spc::ObjectRegistry::Get<spc::iRenderMeshGeneratorFactory>()->Create();
  std::vector<spc::Vector3f> positions;
  positions.push_back(spc::Vector3f(-100.0f, 0.0f, -100.0f));
  positions.push_back(spc::Vector3f(-100.0f, 0.0f, 100.0f));
  positions.push_back(spc::Vector3f(100.0f, 0.0f, -100.0f));
  positions.push_back(spc::Vector3f(100.0f, 0.0f, 100.0f));
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
  return renderMesh;
}

void debug(spc::SpatialState* state, int indent)
{
  if (!state)
  {
    return;
  }
  for (int i = 0; i < indent; i++)
  {
    printf("  ");
  }
  printf("%s [%s]\n",
         state->GetName().c_str(),
         state->GetEntity() ? state->GetEntity()->GetName().c_str() : "n/a"
  );
  for (spc::Size i = 0, in = state->GetNumberOfChildren(); i < in; i++)
  {
    debug(state->GetChild(i), indent + 1);
  }
}

int main(int argc, char** argv)
{
  if (!register_modules(argc, argv))
  {
    return -1;
  }

  if (!initialize_modules(argc, argv))
  {
    return -1;
  }

  spc::iDevice* device = spc::ObjectRegistry::Get<spc::iDevice>();

  spc::iShader* shader = spc::AssetManager::Get()->Load<spc::iShader>(spc::ResourceLocator("testprogram.xml"));
  shader->RegisterAttribute("Diffuse");
  shader->RegisterAttribute("Color");


  spc::iSampler* sampler = spc::AssetManager::Get()->Load<spc::iSampler>(spc::ResourceLocator("sampler_default.spc"));

  spc::Image* image = spc::AssetManager::Get()->Load<spc::Image>(spc::ResourceLocator("GrassGreenTexture0003.jpg"));
  if (!image)
  {
    image = spc::AssetManager::Get()->Load<spc::Image>(spc::ResourceLocator("snowflake_64.png"));
  }

  if (image)
  {
    image->GenerateMipMaps(spc::Image::eMipMapProcedure::eMMP_Linear4x4);
  }

  spc::iTexture2D::Descriptor desc = {};
  desc.Format = image->GetPixelFormat();
  desc.Width = image->GetWidth();
  desc.Height = image->GetHeight();
  desc.MipMaps = true;
  spc::iTexture2D* texture = device->CreateTexture(desc);
  texture->Data(image);

  spc::Material* material = new spc::Material();
  material->SetShader(spc::eRP_Forward, shader);
  material->RegisterAttribute("Diffuse");
  material->RegisterAttribute("Color");
  material->Set(material->IndexOf("Diffuse"), texture);
  material->Set(material->IndexOf("Color"), spc::Color4f(1, 1, 1, 1));

  spc::MaterialInstance* instance = new spc::MaterialInstance();
  instance->SetMaterial(material);
  instance->Set(instance->IndexOf("Color"), spc::Color4f(0, 1, 1, 1));


  spc::iRenderMesh* renderMesh = create_plane_mesh();
  spc::Mesh* mesh = new spc::Mesh();
  mesh->AddMaterialSlot("Default", instance);
  mesh->AddSubMesh(renderMesh, 0);


  spc::World* world = new spc::World();

  int width, height;
  SDL_GetWindowSize(wnd, &width, &height);

  float aspect = (float) height / (float) width;


  spc::Camera* camera = new spc::Camera();
  spc::Projector projector;


  spc::Mesh* suzanne = spc::AssetManager::Get()->Load<spc::Mesh>(spc::ResourceLocator("suzanne.fbx"));
  spc::Mesh* cube = spc::AssetManager::Get()->Load<spc::Mesh>(spc::ResourceLocator("cube.fbx"));
  suzanne->SetDefaultMaterial(0, material);
  cube->SetDefaultMaterial(0, material);

  spc::Entity* entity0 = new spc::Entity("Entity0");
  spc::StaticMeshState* meshState0 = new spc::StaticMeshState("StaticMesh0");
  meshState0->GetTransform()
            .SetTranslation(spc::Vector3f(-100, 0, 0))
            .Finish();
  meshState0->SetMesh(mesh);
  meshState0->SetStatic(true);
  entity0->Attach(meshState0);
  world->Attach(entity0);

  spc::Entity* entity1 = new spc::Entity("Entity1");
  spc::StaticMeshState* meshState1 = new spc::StaticMeshState("StaticMesh1");
  meshState1->GetTransform()
            .SetTranslation(spc::Vector3f(100, 0, 0))
            .Finish();
  meshState1->SetMesh(mesh);
  meshState1->SetStatic(true);
  entity1->Attach(meshState1);
  world->Attach(entity1);

  spc::Entity* lightEntity = new spc::Entity("Light_0");
  spc::LightState* lightState = new spc::LightState("LightState");
  lightEntity->Attach(lightState);
  lightState->SetType(spc::eLT_Point);
  lightState->SetColor(spc::Color4f(1.0f, 0.0f, 0.0f, 1.0f));
  lightState->SetRange(50.0f);
  lightState->SetStatic(false);
  lightEntity->GetRoot()->GetTransform()
             .SetTranslation(spc::Vector3f(-100.0f, 25.0f, 0.0f))
             .Finish();
  world->Attach(lightEntity);

  spc::Entity* sunEntity = new spc::Entity("Sun");
  spc::LightState* sunLightState = new spc::LightState("SunLight");
  sunEntity->Attach(sunLightState);
  sunLightState->SetType(spc::eLT_Directional);
  sunLightState->SetColor(spc::Color4f(1.0f, 1.0f, 1.0f, 1.0f));
  sunLightState->GetTransform()
    .SetRotation(spc::Quaternion::FromAxisAngle(spc::Vector3f(1.0f, 0.0f, 0.0f), spc::spcDeg2Rad(45.0f)))
    .Finish();
  world->Attach(sunEntity);

  float rot = 0.0f;
  float entRot = 0.0f;

  spc::UInt32 nextSec = SDL_GetTicks() + 1000;
  spc::UInt32 frames = 0;
  spc::UInt32 lastTime = SDL_GetTicks();
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
    spc::UInt32 deltaTime = time - lastTime;
    lastTime = time;

    SDL_GL_MakeCurrent(wnd, context);
    UpdateEvents();

    if (spc::Input::IsKeyPressed(spc::Key::eK_Escape))
    {
      break;
    }


    glViewport(0, 0, 1024, 768);
    device->Clear(true, spc::Color4f(0.0f, 0.0, 0.0, 0.0f), true, 1.0f, false, 0);
    /*
    entityX->GetRoot()->GetTransform()
      .SetRotation(spc::Quaternion::FromAxisAngle(spc::Vector3f(0.0f, 1.0f, 0.0f), entRot * 2))
      .Finish();

    entityZ->GetRoot()->GetTransform()
      .SetRotation(spc::Quaternion::FromAxisAngle(spc::Vector3f(0.0f, 1.0f, 0.0f), entRot / 2.0f))
      .Finish();
    */
    entRot += 0.01f;

    lightEntity->GetRoot()->GetTransform()
               .SetTranslation(spc::Vector3f(0.0f + spc::spcSin(entRot) * 100.0f, 25.0f, 0.0f))
               .Finish();

    float dist = 100.0f;
    camera->SetSpot(spc::Vector3f(0, 0.0f, 0.0f));
    camera->SetEye(spc::Vector3f(spc::spcSin(rot) * dist, dist, -spc::spcCos(rot) * dist));
    camera->Bind(device);

    projector.Bind(device);

    rot += 0.005f;

    world->Update((float) deltaTime / 1000.0f);
    world->UpdateTransformation();
    world->GetScene()->Render(device, spc::eRP_Forward);

    SDL_GL_SwapWindow(wnd);

  }


  return 0;
}

