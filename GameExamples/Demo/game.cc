//
// Created by MCEL on 08.05.2024.
//

#include "game.hh"

CE_DEFINE_GAME(Game)


#include <ceLauncher/launchermodule.hh>
#include "exitgamestate.hh"
#include <ceCore/engine.hh>
#include <ceCore/settings.hh>
#include <ceCore/ticker.hh>
#include <ceCore/animation/skeletonanimation.hh>
#include <ceCore/animation/skeletonanimationplayer.hh>
#include <ceCore/entity/camerastate.hh>
#include <ceCore/entity/collisionstate.hh>
#include <ceCore/entity/entity.hh>
#include <ceCore/entity/lightstate.hh>
#include <ceCore/entity/rigidbodystate.hh>
#include <ceCore/entity/skeletonmeshstate.hh>
#include <ceCore/entity/spatialstate.hh>
#include <ceCore/entity/staticcolliderstate.hh>
#include <ceCore/entity/staticmeshstate.hh>
#include <ceCore/entity/terrainmeshstate.hh>
#include <ceCore/entity/world.hh>
#include "ceCore/input/input.hh"
#include <ceCore/math/math.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/objectregistry.hh>
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
#include <ceCore/graphics/mesh.hh>
#include <ceCore/graphics/postprocessing.hh>
#include <ceCore/graphics/skybox/simpleskybox.hh>
#include <ceCore/graphics/pp/pp.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/physics/physics.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/window/iwindow.hh>
#include <ceCore/time.hh>


#include "../../GameExamples/Demo/camerahandler.hh"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>


ce::LightState *shadowLightState = nullptr;




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

        indices.push_back(idxOrigin + i00);
        indices.push_back(idxOrigin + i10);
        indices.push_back(idxOrigin + i11);

        indices.push_back(idxOrigin + i00);
        indices.push_back(idxOrigin + i11);
        indices.push_back(idxOrigin + i01);
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


ce::PostProcessing* setup_post_processing ()
{
  ce::PostProcessing* postProcessing = nullptr;
#if 0
  
  postProcessing = new ce::PostProcessing();


  auto highPass = new ce::PPHighPass(0.92f);
  auto scaleDown0 = new ce::PPScaleDown();
  auto scaleDown1 = new ce::PPScaleDown();
  auto blurH = new ce::PPBlurH(30, 1.f);
  auto blurV = new ce::PPBlurV(30, 1.f);
  auto combine = new ce::PPCombine();

  postProcessing->AddProcess(highPass);
  postProcessing->AddProcess(scaleDown0);
  postProcessing->AddProcess(scaleDown1);
  postProcessing->AddProcess(blurH);
  postProcessing->AddProcess(blurV);
  postProcessing->AddProcess(combine);


  postProcessing->Bind({nullptr, (size_t)ce::PPImageType::Color, highPass, 0});
  postProcessing->Bind({highPass, 0, scaleDown0, 0});
  postProcessing->Bind({scaleDown0, 0, scaleDown1, 0});

  postProcessing->Bind({scaleDown1, 0, blurH, 0 });
  postProcessing->Bind({blurH, 0, blurV, 0});
  postProcessing->Bind({nullptr, (size_t)ce::PPImageType::Color, combine, 0});
  postProcessing->Bind({blurV, 0, combine, 1});
  postProcessing->Bind({combine, 0, nullptr, (size_t)ce::PPImageType::Color });


#endif
  return postProcessing;
}

void generate_camera(ce::World *world)
{
  auto cameraEntity = new ce::Entity("Camera");

  auto cameraState  = new ce::CameraState();
  cameraState->SetClearMode(ce::eClearMode::DepthColor);
  cameraState->SetClearColor(ce::Color4f(0.0f, 0.0f, 0.5f));
  cameraState->SetClearColorMode(ce::eClearColorMode::Skybox);
  cameraState->SetSkyboxRenderer(new ce::SimpleSkybox());

  auto postProcessing = setup_post_processing ();
  cameraState->SetPostProcessing(postProcessing);


  auto cameraHandler = new CameraHandler();
  cameraEntity->Attach(cameraState);
  cameraEntity->Attach(cameraHandler);
  cameraEntity->GetRoot()->GetTransform()
              .SetTranslation(ce::Vector3f(-5, 5, -5))
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
//  start = 0.0f;

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

ce::Skeleton                *global_skeleton  = nullptr;
ce::SkeletonAnimation       *global_animation = nullptr;
ce::SkeletonAnimationPlayer *global_player    = nullptr;

ce::Entity *bones[4];

ce::Entity *add_bone(ce::World *world, ce::iMaterial *material)
{

  ce::Mesh *mesh = ce::AssetManager::Get()->Load<ce::Mesh>("/bone_x.fbx");
  if (!mesh)
  {
    return nullptr;
  }

  ce::Entity          *entity    = new ce::Entity("Bone");
  ce::StaticMeshState *meshState = new ce::StaticMeshState();
  meshState->SetMesh(mesh);
  meshState->SetMaterial(0, material);
  entity->Attach(meshState);
  entity->GetRoot()->SetLocalMatrix(ce::Matrix4f());

  world->Attach(entity);
  return entity;
}

void add_skeleton_mesh(ce::World *world, ce::iMaterial *material)
{

  ce::SkeletonMesh      *mesh      = ce::AssetManager::Get()->Load<ce::SkeletonMesh>("/skinned_mesh.fbx");
  ce::Entity            *entity    = new ce::Entity("Skeleton Entity");
  ce::SkeletonMeshState *meshState = new ce::SkeletonMeshState();
  meshState->SetMesh(mesh);
  meshState->SetMaterial(0, material);
  entity->Attach(meshState);

//  entity->GetRoot()->GetTransform()
//        .SetRotation(ce::Quaternion::FromAxisAngle(0.0f, 1.0f, 0.0f, M_PI / 1.0f * 1.0f))
//        .Finish();

//  world->Attach(entity);

  global_skeleton = &meshState->GetSkeleton();


  ce::SkeletonAnimationPack
      *animationPack = ce::AssetManager::Get()->Load<ce::SkeletonAnimationPack>("/skinned_mesh.fbx");
  global_animation = animationPack->Get("Armature|MyAnimation01");
  global_animation->SetLoop(true);

  global_player = new ce::SkeletonAnimationPlayer();
  global_player->SetSkeleton(global_skeleton);
  global_player->SetAnimation(global_animation);
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
  const ce::Vector3f &direction = lightState->GetTransform().GetForward();
  printf ("%.2f %.2f %.2f\n", direction.x, direction.y, direction.z);
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

void generate_axis_grid(ce::World *world)
{
  auto sphere = create_sphere_mesh(0.25, 16, 12.0f);
  auto matR   = ce::AssetManager::Get()->Get<ce::iMaterial>("/materials/DefaultRed.mat");
  auto matG   = ce::AssetManager::Get()->Get<ce::iMaterial>("/materials/DefaultGreen.mat");
  auto matB   = ce::AssetManager::Get()->Get<ce::iMaterial>("/materials/DefaultBlue.mat");

  auto meshR = new ce::Mesh();
  auto meshG = new ce::Mesh();
  auto meshB = new ce::Mesh();
  meshR->AddMaterialSlot("Default", matR);
  meshG->AddMaterialSlot("Default", matG);
  meshB->AddMaterialSlot("Default", matB);
  meshR->AddSubMesh(sphere, 0);
  meshG->AddSubMesh(sphere, 0);
  meshB->AddSubMesh(sphere, 0);


  int      gridSize = 10;
  for (int i        = 0; i < gridSize; i++)
  {
    {
      auto entity          = new ce::Entity();
      auto meshStateSphere = new ce::StaticMeshState("Mesh");
      meshStateSphere->SetStatic(true);
      meshStateSphere->GetTransform().SetTranslation((float) (i + 1) * 0.5f, 0.0f, 0.0f).Finish();
      meshStateSphere->SetMesh(meshR);
      entity->Attach(meshStateSphere);
      world->Attach(entity);
    }
    {
      auto entity          = new ce::Entity();
      auto meshStateSphere = new ce::StaticMeshState("Mesh");
      meshStateSphere->SetStatic(true);
      meshStateSphere->GetTransform().SetTranslation(0.0f, (float) (i + 1) * 0.5f, 0.0f).Finish();
      meshStateSphere->SetMesh(meshG);
      entity->Attach(meshStateSphere);
      world->Attach(entity);
    }
    {
      auto entity          = new ce::Entity();
      auto meshStateSphere = new ce::StaticMeshState("Mesh");
      meshStateSphere->SetStatic(true);
      meshStateSphere->GetTransform().SetTranslation(0.0f, 0.0f, (float) (i + 1) * 0.5f).Finish();
      meshStateSphere->SetMesh(meshB);
      entity->Attach(meshStateSphere);
      world->Attach(entity);
    }

  }
}

void generate_cube_fbx(ce::World *world)
{

  ce::Mesh *mesh = ce::AssetManager::Get()->Get<ce::Mesh>("/colored_cube.fbx");

  auto entity          = new ce::Entity();
  auto meshStateSphere = new ce::StaticMeshState("Mesh");
  meshStateSphere->SetStatic(true);
  meshStateSphere->GetTransform().SetTranslation(5.0f, 5.0f, 5.0f).Finish();
  meshStateSphere->SetMesh(mesh);
  entity->Attach(meshStateSphere);
  world->Attach(entity);
}

void generate_exit_game(ce::World *world)
{
  ce::Entity* entity = new ce::Entity();
  entity->Attach(new ExitGameState);
  world->Attach(entity);
}

void setup_world(ce::World *world)
{

  auto assetMan        = ce::AssetManager::Get();
  auto material        = assetMan->Get<ce::iMaterial>("/materials/Default.mat");
  auto skinnedMaterial = assetMan->Get<ce::iMaterial>("/materials/DefaultSkinned.mat");

  generate_exit_game(world);
  generate_terrain(world);
  generate_camera(world);
  generate_physics(world, material);
//  generate_batched_test_grid(world, material);
  generate_test_grid(world, material);
  generate_axis_grid(world);
  generate_cube_fbx(world);

  add_skeleton_mesh(world, skinnedMaterial);

  bones[0] = add_bone(world, material);
  bones[1] = add_bone(world, material);
  bones[2] = add_bone(world, material);
  bones[3] = add_bone(world, material);

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


bool Game::Initialize(ce::Engine *engine)
{
  setup_world(engine->GetWorld());
  return true;
}

