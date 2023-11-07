
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
#include <ceCore/entity/spatialstate.hh>
#include <ceCore/entity/staticcolliderstate.hh>
#include <ceCore/entity/staticmeshstate.hh>
#include <ceCore/entity/terrainmeshstate.hh>
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
#include <ceLauncher/testhandler.hh>

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
  ce::Color4f              color(1.0f, 1.0f, 1.0f, 1.0f);
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
  for (int           i = 0; i < 1025; i++)
  {
    float    fi = (float) i / 1024.0f;
    for (int j  = 0; j < 1025; j++)
    {
      float fj = (float) j / 1024.0f;

      float a = sin(fi * 10.0f) * cos(fj * 10.0f);

//      heightData.push_back(0.5f + 0.5f * a);
      heightData.push_back(0.0f);
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
  ce::iRenderMeshGenerator *generator = ce::ObjectRegistry::Get<ce::iRenderMeshGeneratorFactory>()->Create();
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
          cosf(angleH + M_PI / 2.0),
          0.0f,
          sinf(angleH + M_PI / 2.0)
      );
      positions.push_back(normal * radius);
      normals.emplace_back(normal);
      tangents.emplace_back(tangent);
      uv.emplace_back(ce::Vector2f(factH * 2.0f * uv_f, factV * uv_f));
      colors.emplace_back(ce::Color4f(1.0f, 1.0f, 1.0f, 1.0f));
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
  size_t      num = 40;
  for (size_t i   = 0; i < num; i++)
  {
    float       x = -40.0f + (float) i / (float) num * 80.0f;
    for (size_t j = 0; j < num; j++)
    {
      float y = -40.0f + (float) j / (float) num * 80.0f;

      ce::Entity          *suzanneEntity = new ce::Entity("Entity1");
      ce::StaticMeshState *meshState1    = new ce::StaticMeshState("StaticMesh1");
      meshState1->GetTransform()
                .SetTranslation(ce::Vector3f(x, 0, y))
//                .SetRotation(ce::Quaternion::FromAxisAngle(1.0f, 0.0f, 0.0f, ce::ceDeg2Rad(180.0f)))
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
                          ce::World *world
                         )
{
  auto generator = ce::ObjectRegistry::Get<ce::iRenderMeshBatchGeneratorFactory>()->Create();

  size_t      num = 40;
  for (size_t i   = 0; i < num; i++)
  {
    float       x = -40.0f + (float) i / (float) num * 80.0f;
    for (size_t j = 0; j < num; j++)
    {
      float        y = -40.0f + (float) j / (float) num * 80.0f;
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

  ce::Entity          *suzanneEntity = new ce::Entity("Entity1");
  ce::StaticMeshState *meshState1    = new ce::StaticMeshState("StaticMesh1");
  meshState1->GetTransform()
            .SetTranslation(ce::Vector3f(0, 0, 0))
            .Finish();
  meshState1->SetMesh(suzyMesh);
  meshState1->SetStatic(true);
  suzanneEntity->Attach(meshState1);
  world->Attach(suzanneEntity);

  generator->Release();
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

void generate_test_grid(ce::World *world, ce::iMaterial *material)
{
  ce::iRenderMesh *sphere = create_sphere_mesh(0.25, 16, 4.0f);
  ce::Mesh        *mesh   = new ce::Mesh();
  mesh->AddMaterialSlot("Default", material);
  mesh->AddSubMesh(sphere, 0);

  for (int a = 0, i = 0; i < 100; i++)
  {
    for (int j = 0; j < 100; j++, a++)
    {
      ce::Entity
          *entity = new ce::Entity(std::string("Sphere: ") + std::to_string(i + 1) + ":" + std::to_string(j + 1));

      ce::StaticMeshState *meshStateSphere = new ce::StaticMeshState("Mesh");
      meshStateSphere->GetTransform()
                     .SetTranslation(ce::Vector3f(i - 50, 0.25f, j - 50))
                     .Finish();
      meshStateSphere->SetMesh(mesh);
      entity->Attach(meshStateSphere);

      float rnd = (float) rand() / (float) RAND_MAX;
      int   ma  = a % 4;
      ma =  5;
      switch (ma)
      {
        case 0:
        {

          TestHandler01 *testHandler01 = new TestHandler01(ce::Vector3f(i - 50, 0.25f, j - 50), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler01);
        }
          break;
        case 1:
        {
          TestHandler02 *testHandler02 = new TestHandler02(ce::Vector3f(i - 50, 0.25f, j - 50), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler02);
        }
          break;
        case 2:
        {
          TestHandler03 *testHandler03 = new TestHandler03(ce::Vector3f(i - 50, 0.25f, j - 50), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler03);
        }
          break;
        case 3:
        {
          TestHandler04 *testHandler04 = new TestHandler04(ce::Vector3f(i - 50, 0.25f, j - 50), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler04);
        }
          break;
        default:break;
      }


      world->Attach(entity);
    }
  }

}

#include <regex>
#include <sstream>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwardpipeline.hh>
#include <ceOpenGL/gl4/pipeline/deferred/gl4deferredpipeline.hh>

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

  ce::AssetManager *assetMan = ce::AssetManager::Get();

  ce::iMaterial *defaultMaterialInstance = assetMan->Get<ce::iMaterial>(ce::ResourceLocator(
      "/materials/Default.mat"
  ));

  ce::iMaterial *suzanneMaterial = assetMan->Get<ce::iMaterial>(ce::ResourceLocator(
      "/materials/Suzanne.matinstance"
  ));

  ce::iMaterial *broken = assetMan->Get<ce::iMaterial>(ce::ResourceLocator(
      "/materials/Broken.matinstance"
  ));

  ce::TerrainLayer
                       *greenGrassLayer =
      assetMan->Get<ce::TerrainLayer>(ce::ResourceLocator("/terrain/green_grass.terrainlayer"));
  ce::TerrainLayer     *dirtLayer       =
                           assetMan->Get<ce::TerrainLayer>(ce::ResourceLocator("/terrain/dirt.terrainlayer"));
  ce::TerrainLayer     *fieldstoneLayer =
                           assetMan->Get<ce::TerrainLayer>(ce::ResourceLocator("/terrain/fieldstone.terrainlayer"));
  ce::TerrainLayerMask *terrainLayers   =
                           assetMan->Get<ce::TerrainLayerMask>(ce::ResourceLocator("/terrain/terrain.terrainmask"));

  ce::World *world = new ce::World();

  int wnd_width, wnd_height;
  SDL_GetWindowSize(wnd, &wnd_width, &wnd_height);

  ce::Settings settings("file:///${config}/display.config");
  ce::Vector2i resolution   = settings.GetVector2i("resolution", ce::Vector2i(wnd_width, wnd_height));
  int          width        = resolution.x;
  int          height       = resolution.y;
  int          multiSamples = settings.GetInt("multisamples", 1);

  float aspect = (float) wnd_height / (float) wnd_width;

  {
    ce::iTerrainMesh     *terrainMesh  = create_terrain_mesh(40.0f);
    ce::Entity           *entity0      = new ce::Entity("Entity0");
    ce::StaticMeshState  *meshState0   = new ce::StaticMeshState("StaticMesh0");
    ce::TerrainMeshState *terrainState = new ce::TerrainMeshState();
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

  ce::Mesh *suzanneMesh = assetMan->Load<ce::Mesh>(ce::ResourceLocator("file:///suzanne.fbx"));
  ce::Mesh *cube        = assetMan->Load<ce::Mesh>(ce::ResourceLocator("cube.fbx"));
  suzanneMesh->SetDefaultMaterial(0, suzanneMaterial);
  cube->SetDefaultMaterial(0, defaultMaterialInstance);

//  create_suzannes_plain(suzanneMesh, world, suzanneMaterial);
//  create_suzanne_batch(suzanneMesh, world);
//  auto brickWallMesh = assetMan->Load<ce::Mesh>(ce::ResourceLocator("file:///brickwall.fbx"));
//  brickWallMesh->SetDefaultMaterial(0, suzanneMaterial);
//  brickWallMesh->SetDefaultMaterial(1, broken);
//  auto brickWallMeshState = new ce::StaticMeshState("BrickWall.Mesh");
//  auto brickWallEntity    = new ce::Entity("BrickWall");
//  brickWallMeshState->SetMesh(brickWallMesh);
//  brickWallEntity->Attach(brickWallMeshState);
//  brickWallEntity->GetRoot()->GetTransform().Finish();
//  world->Attach(brickWallEntity);


//  float              sphereRadius       = 4.0f;
//  ce::iRenderMesh    * renderMeshSphere = create_sphere_mesh(sphereRadius, 16, 4.0f);
//  ce::Mesh           * meshSphere       = new ce::Mesh();
//  ce::Entity         * entitySphere     = new ce::Entity("Sphere");
//  ce::StaticMeshState* meshStateSphere  = new ce::StaticMeshState("Mesh.Sphere");
//  meshSphere->AddMaterialSlot("Default", defaultMaterialInstance);
//  meshSphere->AddSubMesh(renderMeshSphere, 0);
//  meshStateSphere->GetTransform()
//                 .SetTranslation(ce::Vector3f(0.0f, sphereRadius * 1.5f, 0.0f))
//                 .Finish();
//  meshStateSphere->SetMesh(meshSphere);
//  entitySphere->Attach(meshStateSphere);


//  ce::Entity    * lightEntity = new ce::Entity("Light_0");
//  ce::LightState* lightState  = new ce::LightState("LightState");
//  lightEntity->Attach(lightState);
//  lightState->SetType(ce::eLT_Point);
//  lightState->SetShadowMapBias(0.001f);
//  lightState->SetColor(ce::Color4f(1.0f, 0.8f, 0.5f, 1.0f) * 0.5f);
//  lightState->SetRange(25);
//  lightState->SetStatic(true);
//  lightState->SetCastShadow(true);
//  lightState->GetTransform()
//            .SetTranslation(ce::Vector3f(5.0f, 1.0f, 5.0f))
//            .Finish();
//  world->Attach(lightEntity);



//  ce::SpatialState* spatialState = lightState;

  ce::Entity     *sunEntity     = new ce::Entity("Sun");
  ce::LightState *sunLightState = new ce::LightState("SunLight");
  sunEntity->Attach(sunLightState);
  sunLightState->SetType(ce::eLT_Directional);
  sunLightState->SetColor(ce::Color4f(0.7f, 0.7f, 1.0f, 1.0f) * 1.0);
  sunLightState->SetShadowMapBias(0.003f);
  sunLightState->SetStatic(true);
  sunLightState->SetCastShadow(false);
  sunLightState->SetTransform(sunLightState->GetTransform()
                                               //.SetRotation(ce::Quaternion::FromAxisAngle(ce::Vector3f(1.0f, 0.0f, 0.0f), ce::ceDeg2Rad(-45.0f)))
                                           .SetRotation(
                                               ce::Quaternion::FromAxisAngle(ce::Vector3f(1.0f, 0.2f, 0.0f)
                                                                                 .Normalize(),
                                                                             ce::ceDeg2Rad(-45.0f)))
  );
  world->Attach(sunEntity);


  sunEntity     = new ce::Entity("Sun");
  sunLightState = new ce::LightState("SunLight");
  sunEntity->Attach(sunLightState);
  sunLightState->SetType(ce::eLT_Directional);
  sunLightState->SetColor(ce::Color4f(1.0f, 0.0f, 1.0f, 1.0f) * 0.25);
  sunLightState->SetShadowMapBias(0.003f);
  sunLightState->SetStatic(true);
  sunLightState->SetCastShadow(false);
  sunLightState->SetTransform(sunLightState->GetTransform()
                                               //.SetRotation(ce::Quaternion::FromAxisAngle(ce::Vector3f(1.0f, 0.0f, 0.0f), ce::ceDeg2Rad(-45.0f)))
                                           .SetRotation(
                                               ce::Quaternion::FromAxisAngle(ce::Vector3f(1.0f, 0.2f, 0.0f)
                                                                                 .Normalize(),
                                                                             ce::ceDeg2Rad(45.0f)))
  );
  world->Attach(sunEntity);

  ce::Entity      *cameraEntity = new ce::Entity("Camera");
  ce::CameraState *cameraState  = new ce::CameraState();

  CameraHandler *cameraHandler = new CameraHandler();
  cameraEntity->Attach(cameraState);
  cameraEntity->Attach(cameraHandler);
  cameraEntity->GetRoot()->GetTransform()
              .SetTranslation(ce::Vector3f(20, 20, 20))
              .LookAt(ce::Vector3f(0, 0, 0))
              .Finish();
  world->Attach(cameraEntity);
  world->SetMainCamera(cameraState);


  ce::Entity      *mirrorCameraEntity = new ce::Entity("MirrorCamera");
  ce::CameraState *mirrorCameraState  = new ce::CameraState();
  MirrorHandler   *mirrorHandler      = new MirrorHandler();
  mirrorCameraEntity->Attach(mirrorCameraState);
  mirrorCameraEntity->Attach(mirrorHandler);
  mirrorCameraState->SetRenderShadows(true);
  mirrorHandler->SetCameraState(cameraState);
//  world->Attach(mirrorCameraEntity);

//  auto mirrorRenderTarget = create_render_target(device, width / 2, height / 2, 1);
//  mirrorCameraState->SetRenderTarget(mirrorRenderTarget);
//  materialMirror->Set(materialMirror->IndexOf("Mirror"), mirrorRenderTarget->GetColorTexture(0));



  auto renderTarget = create_render_target(device, width, height, multiSamples);
  auto colorTexture = renderTarget->GetColorTexture(0);

  ce::iFrameRenderer *frameRenderer = ce::ObjectRegistry::Get<ce::iFrameRenderer>();

  std::string title  = ce::Settings("display.config").GetText("title");
  float       rot    = 0.0f;
  float       entRot = 0.0f;

  uint32_t nextSec  = SDL_GetTicks() + 1000;
  uint32_t frames   = 0;
  uint32_t lastTime = SDL_GetTicks();

  bool  useCs     = true;
  bool  anim      = false;
  float roughness = 1.0f;
  float metallic  = 0.0f;

  ce::iPhysicsSystem *physics = ce::ObjectRegistry::Get<ce::iPhysicsSystem>();
//  ce::iPhysicsWorld* physWorld = world->GetPhysicsWorld();


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
    for (int        i                 = 0; i < 10; i++)
    {
      {
        ce::Mesh                *meshSphere          = new ce::Mesh();
        ce::Entity              *entitySphere        = new ce::Entity("Sphere");
        ce::StaticMeshState     *meshStateSphere     = new ce::StaticMeshState("Mesh.Sphere");
        ce::SphereColliderState *sphereColliderState = new ce::SphereColliderState();
        ce::RigidBodyState      *rigidBodyState      = new ce::RigidBodyState("RigidBody.Sphere");


        meshSphere->AddMaterialSlot("Default", defaultMaterialInstance);
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
        meshSphere->AddMaterialSlot("Default", defaultMaterialInstance);
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
        meshSphere->AddMaterialSlot("Default", defaultMaterialInstance);
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
        meshSphere->AddMaterialSlot("Default", defaultMaterialInstance);
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

  generate_test_grid(world, defaultMaterialInstance);
  ce::Matrix4f proj;
  device->GetPerspectiveProjection(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1024.0f, proj);

  ce::iMaterial *updateMaterial = suzanneMaterial;
  size_t roughnessIdx = updateMaterial->IndexOf("Roughness");
  size_t metallicIdx  = updateMaterial->IndexOf("Metallic");
#if _DEBUG
  ce::Size numDrawCallsPerSec = 0;
  ce::Size numTrianglesPerSec = 0;
  ce::Size numShaderStateChanges = 0;
#endif

  float sunRotation    = 0.0f;
  float lightnRotation = 0.0f;

  auto forwardPipeline  = new ce::opengl::GL4ForwardPipeline();
  forwardPipeline->Initialize();
  auto deferredPipeline = new ce::opengl::GL4DeferredPipeline();
  deferredPipeline->Initialize();
  ce::iRenderPipeline *pipeline         = forwardPipeline;
  frameRenderer->SetRenderPipeline(pipeline);

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
      sprintf_s<1024
      >(buffer, "%s  %d FPS [Roughness: %f] [Metallic: %f]", title.c_str(), frames, roughness, metallic);
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

      if (ce::Input::IsKeyDown(ce::Key::eK_Up))
      {
        roughness += 0.5f * tpf;
        roughness = roughness <= 1.0f ? roughness : 1.0f;
        updateMaterial->Set(roughnessIdx, roughness);
      }
      if (ce::Input::IsKeyDown(ce::Key::eK_Down))
      {
        roughness -= 0.5f * tpf;
        roughness = roughness >= 0.0f ? roughness : 0.0f;
        updateMaterial->Set(roughnessIdx, roughness);
      }

      if (ce::Input::IsKeyDown(ce::Key::eK_Right))
      {
        metallic += 0.5f * tpf;
        metallic = metallic <= 1.0f ? metallic : 1.0f;
        updateMaterial->Set(metallicIdx, metallic);
      }
      if (ce::Input::IsKeyDown(ce::Key::eK_Left))
      {
        metallic -= 0.5f * tpf;
        metallic = metallic >= 0.0f ? metallic : 0.0f;
        updateMaterial->Set(metallicIdx, metallic);
      }

      if (ce::Input::IsKeyPressed(ce::Key::eK_E))
      {
        debugCache->SetDebug(!debugCache->IsDebug());
      }


//      sunLightState->SetTransform(sunLightState->GetTransform()
//                                               .SetTranslation(ce::Vector3f(sin(sunRotation) * 20.0f,
//                                                                            20.0f,
//                                                                            cos(sunRotation) * 20.0f))
//                                               .LookAt(ce::Vector3f(0.0f, 0.0f, 0.0f), ce::Vector3f(0.0f, 1.0f, 0.0f))
//      );

//      spatialState->SetTransform(spatialState->GetTransform()
//          .SetTranslation(ce::Vector3f(5.0f, 1.1f, 5.0f) + ce::Vector3f(sin(lightnRotation) * 1.5, 0.0f, cos(lightnRotation) * 0.5f)));

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


//      terrainMesh->SetReferencePoint(cameraState->GetTransform().GetTranslation());
      world->Update(tpf);
    }
    if (ce::Input::IsKeyPressed(ce::Key::eK_P))
    {
      if (pipeline == forwardPipeline)
      {
        printf ("Switch to: DeferredPipeline\n");
        pipeline = deferredPipeline;
      }
      else
      {
        printf ("Switch to: FowardPipeline\n");
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

