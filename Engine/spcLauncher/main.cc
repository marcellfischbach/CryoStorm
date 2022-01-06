
#include <spcLauncher/window/sdlinputsystem.hh>
#include <spcLauncher/window/sdlkeyboard.hh>
#include <spcLauncher/window/sdlmouse.hh>
#include <spcLauncher/launchermodule.hh>
#include <spcCore/coremodule.hh>
#include <spcCore/settings.hh>
#include <spcCore/entity/camerastate.hh>
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
#include <spcCore/graphics/iframerenderer.hh>
#include <spcCore/graphics/image.hh>
#include <spcCore/graphics/ipointlight.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/graphics/irenderpipeline.hh>
#include <spcCore/graphics/irendertarget2d.hh>
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
#include <spcCore/resource/resourcelocator.hh>

#include <spcAssimpLoader/assimploadermodule.hh>
#include <spcOpenGL/openglmodule.hh>
#include <GL/glew.h>

#include <spcImgLoader/imgloadermodule.hh>

#include <spcLauncher/camerahandler.hh>

#include <iostream>
#include <SDL.h>
#include <regex>
#include <string>

spc::SDLKeyboard keyboard;
spc::SDLMouse    mouse;


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
  size_t                   offset = 0;
  size_t                   idx    = 0;
  while ((idx                   = string.find('\n', offset)) != std::string::npos)
  {
    std::string part = string.substr(offset, idx - offset);
    res.push_back(part);
    offset = idx + 1;
  }
  std::string              part = string.substr(offset, string.length() - offset);
  res.push_back(part);

  return res;
}

std::string merge(const std::vector<std::string> &lines)
{
  std::string            res;
  for (const std::string &str: lines)
  {
    res += str + "\n";
  }
  return res;
}

bool register_modules(int argc, char **argv)
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

SDL_Window    *wnd;
SDL_GLContext context;


bool initialize_modules(int argc, char **argv)
{
  spc::VFS::Get()->SetBasePath("D:\\DEV\\SpiceEngine\\data");
  spc::Settings settings("file:///config/display.spc");

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

  uint32_t      flags    = SDL_WINDOW_OPENGL;
  // flags |= SDL_WINDOW_BORDERLESS;
  std::string   title    = settings.GetText("title");
  spc::Vector2i res      = settings.GetVector2i("resolution");
  spc::Vector2i pos      = settings.GetVector2i("pos");
  std::string   viewMode = settings.GetText("viewmode", "windowed");
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
  wnd     = SDL_CreateWindow(title.c_str(),
                             pos.x, pos.y,
                             res.x, res.y, flags
  );
  //  wnd = SDL_CreateWindow("Spice", 0, 0, 1920, 1080, flags);
  context = SDL_GL_CreateContext(wnd);
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

spc::iRenderMesh *create_plane_mesh(float size, float nx, float ny)
{
  //
  // create a render mesh
  spc::iRenderMeshGenerator  *generator = spc::ObjectRegistry::Get<spc::iRenderMeshGeneratorFactory>()->Create();
  std::vector<spc::Vector3f> positions;
  positions.push_back(spc::Vector3f(-size, 0.0f, -size));
  positions.push_back(spc::Vector3f(-size, 0.0f, size));
  positions.push_back(spc::Vector3f(size, 0.0f, -size));
  positions.push_back(spc::Vector3f(size, 0.0f, size));
  std::vector<spc::Vector3f> normals;
  normals.push_back(spc::Vector3f(0.0f, 1.0f, 0.0f));
  normals.push_back(spc::Vector3f(0.0f, 1.0f, 0.0f));
  normals.push_back(spc::Vector3f(0.0f, 1.0f, 0.0f));
  normals.push_back(spc::Vector3f(0.0f, 1.0f, 0.0f));
  std::vector<spc::Vector2f> uv;
  uv.push_back(spc::Vector2f(0.0f, 0.0f));
  uv.push_back(spc::Vector2f(0.0f, ny));
  uv.push_back(spc::Vector2f(nx, 0.0f));
  uv.push_back(spc::Vector2f(nx, ny));
  std::vector<spc::uint32_t> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(3);
  indices.push_back(0);
  indices.push_back(3);
  indices.push_back(2);
  std::vector<spc::Color4f> colors;
  spc::Color4f              color(1.0f, 1.0f, 1.0f, 1.0f);
  colors.push_back(color);
  colors.push_back(color);
  colors.push_back(color);
  colors.push_back(color);
  generator->SetVertices(positions);
  generator->SetNormals(normals);
  generator->SetIndices(indices);
  generator->SetColors(colors);
  generator->SetUV0(uv);
  spc::iRenderMesh *renderMesh = generator->Generate();
  generator->Release();
  return renderMesh;
}

spc::iRenderMesh *create_sphere_mesh(float radius, uint32_t detail, float uv_f)
{
  spc::iRenderMeshGenerator  *generator = spc::ObjectRegistry::Get<spc::iRenderMeshGeneratorFactory>()->Create();
  std::vector<spc::Vector3f> positions;
  std::vector<spc::Vector3f> normals;
  std::vector<spc::Vector2f> uv;
  std::vector<spc::Color4f>  colors;
  std::vector<spc::uint32_t> indices;

  for (uint32_t v = 0; v < detail; v++)
  {
    float factV  = (float) v / (float) (detail - 1);
    float angleV = -(float) M_PI_2 + factV * (float) M_PI;

    for (uint32_t h = 0; h < detail * 2; h++)
    {
      float factH  = (float) h / (float) (detail * 2 - 1);
      float angleH = factH * (float) M_PI * 2.0f;

      spc::Vector3f normal(
          cosf(angleV) * cosf(angleH),
          sinf(angleV),
          cosf(angleV) * sinf(angleH)
      );
      positions.push_back(normal * radius);
      normals.emplace_back(normal);
      uv.emplace_back(spc::Vector2f(factH * 2.0f * uv_f, factV * uv_f));
      colors.emplace_back(spc::Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    }
  }

  for (uint32_t v = 0; v < detail - 1; v++)
  {
    uint32_t      i0 = v * detail * 2;
    uint32_t      i1 = i0 + detail * 2;
    for (uint32_t h  = 0; h < detail * 2 - 1; h++)
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
  spc::iRenderMesh *renderMesh = generator->Generate();
  generator->Release();
  return renderMesh;

}

void debug(spc::SpatialState *state, int indent)
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


void create_suzannes_plain(spc::Mesh *suzanneMesh, spc::World *world, spc::iMaterial *alternativeMaterial)
{
  size_t      num = 20;
  for (size_t i   = 0; i < num; i++)
  {
    float       x = -40.0f + (float) i / (float) num * 80.0f;
    for (size_t j = 0; j < num; j++)
    {
      float y = -40.0f + (float) j / (float) num * 80.0f;

      spc::Entity          *suzanneEntity = new spc::Entity("Entity1");
      spc::StaticMeshState *meshState1    = new spc::StaticMeshState("StaticMesh1");
      meshState1->GetTransform()
                .SetTranslation(spc::Vector3f(x, 0, y))
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


void create_suzanne_batch(spc::Mesh *suzanneMesh,
                          int a,
                          int b,
                          size_t numI,
                          size_t numJ,
                          size_t maxI,
                          size_t maxJ,
                          spc::World *world
                         )
{
  auto generator = spc::ObjectRegistry::Get<spc::iRenderMeshBatchGeneratorFactory>()->Create();

  for (size_t ni = 0, i = a * numI; ni < numI; ni++, i++)
  {
    float       x  = -40.0f + (float) i / (float) maxI * 80.0f;
    for (size_t nj = 0, j = b * numJ; nj < numJ; nj++, j++)
    {
      float         y = -40.0f + (float) j / (float) maxJ * 80.0f;
      spc::Matrix4f mat;
      mat.SetTranslation(spc::Vector3f(x, 0, y));
      generator->Add(suzanneMesh->GetSubMesh(0).GetMesh(), mat);
    }
  }

  spc::iRenderMesh *batchedRM = generator->Generate();

  spc::Mesh   *suzyMesh = new spc::Mesh();
  for (size_t i         = 0; i < suzanneMesh->GetNumberOfMaterialSlots(); i++)
  {
    suzyMesh->AddMaterialSlot(suzanneMesh->GetMaterialSlot(i).GetName(),
                              suzanneMesh->GetMaterialSlot(i).GetDefaultMaterial());
  }
  suzyMesh->AddSubMesh(batchedRM, 0);

  spc::Entity          *suzanneEntity = new spc::Entity("Entity1");
  spc::StaticMeshState *meshState1    = new spc::StaticMeshState("StaticMesh1");
  meshState1->GetTransform()
            .SetTranslation(spc::Vector3f(0, 0, 0))
            .Finish();
  meshState1->SetMesh(suzyMesh);
  meshState1->SetStatic(true);
  suzanneEntity->Attach(meshState1);
  world->Attach(suzanneEntity);

  generator->Release();
}

void create_suzannes_batched(spc::Mesh *suzanneMesh, spc::World *world)
{
  for (int a = 0; a < 10; a++)
  {
    for (int b = 0; b < 10; b++)
    {
      create_suzanne_batch(suzanneMesh, a, b, 2, 2, 20, 20, world);
    }
  }
}

spc::iRenderTarget2D *create_render_target(spc::iDevice *device, uint32_t width, uint32_t height)
{
  spc::iSampler *colorSampler = device->CreateSampler();
  colorSampler->SetFilterMode(spc::eFM_MinMagNearest);

  spc::iSampler *depthSampler = device->CreateSampler();
  depthSampler->SetFilterMode(spc::eFM_MinMagNearest);

  spc::iTexture2D::Descriptor rt_col_desc = {};
  rt_col_desc.Width   = width;
  rt_col_desc.Height  = height;
  rt_col_desc.Format  = spc::ePF_RGBA;
  rt_col_desc.MipMaps = false;
  spc::iTexture2D *color_texture = device->CreateTexture(rt_col_desc);
  color_texture->SetSampler(colorSampler);

  spc::iTexture2D::Descriptor rt_dpth_desc = {};
  rt_dpth_desc.Width   = width;
  rt_dpth_desc.Height  = height;
  rt_dpth_desc.Format  = spc::ePF_Depth;
  rt_dpth_desc.MipMaps = false;
  spc::iTexture2D *depth_texture = device->CreateTexture(rt_dpth_desc);
  depth_texture->SetSampler(depthSampler);


  spc::iRenderTarget2D::Descriptor rt_desc = {};
  rt_desc.Width  = width;
  rt_desc.Height = height;

  spc::iRenderTarget2D *renderTarget = device->CreateRenderTarget(rt_desc);
  renderTarget->AddColorTexture(color_texture);
  renderTarget->SetDepthBuffer(spc::ePF_Depth);
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

  spc::iDevice *device = spc::ObjectRegistry::Get<spc::iDevice>();


  spc::Material         *transMaterial     = spc::AssetManager::Get()->Get<spc::Material>(spc::ResourceLocator(
      "/materials/test_trans_material.spc"
  ));
  spc::MaterialInstance *transRedMaterial  = spc::AssetManager::Get()->Get<spc::MaterialInstance>(spc::ResourceLocator(
      "/materials/red_transparent_material_instance.spc"
  ));
  spc::MaterialInstance *transBlueMaterial = spc::AssetManager::Get()->Get<spc::MaterialInstance>(spc::ResourceLocator(
      "/materials/blue_transparent_material_instance.spc"
  ));


  spc::MaterialInstance *materialInstance = spc::AssetManager::Get()->Get<spc::MaterialInstance>(spc::ResourceLocator(
      "/materials/test_material_instance.spc"
  ));

  spc::MaterialInstance *materialInstance2 = spc::AssetManager::Get()->Get<spc::MaterialInstance>(spc::ResourceLocator(
      "/materials/test_material2_instance.spc"
  ));


  spc::iRenderMesh *renderMesh = create_plane_mesh(40.0f, 8, 8);
  spc::Mesh        *mesh       = new spc::Mesh();
  mesh->AddMaterialSlot("Default", materialInstance);
  mesh->AddSubMesh(renderMesh, 0);

  spc::iRenderMesh *transPlaneMesh = create_plane_mesh(10.0f, 8, 8);
  spc::Mesh        *transRedMesh   = new spc::Mesh();
  transRedMesh->AddMaterialSlot("Default", transRedMaterial);
  transRedMesh->AddSubMesh(transPlaneMesh, 0);
  spc::Mesh *transBlueMesh = new spc::Mesh();
  transBlueMesh->AddMaterialSlot("Default", transBlueMaterial);
  transBlueMesh->AddSubMesh(transPlaneMesh, 0);


  spc::World *world        = new spc::World();

  int wnd_width, wnd_height;
  SDL_GetWindowSize(wnd, &wnd_width, &wnd_height);

  spc::Settings settings("file:///config/display.spc");
  spc::Vector2i resolution = settings.GetVector2i("resolution", spc::Vector2i(wnd_width, wnd_height));
  int           width      = resolution.x;
  int           height     = resolution.y;

  float aspect = (float) wnd_height / (float) wnd_width;


  spc::Mesh *suzanneMesh = spc::AssetManager::Get()->Load<spc::Mesh>(spc::ResourceLocator("file:///suzanne.fbx"));
  spc::Mesh *cube        = spc::AssetManager::Get()->Load<spc::Mesh>(spc::ResourceLocator("cube.fbx"));
  suzanneMesh->SetDefaultMaterial(0, materialInstance);
  cube->SetDefaultMaterial(0, materialInstance);

  spc::Entity          *entity0    = new spc::Entity("Entity0");
  spc::StaticMeshState *meshState0 = new spc::StaticMeshState("StaticMesh0");

  meshState0->GetTransform()
            .SetTranslation(spc::Vector3f(0, 0, 0))
            .Finish();
  meshState0->SetMesh(mesh);
  meshState0->SetStatic(true);
  entity0->Attach(meshState0);
  world->Attach(entity0);


  spc::Entity          *entityTransPlaneRed    = new spc::Entity("Entity0");
  spc::StaticMeshState *meshStateTransPlaneRed = new spc::StaticMeshState("StaticMeshTransPlane");

  meshStateTransPlaneRed->GetTransform()
                        .SetTranslation(spc::Vector3f(0, 2.0f, 0))
                        .Finish();
  meshStateTransPlaneRed->SetMesh(transRedMesh);
  meshStateTransPlaneRed->SetStatic(true);
  entityTransPlaneRed->Attach(meshStateTransPlaneRed);

  spc::Entity          *entityTransPlaneBlue    = new spc::Entity("Entity0");
  spc::StaticMeshState *meshStateTransPlaneBlue = new spc::StaticMeshState("StaticMeshTransPlane");
  meshStateTransPlaneBlue->GetTransform()
                         .SetTranslation(spc::Vector3f(0, 0.10f, 0))
                         .Finish();
  meshStateTransPlaneBlue->SetMesh(transBlueMesh);
  meshStateTransPlaneBlue->SetStatic(true);
  entityTransPlaneBlue->Attach(meshStateTransPlaneBlue);

  float                sphereRadius      = 4.0f;
  spc::iRenderMesh     *renderMeshSphere = create_sphere_mesh(sphereRadius, 32, 4.0f);
  spc::Mesh            *meshSphere       = new spc::Mesh();
  spc::Entity          *entitySphere     = new spc::Entity("Sphere");
  spc::StaticMeshState *meshStateSphere  = new spc::StaticMeshState("Mesh.Sphere");
  meshSphere->AddMaterialSlot("Default", materialInstance);
  meshSphere->AddSubMesh(renderMeshSphere, 0);
  meshStateSphere->GetTransform()
                 .SetTranslation(spc::Vector3f(0.0f, sphereRadius * 1.5f, 0.0f))
                 .Finish();
  meshStateSphere->SetMesh(meshSphere);
  entitySphere->Attach(meshStateSphere);
//  world->Attach(entitySphere);

//  world->Attach(entityTransPlaneBlue);


  create_suzannes_plain(suzanneMesh, world, materialInstance2);
//  create_suzannes_batched(suzanneMesh, world);

  spc::Entity     *lightEntity = new spc::Entity("Light_0");
  spc::LightState *lightState  = new spc::LightState("LightState");
  lightEntity->Attach(lightState);
  lightState->SetType(spc::eLT_Point);
  lightState->SetColor(spc::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 1.0f);
  lightState->SetRange(50);
  lightState->SetStatic(true);
  lightState->SetCastShadow(false);
  lightState->GetTransform()
            .SetTranslation(spc::Vector3f(5.0f, 5.0f, 5.0f))
            .Finish();
//  world->Attach(lightEntity);

  /*
  lightEntity = new spc::Entity("Light_0");
  lightState = new spc::LightState("LightState");
  lightEntity->Attach(lightState);
  lightState->SetType(spc::eLT_Point);
  lightState->SetColor(spc::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 0.5f);
  lightState->SetRange(25.0f);
  lightState->SetStatic(true);
  lightState->SetCastShadow(true);
  lightEntity->GetRoot()->GetTransform()
    .SetTranslation(spc::Vector3f(5.0f, 5.0f, 5.0f))
    .Finish();
  world->Attach(lightEntity);
  */

  spc::Entity     *sunEntity     = new spc::Entity("Sun");
  spc::LightState *sunLightState = new spc::LightState("SunLight");
  sunEntity->Attach(sunLightState);
  sunLightState->SetType(spc::eLT_Directional);
  sunLightState->SetColor(spc::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 1.0f);
  sunLightState->SetSplits(25.0f, 50.0f, 100.0f);
  sunLightState->SetShadowMapBias(0.003f);
  sunLightState->SetStatic(true);
  sunLightState->SetCastShadow(true);
  sunLightState->SetTransform(sunLightState->GetTransform()
                                               //.SetRotation(spc::Quaternion::FromAxisAngle(spc::Vector3f(1.0f, 0.0f, 0.0f), spc::spcDeg2Rad(-45.0f)))
                                           .SetRotation(
                                               spc::Quaternion::FromAxisAngle(spc::Vector3f(1.0f, 0.0f, 0.0f)
                                                                                  .Normalize(),
                                                                              spc::spcDeg2Rad(-45.0f)))
  );

  world->Attach(sunEntity);

  sunEntity     = new spc::Entity("Sun");
  sunLightState = new spc::LightState("SunLight");
  sunEntity->Attach(sunLightState);
  sunLightState->SetType(spc::eLT_Directional);
  sunLightState->SetColor(spc::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 0.2f);
  sunLightState->SetSplits(25.0f, 50.0f, 100.0f);
  sunLightState->SetShadowMapBias(0.003f);
  sunLightState->SetStatic(true);
  sunLightState->SetCastShadow(false);
  sunLightState->SetTransform(sunLightState->GetTransform()
                                               //.SetRotation(spc::Quaternion::FromAxisAngle(spc::Vector3f(1.0f, 0.0f, 0.0f), spc::spcDeg2Rad(-45.0f)))
                                           .SetRotation(
                                               spc::Quaternion::FromAxisAngle(spc::Vector3f(1.0f, 0.0f, 0.0f)
                                                                                  .Normalize(),
                                                                              spc::spcDeg2Rad(-135.0f)))
  );

//  world->Attach(sunEntity);


  spc::Entity      *cameraEntity  = new spc::Entity("Camera");
  spc::CameraState *cameraState   = new spc::CameraState();
  CameraHandler    *cameraHandler = new CameraHandler();
  cameraEntity->Attach(cameraState);
  cameraEntity->Attach(cameraHandler);
  cameraEntity->GetRoot()->GetTransform()
              .SetTranslation(spc::Vector3f(20, 20, 20))
              .LookAt(spc::Vector3f(0, 0, 0))
              .Finish();
  world->Attach(cameraEntity);


  auto renderTarget = create_render_target(device, width, height);
  auto colorTexture = renderTarget->GetColorTexture(0);

  spc::iFrameRenderer *frameRenderer = spc::ObjectRegistry::Get<spc::iFrameRenderer>();

  std::string title  = spc::Settings("display.spc").GetText("title");
  float       rot    = 0.0f;
  float       entRot = 0.0f;

  spc::uint32_t nextSec  = SDL_GetTicks() + 1000;
  spc::uint32_t frames   = 0;
  spc::uint32_t lastTime = SDL_GetTicks();


  bool  useCs     = true;
  bool  anim      = true;
  float roughness = 1.0f;
  materialInstance->Set(2, roughness);
#if _DEBUG
  spc::Size numDrawCallsPerSec = 0;
  spc::Size numTrianglesPerSec = 0;
  spc::Size numShaderStateChanges = 0;
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
      sprintf_s<1024>(buffer, "%s  %d FPS", title.c_str(), frames);
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
    spc::uint32_t deltaTime = time - lastTime;
    lastTime = time;

//    SDL_GL_MakeCurrent(wnd, context);
    UpdateEvents();

    if (spc::Input::IsKeyPressed(spc::Key::eK_Escape))
    {
      break;
    }

    if (spc::Input::IsKeyPressed(spc::Key::eK_A))
    {
      anim = !anim;
    }
    if (spc::Input::IsKeyPressed(spc::Key::eK_Space))
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

      if (spc::Input::IsKeyDown(spc::Key::eK_Up))
      {
        roughness += 0.5f * tpf;
        roughness = roughness <= 10.0f ? roughness : 10.0f;
        materialInstance->Set(2, roughness);
      }
      if (spc::Input::IsKeyDown(spc::Key::eK_Down))
      {
        roughness -= 0.5f * tpf;
        roughness = roughness >= 0.0f ? roughness : 0.0f;
        materialInstance->Set(2, roughness);
      }

      sphereRadius = 0.0f;
      float dist = 10.0f;
//      spc::SpatialState *cameraState = cameraEntity->GetRoot();
//      cameraState->GetTransform()
//                 .SetTranslation(spc::Vector3f(spc::spcCos(entRot + (float) M_PI / 2.0f + 0.2f) * dist,
//                                               dist + sphereRadius * 1.5f,
//                                               spc::spcSin(entRot + (float) M_PI / 2.0f + 0.2f) * dist
//                 ))
//                 .LookAt(spc::Vector3f(0.0f, sphereRadius * 1.5f, 0.0f))
//                 .Finish();


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

  }


  return 0;
}

