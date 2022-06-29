
#include <ceLauncher/window/sdlinputsystem.hh>
#include <ceLauncher/window/sdlkeyboard.hh>
#include <ceLauncher/window/sdlmouse.hh>
#include <ceLauncher/launchermodule.hh>
#include <ceBullet/bulletmodule.hh>
#include <ceCore/coremodule.hh>
#include <ceCore/settings.hh>
#include <ceCore/entity/camerastate.hh>
#include <ceCore/entity/entity.hh>
#include <ceCore/entity/entitystate.hh>
#include <ceCore/entity/lightstate.hh>
#include <ceCore/entity/spatialstate.hh>
#include <ceCore/entity/staticmeshstate.hh>
#include <ceCore/entity/world.hh>
#include <ceCore/input/input.hh>
#include <ceCore/math/math.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/iframerenderer.hh>
#include <ceCore/graphics/image.hh>
#include <ceCore/graphics/ipointlight.hh>
#include <ceCore/graphics/irendermesh.hh>
#include <ceCore/graphics/irenderpipeline.hh>
#include <ceCore/graphics/irendertarget2d.hh>
#include <ceCore/graphics/isampler.hh>
#include <ceCore/graphics/iterrainmesh.hh>
#include <ceCore/graphics/samplers.hh>
#include <ceCore/graphics/mesh.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>
#include <ceCore/graphics/material/material.hh>
#include <ceCore/graphics/material/materialinstance.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <ceCore/physics/physics.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/resource/resourcelocator.hh>
#include <ceCore/physics/physics.hh>
#include <ceAssimpLoader/assimploadermodule.hh>
#include <ceOpenGL/openglmodule.hh>
#include <GL/glew.h>

#include <ceImgLoader/imgloadermodule.hh>

#include <ceLauncher/camerahandler.hh>
#include <ceLauncher/mirrorhandler.hh>

#include <iostream>
#include <SDL.h>
#include <regex>
#include <string>

ce::SDLKeyboard keyboard;
ce::SDLMouse mouse;

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

std::vector<std::string> split(const std::string &string)
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

std::string merge(const std::vector<std::string> &lines)
{
  std::string res;
  for (const std::string &str: lines)
  {
    res += str + "\n";
  }
  return res;
}

bool register_modules(int argc, char **argv)
{

  if (!ce::LauncherModule::Register(argc, argv))
  {
    printf("Unable to register launcher\n");
    return false;
  }
  if (!ce::CoreModule::Register(argc, argv))
  {
    printf("Unable to register core\n");
    return false;
  }
  if (!ce::bullet::BulletModule::Register(argc, argv))
  {
    printf("Unable to register bullet");
    return false;
  }
  if (!ce::opengl::OpenGLModule::Register(argc, argv))
  {
    printf("Unable to register opengl\n");
    return false;
  }
  if (!ce::assimp::AssimpLoaderModule::Register(argc, argv))
  {
    printf("Unable to register assimp loader\n");
    return false;
  }
  if (!ce::img::ImgLoaderModule::Register(argc, argv))
  {
    printf("Unable to register png loader\n");
    return false;
  }

  return true;
}

SDL_Window *wnd;
SDL_GLContext context;


bool initialize_modules(int argc, char **argv)
{
  ce::VFS::Get()->SetBasePath("D:\\DEV\\CrimsonEdge\\data");
  ce::Settings settings("file:///config/display.cef");

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

  uint32_t flags = SDL_WINDOW_OPENGL;
  // flags |= SDL_WINDOW_BORDERLESS;
  std::string title = settings.GetText("title");
  ce::Vector2i res = settings.GetVector2i("resolution");
  ce::Vector2i pos = settings.GetVector2i("pos");
  std::string viewMode = settings.GetText("viewmode", "windowed");
  if (viewMode == "viewMode")
  {
    flags |= SDL_WINDOW_FULLSCREEN;
  }
  else if (viewMode == "desktop")
  {
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  }
  else if (viewMode != "windowed")
  {
    printf("Illegal viewmode: '%s'. Defaulting to 'windowed'\n", viewMode.c_str());
  }

  bool vsync = settings.GetBool("vsync");
  wnd = SDL_CreateWindow(title.c_str(),
                         pos.x, pos.y,
                         res.x, res.y, flags
  );
  mouse.SetWindow(wnd);
  //  wnd = SDL_CreateWindow("Spice", 0, 0, 1920, 1080, flags);
  context = SDL_GL_CreateContext(wnd);
  SDL_GL_SetSwapInterval(vsync ? 1 : 0);

  SDL_ShowWindow(wnd);
  SDL_GL_MakeCurrent(wnd, context);

  if (!ce::CoreModule::Initialize(argc, argv))
  {
    printf("Unable to initialize core\n");
    return false;
  }
  if (!ce::bullet::BulletModule::Initialize(argc, argv))
  {
    printf("Unable to initialize bullet\n");
    return false;
  }
  if (!ce::opengl::OpenGLModule::Initialize(argc, argv))
  {
    printf("Unable to initialize opengl\n");
    return false;
  }
  if (!ce::assimp::AssimpLoaderModule::Initialize(argc, argv))
  {
    printf("Unable to initialize assimp loader\n");
    return false;
  }
  if (!ce::img::ImgLoaderModule::Initialize(argc, argv))
  {
    printf("Unable to initialize png loader\n");
    return false;
  }
  return true;
}

void set_window_icon()
{
  ce::Image *image = ce::AssetManager::Get()->Load<ce::Image>("file:///icons/ce24.png");
  if (!image)
  {
    return;
  }

  SDL_Surface *surf = SDL_CreateRGBSurface(0,
                                           image->GetWidth(),
                                           image->GetHeight(),
                                           32,
                                           0xff000000,
                                           0x00ff0000,
                                           0x0000ff00,
                                           0x000000ff);
  SDL_LockSurface(surf);
  SDL_memcpy(surf->pixels, image->GetData(), image->GetWidth() * image->GetHeight() * 4);
  SDL_UnlockSurface(surf);
  SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_BLEND);

  SDL_SetWindowIcon(wnd, surf);

}

ce::iRenderMesh *create_plane_mesh(float size, float nx, float ny)
{
  //
  // create a render mesh
  ce::iRenderMeshGenerator *generator = ce::ObjectRegistry::Get<ce::iRenderMeshGeneratorFactory>()->Create();
  std::vector<ce::Vector3f> positions;
  positions.push_back(ce::Vector3f(-size, 0.0f, -size));
  positions.push_back(ce::Vector3f(-size, 0.0f, size));
  positions.push_back(ce::Vector3f(size, 0.0f, -size));
  positions.push_back(ce::Vector3f(size, 0.0f, size));
  std::vector<ce::Vector3f> normals;
  normals.push_back(ce::Vector3f(0.0f, 1.0f, 0.0f));
  normals.push_back(ce::Vector3f(0.0f, 1.0f, 0.0f));
  normals.push_back(ce::Vector3f(0.0f, 1.0f, 0.0f));
  normals.push_back(ce::Vector3f(0.0f, 1.0f, 0.0f));
  std::vector<ce::Vector2f> uv;
  uv.push_back(ce::Vector2f(0.0f, 0.0f));
  uv.push_back(ce::Vector2f(0.0f, ny));
  uv.push_back(ce::Vector2f(nx, 0.0f));
  uv.push_back(ce::Vector2f(nx, ny));
  std::vector<uint32_t> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(3);
  indices.push_back(0);
  indices.push_back(3);
  indices.push_back(2);
  std::vector<ce::Color4f> colors;
  ce::Color4f color(1.0f, 1.0f, 1.0f, 1.0f);
  colors.push_back(color);
  colors.push_back(color);
  colors.push_back(color);
  colors.push_back(color);
  generator->SetVertices(positions);
  generator->SetNormals(normals);
  generator->SetIndices(indices);
  generator->SetColors(colors);
  generator->SetUV0(uv);
  ce::iRenderMesh *renderMesh = generator->Generate();
  generator->Release();
  return renderMesh;
}

ce::iTerrainMesh *create_terrain_mesh(float size)
{
  ce::iTerrainMeshGenerator *generator = ce::ObjectRegistry::Get<ce::iTerrainMeshGeneratorFactory>()->Create();

  std::vector<float> heightData;
  for (int i = 0; i < 1025; i++)
  {
    float fi = (float)i / 1024.0f;
    for (int j = 0; j < 1025; j++)
    {
      float fj = (float)j / 1024.0f;

      float a = sin(fi*10.0f) * cos(fj * 10.0f);

      heightData.push_back(0.5f + 0.5f * a);
    }
  }

  generator->SetSize(ce::eTerrainSize::TS_129);
  generator->SetPatchSize(ce::eTerrainSize::TS_33);
  generator->SetSize(ce::eTerrainSize::TS_1025);
  generator->SetPatchSize(ce::eTerrainSize::TS_65);
  generator->SetNormalizedHeightData(heightData);
  generator->SetSize(ce::Vector3f(-size, 0.0f, -size), ce::Vector3f(size, 10.0f, size));

  ce::iTerrainMesh *terrainMesh = generator->Generate();
  generator->Release();
  return terrainMesh;
}

ce::iRenderMesh *create_sphere_mesh(float radius, uint32_t detail, float uv_f)
{
  ce::iRenderMeshGenerator *generator = ce::ObjectRegistry::Get<ce::iRenderMeshGeneratorFactory>()->Create();
  std::vector<ce::Vector3f> positions;
  std::vector<ce::Vector3f> normals;
  std::vector<ce::Vector2f> uv;
  std::vector<ce::Color4f> colors;
  std::vector<uint32_t> indices;

  for (uint32_t v = 0; v < detail; v++)
  {
    float factV = (float) v / (float) (detail - 1);
    float angleV = -(float) M_PI_2 + factV * (float) M_PI;

    for (uint32_t h = 0; h < detail * 2; h++)
    {
      float factH = (float) h / (float) (detail * 2 - 1);
      float angleH = factH * (float) M_PI * 2.0f;

      ce::Vector3f normal(
          cosf(angleV) * cosf(angleH),
          sinf(angleV),
          cosf(angleV) * sinf(angleH)
      );
      positions.push_back(normal * radius);
      normals.emplace_back(normal);
      uv.emplace_back(ce::Vector2f(factH * 2.0f * uv_f, factV * uv_f));
      colors.emplace_back(ce::Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    }
  }

  for (uint32_t v = 0; v < detail - 1; v++)
  {
    uint32_t i0 = v * detail * 2;
    uint32_t i1 = i0 + detail * 2;
    for (uint32_t h = 0; h < detail * 2 - 1; h++)
    {
      uint32_t i00 = i0 + h;
      uint32_t i01 = i00 + 1;
      uint32_t i10 = i1 + h;
      uint32_t i11 = i10 + 1;

      indices.emplace_back(i00);
      indices.emplace_back(i10);
      indices.emplace_back(i11);

      indices.emplace_back(i00);
      indices.emplace_back(i11);
      indices.emplace_back(i01);
    }
  }
  generator->SetVertices(positions);
  generator->SetNormals(normals);
  generator->SetColors(colors);
  generator->SetUV0(uv);
  generator->SetIndices(indices);
  ce::iRenderMesh *renderMesh = generator->Generate();
  generator->Release();
  return renderMesh;

}

void debug(ce::SpatialState *state, int indent)
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
  for (ce::Size i = 0, in = state->GetNumberOfChildren(); i < in; i++)
  {
    debug(state->GetChild(i), indent + 1);
  }
}


void create_suzannes_plain(ce::Mesh *suzanneMesh, ce::World *world, ce::iMaterial *alternativeMaterial)
{
  size_t num = 20;
  for (size_t i = 0; i < num; i++)
  {
    float x = -40.0f + (float) i / (float) num * 80.0f;
    for (size_t j = 0; j < num; j++)
    {
      float y = -40.0f + (float) j / (float) num * 80.0f;

      ce::Entity *suzanneEntity = new ce::Entity("Entity1");
      ce::StaticMeshState *meshState1 = new ce::StaticMeshState("StaticMesh1");
      meshState1->GetTransform()
                .SetTranslation(ce::Vector3f(x, 0, y))
                .Finish();
      meshState1->SetMesh(suzanneMesh);
      meshState1->SetStatic(true);
      meshState1->SetCastShadow(true);
      if ((i + j) % 2 == 0)
      {
        meshState1->SetMaterial(0, alternativeMaterial);
      }
      suzanneEntity->Attach(meshState1);
      world->Attach(suzanneEntity);
    }
  }
}


void create_suzanne_batch(ce::Mesh *suzanneMesh,
                          int a,
                          int b,
                          size_t numI,
                          size_t numJ,
                          size_t maxI,
                          size_t maxJ,
                          ce::World *world
)
{
  auto generator = ce::ObjectRegistry::Get<ce::iRenderMeshBatchGeneratorFactory>()->Create();

  for (size_t ni = 0, i = a * numI; ni < numI; ni++, i++)
  {
    float x = -40.0f + (float) i / (float) maxI * 80.0f;
    for (size_t nj = 0, j = b * numJ; nj < numJ; nj++, j++)
    {
      float y = -40.0f + (float) j / (float) maxJ * 80.0f;
      ce::Matrix4f mat;
      mat.SetTranslation(ce::Vector3f(x, 0, y));
      generator->Add(suzanneMesh->GetSubMesh(0).GetMesh(), mat);
    }
  }

  ce::iRenderMesh *batchedRM = generator->Generate();

  ce::Mesh *suzyMesh = new ce::Mesh();
  for (size_t i = 0; i < suzanneMesh->GetNumberOfMaterialSlots(); i++)
  {
    suzyMesh->AddMaterialSlot(suzanneMesh->GetMaterialSlot(i).GetName(),
                              suzanneMesh->GetMaterialSlot(i).GetDefaultMaterial());
  }
  suzyMesh->AddSubMesh(batchedRM, 0);

  ce::Entity *suzanneEntity = new ce::Entity("Entity1");
  ce::StaticMeshState *meshState1 = new ce::StaticMeshState("StaticMesh1");
  meshState1->GetTransform()
            .SetTranslation(ce::Vector3f(0, 0, 0))
            .Finish();
  meshState1->SetMesh(suzyMesh);
  meshState1->SetStatic(true);
  suzanneEntity->Attach(meshState1);
  world->Attach(suzanneEntity);

  generator->Release();
}

void create_suzannes_batched(ce::Mesh *suzanneMesh, ce::World *world)
{
  for (int a = 0; a < 10; a++)
  {
    for (int b = 0; b < 10; b++)
    {
      create_suzanne_batch(suzanneMesh, a, b, 2, 2, 20, 20, world);
    }
  }
}

ce::iRenderTarget2D *create_render_target(ce::iDevice *device, uint32_t width, uint32_t height, uint16_t multiSamples)
{
  ce::iSampler *colorSampler = device->CreateSampler();
  colorSampler->SetFilterMode(ce::eFM_MinMagNearest);

  ce::iSampler *depthSampler = device->CreateSampler();
  depthSampler->SetFilterMode(ce::eFM_MinMagNearest);

  ce::iTexture2D::Descriptor rt_col_desc = {};
  rt_col_desc.Width = width;
  rt_col_desc.Height = height;
  rt_col_desc.Format = ce::ePF_RGBA;
  rt_col_desc.MipMaps = false;
  rt_col_desc.MultiSamples = multiSamples;
  ce::iTexture2D *color_texture = device->CreateTexture(rt_col_desc);
  color_texture->SetSampler(colorSampler);

  ce::iTexture2D::Descriptor rt_dpth_desc = {};
  rt_dpth_desc.Width = width;
  rt_dpth_desc.Height = height;
  rt_dpth_desc.Format = ce::ePF_Depth;
  rt_dpth_desc.MipMaps = false;
  rt_dpth_desc.MultiSamples = multiSamples;
  ce::iTexture2D *depth_texture = device->CreateTexture(rt_dpth_desc);
  depth_texture->SetSampler(depthSampler);


  ce::iRenderTarget2D::Descriptor rt_desc = {};
  rt_desc.Width = width;
  rt_desc.Height = height;

  ce::iRenderTarget2D *renderTarget = device->CreateRenderTarget(rt_desc);
  renderTarget->AddColorTexture(color_texture);
//  renderTarget->SetDepthBuffer(ce::ePF_Depth);
  renderTarget->SetDepthTexture(depth_texture);
  if (!renderTarget->Compile())
  {
    printf("Unable to compile render target: %s\n", renderTarget->GetCompileLog().c_str());
    return nullptr;
  }
  return renderTarget;
}

#include <regex>
#include <sstream>

int main(int argc, char **argv)
{

  if (!register_modules(argc, argv))
  {
    return -1;
  }

  if (!initialize_modules(argc, argv))
  {
    return -1;
  }

  set_window_icon();

  ce::iDevice *device = ce::ObjectRegistry::Get<ce::iDevice>();


  ce::Material *transMaterial = ce::AssetManager::Get()->Get<ce::Material>(ce::ResourceLocator(
      "/materials/test_trans_material.cef"
  ));
  ce::MaterialInstance *transRedMaterial = ce::AssetManager::Get()->Get<ce::MaterialInstance>(ce::ResourceLocator(
      "/materials/red_transparent_material_instance.cef"
  ));
  ce::MaterialInstance *transBlueMaterial = ce::AssetManager::Get()->Get<ce::MaterialInstance>(ce::ResourceLocator(
      "/materials/blue_transparent_material_instance.cef"
  ));


  ce::MaterialInstance *materialInstance = ce::AssetManager::Get()->Get<ce::MaterialInstance>(ce::ResourceLocator(
      "/materials/test_material_instance.cef"
  ));

  ce::MaterialInstance *materialInstance2 = ce::AssetManager::Get()->Get<ce::MaterialInstance>(ce::ResourceLocator(
      "/materials/test_material2_instance.cef"
  ));


//  ce::Material *materialMirror = ce::AssetManager::Get()->Get<ce::Material>(ce::ResourceLocator(
//      "/materials/test_material_mirror.cef"
//  ));

//  ce::iRenderMesh *renderMesh = create_plane_mesh(40.0f, 8, 8);
//  ce::Mesh *mesh = new ce::Mesh();
//  mesh->AddMaterialSlot("Default", materialInstance);
//  mesh->AddSubMesh(renderMesh, 0);


  ce::iTerrainMesh *terrainMesh = create_terrain_mesh(40.0f);
  ce::Mesh *mesh = new ce::Mesh();
  mesh->AddMaterialSlot("Default", materialInstance2);
  mesh->AddSubMesh(terrainMesh, 0);


  ce::iRenderMesh *transPlaneMesh = create_plane_mesh(10.0f, 8, 8);
  ce::Mesh *transRedMesh = new ce::Mesh();
  transRedMesh->AddMaterialSlot("Default", transRedMaterial);
  transRedMesh->AddSubMesh(transPlaneMesh, 0);
  ce::Mesh *transBlueMesh = new ce::Mesh();
  transBlueMesh->AddMaterialSlot("Default", transBlueMaterial);
  transBlueMesh->AddSubMesh(transPlaneMesh, 0);


  ce::World *world = new ce::World();

  int wnd_width, wnd_height;
  SDL_GetWindowSize(wnd, &wnd_width, &wnd_height);

  ce::Settings settings("file:///config/display.cef");
  ce::Vector2i resolution = settings.GetVector2i("resolution", ce::Vector2i(wnd_width, wnd_height));
  int width = resolution.x;
  int height = resolution.y;
  int multiSamples = settings.GetInt("multisamples", 1);

  float aspect = (float) wnd_height / (float) wnd_width;


  ce::Mesh *suzanneMesh = ce::AssetManager::Get()->Load<ce::Mesh>(ce::ResourceLocator("file:///suzanne.fbx"));
  ce::Mesh *cube = ce::AssetManager::Get()->Load<ce::Mesh>(ce::ResourceLocator("cube.fbx"));
  suzanneMesh->SetDefaultMaterial(0, materialInstance);
  cube->SetDefaultMaterial(0, materialInstance);

  ce::Entity *entity0 = new ce::Entity("Entity0");
  ce::StaticMeshState *meshState0 = new ce::StaticMeshState("StaticMesh0");

  meshState0->GetTransform()
            .SetTranslation(ce::Vector3f(0, 0, 0))
            .Finish();
  meshState0->SetMesh(mesh);
//  meshState0->SetMaterial(0, materialMirror);
  meshState0->SetStatic(true);
  entity0->Attach(meshState0);
  world->Attach(entity0);


  ce::Entity *entityTransPlaneRed = new ce::Entity("Entity0");
  ce::StaticMeshState *meshStateTransPlaneRed = new ce::StaticMeshState("StaticMeshTransPlane");

  meshStateTransPlaneRed->GetTransform()
                        .SetTranslation(ce::Vector3f(0, 2.0f, 0))
                        .Finish();
  meshStateTransPlaneRed->SetMesh(transRedMesh);
  meshStateTransPlaneRed->SetStatic(true);
  entityTransPlaneRed->Attach(meshStateTransPlaneRed);

  ce::Entity *entityTransPlaneBlue = new ce::Entity("Entity0");
  ce::StaticMeshState *meshStateTransPlaneBlue = new ce::StaticMeshState("StaticMeshTransPlane");
  meshStateTransPlaneBlue->GetTransform()
                         .SetTranslation(ce::Vector3f(0, 0.10f, 0))
                         .Finish();
  meshStateTransPlaneBlue->SetMesh(transBlueMesh);
  meshStateTransPlaneBlue->SetStatic(true);
  entityTransPlaneBlue->Attach(meshStateTransPlaneBlue);

  float sphereRadius = 4.0f;
  ce::iRenderMesh *renderMeshSphere = create_sphere_mesh(sphereRadius, 32, 4.0f);
  ce::Mesh *meshSphere = new ce::Mesh();
  ce::Entity *entitySphere = new ce::Entity("Sphere");
  ce::StaticMeshState *meshStateSphere = new ce::StaticMeshState("Mesh.Sphere");
  meshSphere->AddMaterialSlot("Default", materialInstance);
  meshSphere->AddSubMesh(renderMeshSphere, 0);
  meshStateSphere->GetTransform()
                 .SetTranslation(ce::Vector3f(0.0f, sphereRadius * 1.5f, 0.0f))
                 .Finish();
  meshStateSphere->SetMesh(meshSphere);
  entitySphere->Attach(meshStateSphere);


//  materialInstance2->GetMaterial()->SetFillMode(ce::eFillMode::Wireframe);
  //create_suzannes_plain(suzanneMesh, world, materialInstance2);
//  create_suzannes_batched(suzanneMesh, world);

  ce::Entity *lightEntity = new ce::Entity("Light_0");
  ce::LightState *lightState = new ce::LightState("LightState");
  lightEntity->Attach(lightState);
  lightState->SetType(ce::eLT_Point);
  lightState->SetColor(ce::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 1.0f);
  lightState->SetRange(50);
  lightState->SetStatic(true);
  lightState->SetCastShadow(false);
  lightState->GetTransform()
            .SetTranslation(ce::Vector3f(5.0f, 5.0f, 5.0f))
            .Finish();
//  world->Attach(lightEntity);

  /*
  lightEntity = new ce::Entity("Light_0");
  lightState = new ce::LightState("LightState");
  lightEntity->Attach(lightState);
  lightState->SetType(ce::eLT_Point);
  lightState->SetColor(ce::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 0.5f);
  lightState->SetRange(25.0f);
  lightState->SetStatic(true);
  lightState->SetCastShadow(true);
  lightEntity->GetRoot()->GetTransform()
    .SetTranslation(ce::Vector3f(5.0f, 5.0f, 5.0f))
    .Finish();
  world->Attach(lightEntity);
  */

  ce::Entity *sunEntity = new ce::Entity("Sun");
  ce::LightState *sunLightState = new ce::LightState("SunLight");
  sunEntity->Attach(sunLightState);
  sunLightState->SetType(ce::eLT_Directional);
  sunLightState->SetColor(ce::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 1.0f);
  sunLightState->SetShadowMapBias(0.003f);
  sunLightState->SetStatic(true);
  sunLightState->SetCastShadow(false);
  sunLightState->SetTransform(sunLightState->GetTransform()
                                               //.SetRotation(ce::Quaternion::FromAxisAngle(ce::Vector3f(1.0f, 0.0f, 0.0f), ce::ceDeg2Rad(-45.0f)))
                                           .SetRotation(
                                               ce::Quaternion::FromAxisAngle(ce::Vector3f(1.0f, 0.0f, 0.0f)
                                                                                 .Normalize(),
                                                                             ce::ceDeg2Rad(-45.0f)))
  );

  world->Attach(sunEntity);

  sunEntity = new ce::Entity("Sun");
  sunLightState = new ce::LightState("SunLight");
  sunEntity->Attach(sunLightState);
  sunLightState->SetType(ce::eLT_Directional);
  sunLightState->SetColor(ce::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 0.2f);
  sunLightState->SetShadowMapBias(0.003f);
  sunLightState->SetStatic(true);
  sunLightState->SetCastShadow(false);
  sunLightState->SetTransform(sunLightState->GetTransform()
                                               //.SetRotation(ce::Quaternion::FromAxisAngle(ce::Vector3f(1.0f, 0.0f, 0.0f), ce::ceDeg2Rad(-45.0f)))
                                           .SetRotation(
                                               ce::Quaternion::FromAxisAngle(ce::Vector3f(1.0f, 0.0f, 0.0f)
                                                                                 .Normalize(),
                                                                             ce::ceDeg2Rad(-135.0f)))
  );

  world->Attach(sunEntity);


  ce::Entity *cameraEntity = new ce::Entity("Camera");
  ce::CameraState *cameraState = new ce::CameraState();
  CameraHandler *cameraHandler = new CameraHandler();
  cameraEntity->Attach(cameraState);
  cameraEntity->Attach(cameraHandler);
  cameraEntity->GetRoot()->GetTransform()
              .SetTranslation(ce::Vector3f(20, 20, 20))
              .LookAt(ce::Vector3f(0, 0, 0))
              .Finish();
  world->Attach(cameraEntity);


  ce::Entity *mirrorCameraEntity = new ce::Entity("MirrorCamera");
  ce::CameraState *mirrorCameraState = new ce::CameraState();
  MirrorHandler *mirrorHandler = new MirrorHandler();
  mirrorCameraEntity->Attach(mirrorCameraState);
  mirrorCameraEntity->Attach(mirrorHandler);
  mirrorCameraState->SetRenderShadows(true);
  mirrorHandler->SetCameraState(cameraState);
//  world->Attach(mirrorCameraEntity);

  auto mirrorRenderTarget = create_render_target(device, width / 2, height / 2, 1);
  mirrorCameraState->SetRenderTarget(mirrorRenderTarget);
//  materialMirror->Set(materialMirror->IndexOf("Mirror"), mirrorRenderTarget->GetColorTexture(0));



  auto renderTarget = create_render_target(device, width, height, multiSamples);
  auto colorTexture = renderTarget->GetColorTexture(0);

  ce::iFrameRenderer *frameRenderer = ce::ObjectRegistry::Get<ce::iFrameRenderer>();

  std::string title = ce::Settings("display.cef").GetText("title");
  float rot = 0.0f;
  float entRot = 0.0f;

  uint32_t nextSec = SDL_GetTicks() + 1000;
  uint32_t frames = 0;
  uint32_t lastTime = SDL_GetTicks();


  bool useCs = true;
  bool anim = true;
  float roughness = 1.0f;
  materialInstance->Set(2, roughness);


  ce::iPhysicsSystem *physics = ce::ObjectRegistry::Get<ce::iPhysicsSystem>();
  ce::iPhysicsWorld *physWorld = physics->CreateWorld();


  // add the ground plane
  ce::BoxShapeDesc floorDesc{ce::Vector3f(100.0f, 1.0f, 100.0f)};
  ce::iCollisionShape *floorShape = physics->CreateShape(floorDesc);
  ce::iStaticCollider *floorCollider = physics->CreateStaticCollider();
  floorCollider->Attach(floorShape);
  floorCollider->SetTransform(ce::Matrix4f::Translation(0.0f, -1.0f, 0.0f));
  physWorld->AddCollider(floorCollider);

  sphereRadius = 0.5f;
  renderMeshSphere = create_sphere_mesh(sphereRadius, 32, 4.0f);
  for (int i = 0; i < 10; i++)
  {
    {
      ce::Mesh *meshSphere = new ce::Mesh();
      ce::Entity *entitySphere = new ce::Entity("Sphere");
      ce::StaticMeshState *meshStateSphere = new ce::StaticMeshState("Mesh.Sphere");
      meshSphere->AddMaterialSlot("Default", materialInstance);
      meshSphere->AddSubMesh(renderMeshSphere, 0);
      meshStateSphere->GetTransform()
                     .SetTranslation(ce::Vector3f(0.0f, sphereRadius * 2.5f, 0.0f) * ((float) i + 2.0f))
                     .Finish();
      meshStateSphere->SetMesh(meshSphere);
      entitySphere->Attach(meshStateSphere);
      world->Attach(entitySphere);

      ce::SphereShapeDesc sphereDesc{sphereRadius};
      ce::iCollisionShape *sphereShape = physics->CreateShape(sphereDesc);
      ce::iDynamicCollider *sphereCollider = physics->CreateDynamicCollider();
      sphereCollider->Attach(sphereShape);
      sphereCollider->SetTransform(entitySphere->GetRoot()->GetGlobalMatrix());
      sphereCollider->SetUserData(meshStateSphere);
      physWorld->AddCollider(sphereCollider);
    }
    {
      ce::Mesh *meshSphere = new ce::Mesh();
      ce::Entity *entitySphere = new ce::Entity("Sphere");
      ce::StaticMeshState *meshStateSphere = new ce::StaticMeshState("Mesh.Sphere");
      meshSphere->AddMaterialSlot("Default", materialInstance);
      meshSphere->AddSubMesh(renderMeshSphere, 0);
      meshStateSphere->GetTransform()
                     .SetTranslation(ce::Vector3f(i * sphereRadius, 0.0f, 0.0f))
                     .Finish();
      meshStateSphere->SetMesh(meshSphere);
      entitySphere->Attach(meshStateSphere);
      world->Attach(entitySphere);
    }
    {
      ce::Mesh *meshSphere = new ce::Mesh();
      ce::Entity *entitySphere = new ce::Entity("Sphere");
      ce::StaticMeshState *meshStateSphere = new ce::StaticMeshState("Mesh.Sphere");
      meshSphere->AddMaterialSlot("Default", materialInstance);
      meshSphere->AddSubMesh(renderMeshSphere, 0);
      meshStateSphere->GetTransform()
                     .SetTranslation(ce::Vector3f(0.0f, i * sphereRadius, 0.0f))
                     .Finish();
      meshStateSphere->SetMesh(meshSphere);
      entitySphere->Attach(meshStateSphere);
      world->Attach(entitySphere);
    }
    {
      ce::Mesh *meshSphere = new ce::Mesh();
      ce::Entity *entitySphere = new ce::Entity("Sphere");
      ce::StaticMeshState *meshStateSphere = new ce::StaticMeshState("Mesh.Sphere");
      meshSphere->AddMaterialSlot("Default", materialInstance);
      meshSphere->AddSubMesh(renderMeshSphere, 0);
      meshStateSphere->GetTransform()
                     .SetTranslation(ce::Vector3f(0.0f, 0.0f, i * 2.0f *  sphereRadius))
                     .Finish();
      meshStateSphere->SetMesh(meshSphere);
      entitySphere->Attach(meshStateSphere);
      world->Attach(entitySphere);
    }
  }

  ce::Matrix4f proj;

  device->GetPerspectiveProjection(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1024.0f, proj);
  proj.Debug("ProjectionMatrix");

#if _DEBUG
  ce::Size numDrawCallsPerSec = 0;
  ce::Size numTrianglesPerSec = 0;
  ce::Size numShaderStateChanges = 0;
#endif
  while (true)
  {
#if _DEBUG
    device->ResetDebug();
#endif
    uint32_t time = SDL_GetTicks();
    if (time > nextSec)
    {
      nextSec += 1000;
      char buffer[1024];
#if _DEBUG
      sprintf_s<1024>(buffer, "%s  %d FPS  #%llu calls (%llu triangles) %.2f shader changes", title.c_str(), frames, numDrawCallsPerSec, numTrianglesPerSec, (float)numShaderStateChanges / frames);
      numDrawCallsPerSec = 0;
      numTrianglesPerSec = 0;
      numShaderStateChanges = 0;
#else
      sprintf_s<1024>(buffer, "%s  %d FPS [%f]", title.c_str(), frames, roughness);
#endif
      SDL_SetWindowTitle(wnd, buffer);
      printf("%s\n", buffer);
      fflush(stdout);
      frames = 0;

    }
    else
    {
      frames++;
    }
    uint32_t deltaTime = time - lastTime;
    lastTime = time;

//    SDL_GL_MakeCurrent(wnd, context);
    UpdateEvents();

    if (ce::Input::IsKeyPressed(ce::Key::eK_Escape))
    {
      break;
    }

    if (ce::Input::IsKeyPressed(ce::Key::eK_A))
    {
      anim = !anim;
    }
    if (ce::Input::IsKeyPressed(ce::Key::eK_Space))
    {
      useCs = !useCs;
    }
    if (deltaTime != 0)
    {
      float tpf = (float) deltaTime / 1000.0f;

      if (anim)
      {
        entRot += tpf * 1.0f;
      }

      if (ce::Input::IsKeyDown(ce::Key::eK_Up))
      {
        roughness += 0.5f * tpf;
        roughness = roughness <= 10.0f ? roughness : 10.0f;
        materialInstance->Set(2, roughness);
      }
      if (ce::Input::IsKeyDown(ce::Key::eK_Down))
      {
        roughness -= 0.5f * tpf;
        roughness = roughness >= 0.0f ? roughness : 0.0f;
        materialInstance->Set(2, roughness);
      }

      sphereRadius = 0.0f;
      float dist = 10.0f;
//      ce::SpatialState *cameraState = cameraEntity->GetRoot();
//      cameraState->GetTransform()
//                 .SetTranslation(ce::Vector3f(ce::ceCos(entRot + (float) M_PI / 2.0f + 0.2f) * dist,
//                                               dist + sphereRadius * 1.5f,
//                                               ce::ceSin(entRot + (float) M_PI / 2.0f + 0.2f) * dist
//                 ))
//                 .LookAt(ce::Vector3f(0.0f, sphereRadius * 1.5f, 0.0f))
//                 .Finish();


      terrainMesh->SetReferencePoint(cameraState->GetTransform().GetTranslation());
      world->Update(tpf);
    }


    frameRenderer->Render(renderTarget, device, world->GetScene());

    device->SetRenderTarget(nullptr);
    device->SetViewport(0, 0, wnd_width, wnd_height);
    device->SetDepthTest(false);
    device->RenderFullscreen(colorTexture);
    device->SetDepthTest(true);


#if _DEBUG
    numDrawCallsPerSec += device->GetNumberOfDrawCalls();
    numTrianglesPerSec += device->GetNumberOfTriangles();
    numShaderStateChanges += device->GetNumberOfShaderStateChanges();
#endif

    SDL_GL_SwapWindow(wnd);

    physWorld->Step(1.0f / 60.0f);
    const std::vector<ce::iPhysicsWorld::DynamicResult> &result = physWorld->SwapResult();
    for (auto &res: result)
    {
      res.Collider->GetUserData()->SetLocalMatrix(res.Matrix);
    }
//    break;
  }

  ce::iMouse *mouse = ce::Input::GetMouse();
  mouse->SetCursorMode(ce::eCursorMode::Free);
  mouse->SetVisible(true);

  return 0;
}
