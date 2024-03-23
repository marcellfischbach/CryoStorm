
#include <ceLauncher/window/sdlinputsystem.hh>
#include <ceLauncher/window/sdlkeyboard.hh>
#include <ceLauncher/window/sdlmouse.hh>
#include <ceLauncher/launchermodule.hh>
#include <ceBullet/bulletmodule.hh>
#include <ceCore/coremodule.hh>
#include <ceCore/settings.hh>
#include <ceCore/entity/camerastate.hh>
#include <ceCore/entity/collisionstate.hh>
#include <ceCore/entity/entity.hh>
#include <ceCore/entity/entitystate.hh>
#include <ceCore/entity/lightstate.hh>
#include <ceCore/entity/rigidbodystate.hh>
#include <ceCore/entity/skeletonmeshstate.hh>
#include <ceCore/entity/spatialstate.hh>
#include <ceCore/entity/staticcolliderstate.hh>
#include <ceCore/entity/staticmeshstate.hh>
#include <ceCore/entity/terrainmeshstate.hh>
#include <ceCore/entity/world.hh>
#include <ceCore/input/input.hh>
#include <ceCore/math/math.hh>
#include <ceCore/math/color4f.hh>
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
#include <ceCore/graphics/skeletonmesh.hh>
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
#include <ceLauncher/testhandler.hh>

#include <iostream>
#include <SDL.h>
#include <regex>
#include <string>
#include <ceCore/time.hh>

ce::SDLKeyboard keyboard;

ce::SDLMouse mouse;

ce::LightState *shadowLightState = nullptr;


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
  std::string basePath("../data");
  for (int    i = 0; i < argc; i++)
  {
    std::string arg(argv[i]);
    if (arg == std::string("--data") && i + 1 < argc)
    {
      basePath = std::string(argv[++i]);
    }
  }
  printf("Starting with base-path: '%s'\n", basePath.c_str());
  ce::VFS::Get()->SetBasePath(basePath);
  ce::Settings settings("file:///${config}/display.config");

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

  uint32_t     flags    = SDL_WINDOW_OPENGL;
  // flags |= SDL_WINDOW_BORDERLESS;
  std::string  title    = settings.GetText("title");
  ce::Vector2i res      = settings.GetVector2i("resolution");
  ce::Vector2i pos      = settings.GetVector2i("pos");
  std::string  viewMode = settings.GetText("viewmode", "windowed");
  if (viewMode == "fullscreen")
  {
    flags |= SDL_WINDOW_FULLSCREEN;
  }
  else if (viewMode == "desktop")
  {
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  }
  else if (viewMode == "windowed")
  {
    flags |= SDL_WINDOW_BORDERLESS;
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
  auto image = ce::AssetManager::Get()->Load<ce::Image>("file:///icons/ce24.png");
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

ce::iTerrainMesh *create_terrain_mesh(float size)
{
#define FLAT
  ce::iTerrainMeshGenerator *generator = ce::ObjectRegistry::Get<ce::iTerrainMeshGeneratorFactory>()->Create();

  std::vector<float> heightData;
  for (int           i = 0; i < 1025; i++)
  {
#ifndef FLAT
    float    fi = (float) i / 1024.0f;
#endif
    for (int j = 0; j < 1025; j++)
    {
#ifndef FLAT
      float fj = (float) j / 1024.0f;
#endif

#ifndef FLAT
      float a = sin(fi * 10.0f) * cos(fj * 10.0f);
      heightData.push_back(0.5f + 0.5f * a);
#else
      heightData.push_back(0.0f);
#endif
    }
  }

  generator->SetSize(ce::eTerrainSize::TS_129);
  generator->SetPatchSize(ce::eTerrainSize::TS_33);
  generator->SetSize(ce::eTerrainSize::TS_1025);
  generator->SetPatchSize(ce::eTerrainSize::TS_129);
  generator->SetNormalizedHeightData(heightData);
  generator->SetSize(ce::Vector3f(-size, 0.0f, -size), ce::Vector3f(size, 10.0f, size));

  ce::iTerrainMesh *terrainMesh = generator->Generate();
  generator->Release();
  return terrainMesh;
}

ce::iRenderMesh *create_sphere_mesh(float radius, uint32_t detail, float uv_f)
{
  ce::iRenderMeshGenerator  *generator = ce::ObjectRegistry::Get<ce::iRenderMeshGeneratorFactory>()->Create();
  std::vector<ce::Vector3f> positions;
  std::vector<ce::Vector3f> normals;
  std::vector<ce::Vector3f> tangents;
  std::vector<ce::Vector2f> uv;
  std::vector<ce::Color4f>  colors;
  std::vector<uint32_t>     indices;

  for (uint32_t v = 0; v < detail; v++)
  {
    float factV  = (float) v / (float) (detail - 1);
    float angleV = -(float) M_PI_2 + factV * (float) M_PI;

    for (uint32_t h = 0; h < detail * 2; h++)
    {
      float factH  = (float) h / (float) (detail * 2 - 1);
      float angleH = factH * (float) M_PI * 2.0f;

      ce::Vector3f normal(
          cosf(angleV) * cosf(angleH),
          sinf(angleV),
          cosf(angleV) * sinf(angleH)
      );
      ce::Vector3f tangent(
          cosf(angleH + (float) M_PI / 2.0f),
          0.0f,
          sinf(angleH + (float) M_PI / 2.0f)
      );
      positions.push_back(normal * radius);
      normals.emplace_back(normal);
      tangents.emplace_back(tangent);
      uv.emplace_back(factH * 2.0f * uv_f, factV * uv_f);
      colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
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
  generator->SetTangents(tangents);
  generator->SetColors(colors);
  generator->SetUV0(uv);
  generator->SetIndices(indices);
  ce::iRenderMesh *renderMesh = generator->Generate();
  generator->Release();
  return renderMesh;

}


ce::iRenderMesh *
create_multi_sphere_mesh(float radius, uint32_t detail, float uv_f, size_t num_spheres, ce::Vector3f *sphere_positions)
{
  ce::iRenderMeshGenerator  *generator = ce::ObjectRegistry::Get<ce::iRenderMeshGeneratorFactory>()->Create();
  std::vector<ce::Vector3f> positions;
  std::vector<ce::Vector3f> normals;
  std::vector<ce::Vector3f> tangents;
  std::vector<ce::Vector2f> uv;
  std::vector<ce::Color4f>  colors;
  std::vector<uint32_t>     indices;

  size_t      idxOrigin = 0;
  for (size_t i         = 0; i < num_spheres; i++)
  {
    ce::Vector3f  origin = sphere_positions[i];
    for (uint32_t v      = 0; v < detail; v++)
    {
      float factV  = (float) v / (float) (detail - 1);
      float angleV = -(float) M_PI_2 + factV * (float) M_PI;

      for (uint32_t h = 0; h < detail * 2; h++)
      {
        float factH  = (float) h / (float) (detail * 2 - 1);
        float angleH = factH * (float) M_PI * 2.0f;

        ce::Vector3f normal(
            cosf(angleV) * cosf(angleH),
            sinf(angleV),
            cosf(angleV) * sinf(angleH)
        );
        ce::Vector3f tangent(
            cosf(angleH + (float) M_PI / 2.0f),
            0.0f,
            sinf(angleH + (float) M_PI / 2.0f)
        );
        positions.push_back(origin + normal * radius);
        normals.emplace_back(normal);
        tangents.emplace_back(tangent);
        uv.emplace_back(factH * 2.0f * uv_f, factV * uv_f);
        colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
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

        indices.emplace_back(idxOrigin + i00);
        indices.emplace_back(idxOrigin + i10);
        indices.emplace_back(idxOrigin + i11);

        indices.emplace_back(idxOrigin + i00);
        indices.emplace_back(idxOrigin + i11);
        indices.emplace_back(idxOrigin + i01);
      }
    }
    idxOrigin += detail * detail * 2;
  }

  generator->SetVertices(positions);
  generator->SetNormals(normals);
  generator->SetTangents(tangents);
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


ce::iRenderTarget2D *create_render_target(ce::iDevice *device, uint32_t width, uint32_t height, uint16_t multiSamples)
{
  ce::iSampler *colorSampler = device->CreateSampler();
  colorSampler->SetFilterMode(ce::eFM_MinMagNearest);

  ce::iSampler *depthSampler = device->CreateSampler();
  depthSampler->SetFilterMode(ce::eFM_MinMagNearest);
  depthSampler->SetTextureCompareFunc(ce::eCF_LessOrEqual);
  depthSampler->SetTextureCompareMode(ce::eTCM_None);

  ce::iTexture2D::Descriptor rt_col_desc = {};
  rt_col_desc.Width        = width;
  rt_col_desc.Height       = height;
  rt_col_desc.Format       = ce::ePF_RGBA;
  rt_col_desc.MipMaps      = false;
  rt_col_desc.MultiSamples = multiSamples;
  ce::iTexture2D *color_texture = device->CreateTexture(rt_col_desc);
  color_texture->SetSampler(colorSampler);

  ce::iTexture2D::Descriptor rt_dpth_desc = {};
  rt_dpth_desc.Width        = width;
  rt_dpth_desc.Height       = height;
  rt_dpth_desc.Format       = ce::ePF_DepthStencil;
  rt_dpth_desc.MipMaps      = false;
  rt_dpth_desc.MultiSamples = multiSamples;
  ce::iTexture2D *depth_texture = device->CreateTexture(rt_dpth_desc);
  depth_texture->SetSampler(depthSampler);
  printf("CreateDepthTexture: %p\n", depth_texture);


  ce::iRenderTarget2D::Descriptor rt_desc = {};
  rt_desc.Width  = width;
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

void generate_camera(ce::World *world)
{
  auto cameraEntity = new ce::Entity("Camera");
  auto cameraState  = new ce::CameraState();

  auto cameraHandler = new CameraHandler();
  cameraEntity->Attach(cameraState);
  cameraEntity->Attach(cameraHandler);
  cameraEntity->GetRoot()->GetTransform()
              .SetTranslation(ce::Vector3f(5, 5, 5))
              .LookAt(ce::Vector3f(0, 0, 0))
              .Finish();
  world->Attach(cameraEntity);
  world->SetMainCamera(cameraState);

//  auto mirrorCameraEntity = new ce::Entity("MirrorCamera");
//  auto mirrorCameraState  = new ce::CameraState();
//  auto mirrorHandler      = new MirrorHandler();
//  mirrorCameraEntity->Attach(mirrorCameraState);
//  mirrorCameraEntity->Attach(mirrorHandler);
//  mirrorCameraState->SetRenderShadows(true);
//  mirrorHandler->SetCameraState(cameraState);
//  world->Attach(mirrorCameraEntity);

//  auto mirrorRenderTarget = create_render_target(device, width / 2, height / 2, 1);
//  mirrorCameraState->SetRenderTarget(mirrorRenderTarget);
//  materialMirror->Set(materialMirror->IndexOf("Mirror"), mirrorRenderTarget->GetColorTexture(0));
}

void generate_terrain(ce::World *world)
{
  ce::AssetManager *assetMan = ce::AssetManager::Get();

  auto greenGrassLayer = assetMan->Get<ce::TerrainLayer>("/terrain/green_grass.terrainlayer");
  auto dirtLayer       = assetMan->Get<ce::TerrainLayer>("/terrain/dirt.terrainlayer");
  auto fieldstoneLayer = assetMan->Get<ce::TerrainLayer>("/terrain/fieldstone.terrainlayer");
  auto terrainLayers   = assetMan->Get<ce::TerrainLayerMask>("/terrain/terrain.terrainmask");

  auto terrainMesh  = create_terrain_mesh(40.0f);
  auto entity0      = new ce::Entity("Terrain");
  auto terrainState = new ce::TerrainMeshState();
  terrainState->SetTerrainMesh(terrainMesh);
  terrainState->SetLayerMask(terrainLayers);
  terrainState->AddLayer(greenGrassLayer);
  terrainState->AddLayer(dirtLayer);
  terrainState->AddLayer(fieldstoneLayer);
  terrainState->GetTransform()
              .SetTranslation(ce::Vector3f(0, 0, 0))
              .Finish();
  terrainState->SetStatic(true);
  entity0->Attach(terrainState);
  world->Attach(entity0);
}


void generate_test_grid(ce::World *world, ce::iMaterial *material)
{
  auto sphere = create_sphere_mesh(0.25, 16, 12.0f);
  auto mesh   = new ce::Mesh();
  mesh->AddMaterialSlot("Default", material);
  mesh->AddSubMesh(sphere, 0);
  int gridSize = 100;

  float start = static_cast<float>(gridSize) / 2.0f;

  for (int a = 0, i = 0; i < gridSize; i++)
  {
    auto     fi = (float) i;
    for (int j  = 0; j < gridSize; j++, a++)
    {
      auto fj     = (float) j;
      auto entity = new ce::Entity(std::string("Sphere: ") + std::to_string(i + 1) + ":" + std::to_string(j + 1));

      auto meshStateSphere = new ce::StaticMeshState("Mesh");
      meshStateSphere->SetStatic(true);
      meshStateSphere->GetTransform()
                     .SetTranslation(i - start, 0.25f, j - start)
                     .Finish();
      meshStateSphere->SetMesh(mesh);
      entity->Attach(meshStateSphere);
#if 0
      auto rnd = (float) rand() / (float) RAND_MAX;
      int   ma  = a % 4;
      switch (ma)
      {
        case 0:
        {

          auto testHandler01 = new TestHandler01(ce::Vector3f(fi - 50.0f, 0.25f, fj - 50.0f), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler01);
          break;
        }
        case 1:
        {
          auto testHandler02 = new TestHandler02(ce::Vector3f(fi - 50.0f, 0.25f, fj - 50.0f), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler02);
          break;
        }
        case 2:
        {
          auto testHandler03 = new TestHandler03(ce::Vector3f(fi - 50.0f, 0.25f, fj - 50.0f), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler03);
          break;
        }
        case 3:
        {
          auto testHandler04 = new TestHandler04(ce::Vector3f(fi - 50.0f, 0.25f, fj - 50.0f), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler04);
          break;
        }
        default:
          break;
      }
#endif

      world->Attach(entity);
    }
  }

}

ce::Skeleton *global_skeleton = nullptr;

void add_skeleton_mesh(ce::World *world, ce::iMaterial *material)
{
  ce::SkeletonMesh *mesh = ce::AssetManager::Get()->Load<ce::SkeletonMesh>("/skinned_mesh.fbx");

  ce::Entity *entity = new ce::Entity("Skeleton Entity");

  ce::SkeletonMeshState *meshState = new ce::SkeletonMeshState();
  meshState->SetMesh(mesh);
  meshState->SetMaterial(0, material);
  entity->Attach(meshState);

  entity->GetRoot()->SetLocalMatrix(
      ce::Matrix4f::Rotation(ce::Vector3f(0.0f, 1.0f, 0.0f), M_PI)
      * ce::Matrix4f::Rotation(ce::Vector3f(1.0f, 0.0f, 0.0f), M_PI / 2.0f)
      * ce::Matrix4f::Translation(0, 2, 0));

  world->Attach(entity);

  global_skeleton = &meshState->GetSkeleton();

}


void generate_batched_test_grid(ce::World *world, ce::iMaterial *material)
{

  auto sphere = create_multi_sphere_mesh(0.25, 16, 12.0f, 25, new ce::Vector3f[] {
      ce::Vector3f(-2, 0.0f, -2.0f),
      ce::Vector3f(-1, 0.0f, -2.0f),
      ce::Vector3f(0, 0.0f, -2.0f),
      ce::Vector3f(1, 0.0f, -2.0f),
      ce::Vector3f(2, 0.0f, -2.0f),

      ce::Vector3f(-2, 0.0f, -1.0f),
      ce::Vector3f(-1, 0.0f, -1.0f),
      ce::Vector3f(0, 0.0f, -1.0f),
      ce::Vector3f(1, 0.0f, -1.0f),
      ce::Vector3f(2, 0.0f, -1.0f),

      ce::Vector3f(-2, 0.0f, 0.0f),
      ce::Vector3f(-1, 0.0f, 0.0f),
      ce::Vector3f(0, 0.0f, 0.0f),
      ce::Vector3f(1, 0.0f, 0.0f),
      ce::Vector3f(2, 0.0f, 0.0f),

      ce::Vector3f(-2, 0.0f, 1.0f),
      ce::Vector3f(-1, 0.0f, 1.0f),
      ce::Vector3f(0, 0.0f, 1.0f),
      ce::Vector3f(1, 0.0f, 1.0f),
      ce::Vector3f(2, 0.0f, 1.0f),

      ce::Vector3f(-2, 0.0f, 2.0f),
      ce::Vector3f(-1, 0.0f, 2.0f),
      ce::Vector3f(0, 0.0f, 2.0f),
      ce::Vector3f(1, 0.0f, 2.0f),
      ce::Vector3f(2, 0.0f, 2.0f)
  });
  auto mesh   = new ce::Mesh();
  mesh->AddMaterialSlot("Default", material);
  mesh->AddSubMesh(sphere, 0);
  int gridSize = 100;

  float start = static_cast<float>(gridSize) / 2.0f;

  for (int i = 0; i < gridSize; i += 5)
  {
    auto     fi = (float) i;
    for (int j  = 0; j < gridSize; j += 5)
    {
      auto fj     = (float) j;
      auto entity = new ce::Entity(std::string("Sphere: ") + std::to_string(i + 1) + ":" + std::to_string(j + 1));

      auto meshStateSphere = new ce::StaticMeshState("Mesh");
      meshStateSphere->SetStatic(true);
      meshStateSphere->GetTransform()
                     .SetTranslation(i - start + 2, 0.25f, j - start + 2)
                     .Finish();
      meshStateSphere->SetMesh(mesh);
      entity->Attach(meshStateSphere);


      world->Attach(entity);
    }
  }

}


void generate_physics(ce::World *world, ce::iMaterial *material)
{

  ce::iPhysicsSystem *physics = ce::ObjectRegistry::Get<ce::iPhysicsSystem>();
  // add the ground plane
  /*
  ce::BoxShapeDesc floorDesc{ ce::Vector3f(100.0f, 1.0f, 100.0f) };
  ce::iCollisionShape* floorShape    = physics->CreateShape(floorDesc);
  ce::iStaticCollider* floorCollider = physics->CreateStaticCollider();
  floorCollider->Attach(floorShape);
  floorCollider->SetTransform(ce::Matrix4f::Translation(0.0f, -1.0f, 0.0f));
  physWorld->AddCollider(floorCollider);
  */

  ce::Entity              *floorEntity         = new ce::Entity("Floor");
  ce::BoxColliderState    *floorBoxCollider    = new ce::BoxColliderState();
  ce::StaticColliderState *floorStaticCollider = new ce::StaticColliderState();
  floorBoxCollider->SetHalfExtends(ce::Vector3f(100.0f, 1.0f, 100.0f));
  floorEntity->Attach(floorBoxCollider);
  floorEntity->Attach(floorStaticCollider);
  floorEntity->GetRoot()->GetTransform().SetTranslation(ce::Vector3f(0.0f, -1.0f, 0.0f)).Finish();
  world->Attach(floorEntity);

  float sphereRadius = 0.5f;
  if (false)
  {
    ce::iRenderMesh *renderMeshSphere = create_sphere_mesh(sphereRadius, 16, 4.0f);

    for (int i = 0; i < 10; i++)
    {
      {
        ce::Mesh                *meshSphere          = new ce::Mesh();
        ce::Entity              *entitySphere        = new ce::Entity("Sphere");
        ce::StaticMeshState     *meshStateSphere     = new ce::StaticMeshState("Mesh.Sphere");
        ce::SphereColliderState *sphereColliderState = new ce::SphereColliderState();
        ce::RigidBodyState      *rigidBodyState      = new ce::RigidBodyState("RigidBody.Sphere");


        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        sphereColliderState->SetRadius(sphereRadius);

        entitySphere->Attach(sphereColliderState);
        entitySphere->Attach(rigidBodyState);
        rigidBodyState->Attach(meshStateSphere);

        rigidBodyState->GetTransform()
                      .SetTranslation(ce::Vector3f(0.0f, sphereRadius * 2.5f, 0.0f) * ((float) i + 2.0f))
                      .Finish();
        meshStateSphere->SetMesh(meshSphere);
        world->Attach(entitySphere);



        /*
        ce::iDynamicCollider* sphereCollider = physics->CreateDynamicCollider();
        sphereCollider->Attach(sphereShape);
        sphereCollider->SetTransform(entitySphere->GetRoot()->GetGlobalMatrix());
        sphereCollider->SetUserData(meshStateSphere);
        //physWorld->AddCollider(sphereCollider);
         */
      }
      {
        ce::Mesh            *meshSphere      = new ce::Mesh();
        ce::Entity          *entitySphere    = new ce::Entity("Sphere");
        ce::StaticMeshState *meshStateSphere = new ce::StaticMeshState("Mesh.Sphere");
        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        meshStateSphere->GetTransform()
                       .SetTranslation(ce::Vector3f(i * sphereRadius * 0.5, 0.0f, 0.0f))
                       .Finish();
        meshStateSphere->SetMesh(meshSphere);
        entitySphere->Attach(meshStateSphere);
        world->Attach(entitySphere);
      }
      {
        ce::Mesh            *meshSphere      = new ce::Mesh();
        ce::Entity          *entitySphere    = new ce::Entity("Sphere");
        ce::StaticMeshState *meshStateSphere = new ce::StaticMeshState("Mesh.Sphere");
        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        meshStateSphere->GetTransform()
                       .SetTranslation(ce::Vector3f(0.0f, i * sphereRadius, 0.0f))
                       .Finish();
        meshStateSphere->SetMesh(meshSphere);
        entitySphere->Attach(meshStateSphere);
        world->Attach(entitySphere);
      }
      {
        ce::Mesh            *meshSphere      = new ce::Mesh();
        ce::Entity          *entitySphere    = new ce::Entity("Sphere");
        ce::StaticMeshState *meshStateSphere = new ce::StaticMeshState("Mesh.Sphere");
        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        meshStateSphere->GetTransform()
                       .SetTranslation(ce::Vector3f(0.0f, 0.0f, i * 2.0f * sphereRadius))
                       .Finish();
        meshStateSphere->SetMesh(meshSphere);
        entitySphere->Attach(meshStateSphere);
        world->Attach(entitySphere);
      }
    }
  }


}

#include <regex>
#include <sstream>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwardpipeline.hh>
#include <ceOpenGL/gl4/pipeline/deferred/gl4deferredpipeline.hh>

ce::LightState *add_directional_light(ce::World *world,
                                      const ce::Vector3f &axis,
                                      float rad,
                                      const ce::Color4f &color,
                                      bool isStatic,
                                      bool castsShadow)
{
  ce::Entity     *entity     = new ce::Entity("Directional");
  ce::LightState *lightState = new ce::LightState("DirectionalLight");
  entity->Attach(lightState);
  lightState->SetType(ce::eLT_Directional);
  lightState->SetColor(color);
  lightState->SetShadowMapBias(0.003f);
  lightState->SetStatic(isStatic);
  lightState->SetCastShadow(castsShadow);
  lightState->GetTransform()
            .SetRotation(ce::Quaternion::FromAxisAngle(axis.Normalized(), rad))
            .Finish();
  world->Attach(entity);
  return lightState;
}


ce::LightState *add_point_light(ce::World *world,
                                const ce::Vector3f &position,
                                float range,
                                const ce::Color4f &color,
                                bool castsShadow)
{
  float rnd = (float) rand() / (float) RAND_MAX;

  auto entity     = new ce::Entity("Point");
  auto lightState = new ce::LightState("PointLight");
//  auto testState  = new TestHandler01(position, range * 0.25f, 0.5f * rnd);
  entity->Attach(lightState);
//  entity->Attach(testState);
  lightState->SetType(ce::eLT_Point);
  lightState->SetColor(color);
  lightState->SetRange(range);
  lightState->SetShadowMapBias(0.003f);
  lightState->SetStatic(false);
  lightState->SetCastShadow(castsShadow);
  lightState->GetTransform().SetTranslation(position).Finish();


  world->Attach(entity);
  return lightState;
}

void setup_world(ce::World *world)
{
  auto assetMan        = ce::AssetManager::Get();
  auto material        = assetMan->Get<ce::iMaterial>("/materials/Default.mat");
  auto skinnedMaterial = assetMan->Get<ce::iMaterial>("/materials/DefaultSkinned.mat");

  generate_terrain(world);
  generate_camera(world);
  generate_physics(world, material);
//  generate_batched_test_grid(world, material);
//  generate_test_grid(world, material);

  add_skeleton_mesh(world, skinnedMaterial);

#if 1
  shadowLightState = add_directional_light(world,
                                           ce::Vector3f(1.0f, 0.2f, 0.0f),
                                           ce::ceDeg2Rad(-45.0f),
                                           ce::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 0.9f,
                                           true,
                                           true);

  add_directional_light(world,
                        ce::Vector3f(1.0f, 0.2f, 0.0f),
                        ce::ceDeg2Rad(-45.0f),
                        ce::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 0.1f,
                        true,
                        false);
#endif

//  add_point_light(world, ce::Vector3f(0.0f, 5.0f, 0.0f), 50.0f, ce::Color4f(1.0, 1.0f, 1.0f), false);

#if 0
  for (int i=-5; i<=5; i+= 1) {
    for (int j=-5; j<=5; j+= 1) {

      add_point_light(world,
                      ce::Vector3f((float)i, 2.0f, (float)j), 5.0f, ce::Color4f(1.0, 0.8f, 0.4f) * 0.1f, false);
    }

  }
//  add_point_light(world, ce::Vector3f(10.0f, 10.0f, 10.0f), 10.0f, ce::Color4f(1.0, 0.0f, 1.0f), false);
//  add_point_light(world, ce::Vector3f(0.0f, 10.0f, 10.0f), 10.0f, ce::Color4f(0.5, 0.0f, 1.0f), false);
//  add_point_light(world, ce::Vector3f(-10.0f, 10.0f, 10.0f), 10.0f, ce::Color4f(0.0, 0.0f, 1.0f), false);
//
//  add_point_light(world, ce::Vector3f(10.0f, 10.0f, -10.0f), 25.0f, ce::Color4f(1.0, 1.0f, 0.0f), false);
//  add_point_light(world, ce::Vector3f(0.0f, 10.0f, -10.0f), 25.0f, ce::Color4f(0.5, 1.0f, 0.0f), false);
//  add_point_light(world, ce::Vector3f(-10.0f, 10.0f, -10.0f), 25.0f, ce::Color4f(0.0, 1.0f, 0.0f), false);
#endif
}

int main(int argc, char **argv)
{

  if (!register_modules(argc, argv))
  {
    return -1;
  }

  ce::DebugCache *debugCache = new ce::DebugCache();
  ce::ObjectRegistry::Register<ce::DebugCache>(debugCache);

  if (!initialize_modules(argc, argv))
  {
    return -1;
  }

  set_window_icon();

  ce::iDevice *device = ce::ObjectRegistry::Get<ce::iDevice>();


  ce::World *world = new ce::World();

  int wnd_width, wnd_height;
  SDL_GetWindowSize(wnd, &wnd_width, &wnd_height);

  ce::Settings settings("file:///${config}/display.config");
  ce::Vector2i resolution   = settings.GetVector2i("resolution", ce::Vector2i(wnd_width, wnd_height));
  int          width        = resolution.x;
  int          height       = resolution.y;
  int          multiSamples = settings.GetInt("multisamples", 1);

  setup_world(world);


  std::string title  = ce::Settings("display.config").GetText("title");
  float       rot    = 0.0f;
  float       entRot = 0.0f;

  uint32_t nextSec  = SDL_GetTicks() + 1000;
  uint32_t frames   = 0;
  uint32_t lastTime = SDL_GetTicks();

  bool anim = false;

  ce::iPhysicsSystem *physics = ce::ObjectRegistry::Get<ce::iPhysicsSystem>();


#if _DEBUG
  ce::Size numDrawCallsPerSec    = 0;
  ce::Size numTrianglesPerSec    = 0;
  ce::Size numShaderStateChanges = 0;
#endif

  float sunRotation    = 0.0f;
  float lightnRotation = 0.0f;

  auto renderTarget   = create_render_target(device, width, height, multiSamples);
  auto colorTexture   = renderTarget->GetColorTexture(0);
  auto *frameRenderer = ce::ObjectRegistry::Get<ce::iFrameRenderer>();

  auto forwardPipeline  = new ce::opengl::GL4ForwardPipeline();
  auto deferredPipeline = new ce::opengl::GL4DeferredPipeline();


  forwardPipeline->Initialize();
  deferredPipeline->Initialize();
  ce::iRenderPipeline *pipeline = deferredPipeline;


  frameRenderer->SetRenderPipeline(pipeline);

  float    rotation[4];
  bool     rotation_direction[4];
  for (int i = 0; i < 4; i++)
  {
    rotation[i]           = 0.0f;
    rotation_direction[i] = true;
  }

  bool first = true;
  while (true)
  {
#if _DEBUG
    device->ResetDebug();
#endif
    uint32_t time  = SDL_GetTicks();
    uint64_t _time = ce::Time::GetTime();
    if (time > nextSec)
    {
      nextSec += 1000;
      char buffer[1024];
#if _DEBUG
      sprintf_s < 1024 > (buffer,
          "%s  %d FPS  #%llu calls (%.2f triangles) %.2f shader changes",
          title.c_str(),
          frames,
          numDrawCallsPerSec,
          (float) numTrianglesPerSec / (float) frames,
          (float) numShaderStateChanges / (float) frames);
      numDrawCallsPerSec    = 0;
      numTrianglesPerSec    = 0;
      numShaderStateChanges = 0;
#else
      sprintf_s < 1024 > (buffer, "%s  %d FPS ", title.c_str(), frames);
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





//    SDL_GL_MakeCurrent(wnd, context);
    UpdateEvents();

    if (ce::Input::IsKeyPressed(ce::Key::eK_Escape))
    {
      break;
    }

    if (ce::Input::IsKeyPressed(ce::Key::eK_Space))
    {
      anim = !anim;
    }

    if (deltaTime != 0)
    {
      lastTime = time;
      float tpf = (float) deltaTime / 1000.0f;

      if (anim)
      {
        entRot += tpf * 1.0f;
        sunRotation += M_PI * 0.5f * tpf;
        lightnRotation += M_PI * 0.25f * tpf;
      }


      if (ce::Input::IsKeyPressed(ce::Key::eK_E))
      {
        debugCache->SetDebug(!debugCache->IsDebug());
      }

      for (int i = 0; i < 4; i++)
      {
        ce::Matrix4f boneMatrix = global_skeleton->GetBone(i);
        boneMatrix.ClearRotation();
        boneMatrix.SetRotationY(rotation[i]);
        global_skeleton->SetBone(i, boneMatrix);

        if (rotation_direction[i])
        {
          rotation[i] += tpf * 0.1f;
          if (rotation[i] > M_PI / 4.0f)
          {
            rotation[i]           = M_PI / 4.0f;
            rotation_direction[i] = false;
          }
        }
        else
        {
          rotation[i] -= tpf * 0.1f;
          if (rotation[i] < 0.0f)
          {
            rotation[i]           = 0.0f;
            rotation_direction[i] = true;
          }
        }


      }
      global_skeleton->UpdateBones();

      world->Update(tpf);
    }
    if (ce::Input::IsKeyPressed(ce::Key::eK_P))
    {
      if (pipeline == forwardPipeline)
      {
        printf("Switch to: DeferredPipeline\n");
        pipeline = deferredPipeline;
      }
      else
      {
        printf("Switch to: FowardPipeline\n");
        pipeline = forwardPipeline;
      }
      frameRenderer->SetRenderPipeline(pipeline);
    }


    if (ce::Input::IsKeyPressed(ce::Key::eK_M))
    {
      if (pipeline == deferredPipeline)
      {
        if (ce::Input::IsKeyDown(ce::Key::eK_LeftShift))
        {
          deferredPipeline->DecRenderMode();
        }
        else
        {
          deferredPipeline->IncRenderMode();
        }
      }
    }

    if (ce::Input::IsKeyPressed(ce::Key::eK_L) && shadowLightState)
    {
      shadowLightState->SetCastShadow(!shadowLightState->IsCastShadow());
    }

    frameRenderer->Render(renderTarget, device, world->GetScene());

    device->SetRenderTarget(nullptr);
    device->SetViewport(0, 0, wnd_width, wnd_height);
    device->SetDepthTest(false);
    device->SetBlending(false);
    device->RenderFullscreen(colorTexture);
    device->SetDepthTest(true);


#if _DEBUG
    numDrawCallsPerSec += device->GetNumberOfDrawCalls();
    numTrianglesPerSec += device->GetNumberOfTriangles();
    numShaderStateChanges += device->GetNumberOfShaderStateChanges();
#endif

    SDL_GL_SwapWindow(wnd);


//    break;
  }

  ce::iMouse *mouse = ce::Input::GetMouse();
  mouse->SetCursorMode(ce::eCursorMode::Free);
  mouse->SetVisible(true);

  return 0;
}

