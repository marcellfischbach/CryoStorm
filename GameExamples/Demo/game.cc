//
// Created by MCEL on 08.05.2024.
//

#include "game.hh"
#include <ceCore/graphics/shadergraph/iShaderGraphCompiler.hh>
#include "ceCore/graphics/shadergraph/csShaderGraph.hh"
#include <ceCore/graphics/shadergraph/csSGNodes.hh>
#include <ceCore/objectregistry.hh>

CS_DEFINE_GAME(Game)


#include <ceLauncher/launchermodule.hh>
#include "exitgamestate.hh"
#include "testhandler.hh"
#include <ceCore/engine.hh>
#include <ceCore/settings.hh>
#include <ceCore/ticker.hh>
#include <ceCore/animation/csSkeletonAnimation.hh>
#include <ceCore/animation/csSkeletonAnimationPlayer.hh>
#include <ceCore/entity/csCameraState.hh>
#include <ceCore/entity/csCollisionState.hh>
#include "ceCore/entity/csEntity.hh"
#include <ceCore/entity/csLightState.hh>
#include <ceCore/entity/csRigidBodyState.hh>
#include <ceCore/entity/csSkeletonMeshState.hh>
#include <ceCore/entity/csSpatialState.hh>
#include <ceCore/entity/csStaticColliderState.hh>
#include <ceCore/entity/csStaticMeshState.hh>
#include <ceCore/entity/csTerrainMeshState.hh>
#include <ceCore/entity/csWorld.hh>
#include "ceCore/input/input.hh"
#include <ceCore/math/math.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/graphics/iDevice.hh>
#include <ceCore/graphics/iFrameRenderer.hh>
#include <ceCore/graphics/csImage.hh>
#include <ceCore/graphics/iPointLight.hh>
#include <ceCore/graphics/iRenderMesh.hh>
#include <ceCore/graphics/iRenderPipeline.hh>
#include <ceCore/graphics/iRenderTarget2D.hh>
#include <ceCore/graphics/iSampler.hh>
#include <ceCore/graphics/iTerrainMesh.hh>
#include <ceCore/graphics/csSkeletonMesh.hh>
#include <ceCore/graphics/csMesh.hh>
#include <ceCore/graphics/csPostProcessing.hh>
#include <ceCore/graphics/skybox/csSimpleSkybox.hh>
#include "ceCore/graphics/pp/csPP.hh"
#include <ceCore/graphics/shading/iShaderAttribute.hh>
#include <ceCore/graphics/scene/iGfxScene.hh>
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


cryo::csLightState *shadowLightState = nullptr;




cryo::iTerrainMesh *create_terrain_mesh(float size)
{
#define FLAT
  cryo::iTerrainMeshGenerator *generator = cryo::ObjectRegistry::Get<cryo::iTerrainMeshGeneratorFactory>()->Create();

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

  generator->SetSize(cryo::eTerrainSize::TS_129);
  generator->SetPatchSize(cryo::eTerrainSize::TS_33);
  generator->SetSize(cryo::eTerrainSize::TS_1025);
  generator->SetPatchSize(cryo::eTerrainSize::TS_129);
  generator->SetNormalizedHeightData(heightData);
  generator->SetSize(cryo::Vector3f(-size, 0.0f, -size), cryo::Vector3f(size, 10.0f, size));

  cryo::iTerrainMesh *terrainMesh = generator->Generate();
  generator->Release();
  return terrainMesh;
}

cryo::iRenderMesh *create_sphere_mesh(float radius, uint32_t detail, float uv_f)
{
  cryo::iRenderMeshGenerator  *generator = cryo::ObjectRegistry::Get<cryo::iRenderMeshGeneratorFactory>()->Create();
  std::vector<cryo::Vector3f> positions;
  std::vector<cryo::Vector3f> normals;
  std::vector<cryo::Vector3f> tangents;
  std::vector<cryo::Vector2f> uv;
  std::vector<cryo::Color4f>  colors;
  std::vector<uint32_t>     indices;

  for (uint32_t v = 0; v < detail; v++)
  {
    float factV  = (float) v / (float) (detail - 1);
    float angleV = -(float) M_PI_2 + factV * (float) M_PI;

    for (uint32_t h = 0; h < detail * 2; h++)
    {
      float factH  = (float) h / (float) (detail * 2 - 1);
      float angleH = factH * (float) M_PI * 2.0f;

      cryo::Vector3f normal(
          cosf(angleV) * cosf(angleH),
          sinf(angleV),
          cosf(angleV) * sinf(angleH)
      );
      cryo::Vector3f tangent(
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
  cryo::iRenderMesh *renderMesh = generator->Generate();
  generator->Release();
  return renderMesh;

}


cryo::iRenderMesh *
create_multi_sphere_mesh(float radius, uint32_t detail, float uv_f, size_t num_spheres, cryo::Vector3f *sphere_positions)
{
  cryo::iRenderMeshGenerator  *generator = cryo::ObjectRegistry::Get<cryo::iRenderMeshGeneratorFactory>()->Create();
  std::vector<cryo::Vector3f> positions;
  std::vector<cryo::Vector3f> normals;
  std::vector<cryo::Vector3f> tangents;
  std::vector<cryo::Vector2f> uv;
  std::vector<cryo::Color4f>  colors;
  std::vector<uint32_t>     indices;

  size_t      idxOrigin = 0;
  for (size_t i         = 0; i < num_spheres; i++)
  {
    cryo::Vector3f  origin = sphere_positions[i];
    for (uint32_t v      = 0; v < detail; v++)
    {
      float factV  = (float) v / (float) (detail - 1);
      float angleV = -(float) M_PI_2 + factV * (float) M_PI;

      for (uint32_t h = 0; h < detail * 2; h++)
      {
        float factH  = (float) h / (float) (detail * 2 - 1);
        float angleH = factH * (float) M_PI * 2.0f;

        cryo::Vector3f normal(
            cosf(angleV) * cosf(angleH),
            sinf(angleV),
            cosf(angleV) * sinf(angleH)
        );
        cryo::Vector3f tangent(
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
  cryo::iRenderMesh *renderMesh = generator->Generate();
  generator->Release();
  return renderMesh;

}


void debug(cryo::csSpatialState *state, int indent)
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
  for (cryo::Size i = 0, in = state->GetNumberOfChildren(); i < in; i++)
  {
    debug(state->GetChild(i), indent + 1);
  }
}


cryo::iRenderTarget2D *create_render_target(cryo::iDevice *device, uint32_t width, uint32_t height, uint16_t multiSamples)
{
  cryo::iSampler *colorSampler = device->CreateSampler();
  colorSampler->SetFilterMode(cryo::eFM_MinMagNearest);

  cryo::iSampler *depthSampler = device->CreateSampler();
  depthSampler->SetFilterMode(cryo::eFM_MinMagNearest);
  depthSampler->SetTextureCompareFunc(cryo::eCF_LessOrEqual);
  depthSampler->SetTextureCompareMode(cryo::eTCM_None);

  cryo::iTexture2D::Descriptor rt_col_desc = {};
  rt_col_desc.Width        = width;
  rt_col_desc.Height       = height;
  rt_col_desc.Format       = cryo::ePF_RGBA;
  rt_col_desc.MipMaps      = false;
  rt_col_desc.MultiSamples = multiSamples;
  cryo::iTexture2D *color_texture = device->CreateTexture(rt_col_desc);
  color_texture->SetSampler(colorSampler);

  cryo::iTexture2D::Descriptor rt_dpth_desc = {};
  rt_dpth_desc.Width        = width;
  rt_dpth_desc.Height       = height;
  rt_dpth_desc.Format       = cryo::ePF_DepthStencil;
  rt_dpth_desc.MipMaps      = false;
  rt_dpth_desc.MultiSamples = multiSamples;
  cryo::iTexture2D *depth_texture = device->CreateTexture(rt_dpth_desc);
  depth_texture->SetSampler(depthSampler);
  printf("CreateDepthTexture: %p\n", depth_texture);


  cryo::iRenderTarget2D::Descriptor rt_desc = {};
  rt_desc.Width  = width;
  rt_desc.Height = height;

  cryo::iRenderTarget2D *renderTarget = device->CreateRenderTarget(rt_desc);
  renderTarget->AddColorTexture(color_texture);
//  renderTarget->SetDepthBuffer(cryo::ePF_Depth);
  renderTarget->SetDepthTexture(depth_texture);
  if (!renderTarget->Compile())
  {
    printf("Unable to compile render target: %s\n", renderTarget->GetCompileLog().c_str());
    return nullptr;
  }
  return renderTarget;
}


cryo::csPostProcessing* setup_post_processing()
{
  cryo::csPostProcessing * postProcessing = nullptr;
#if 0
  
  postProcessing = new cryo::PostProcessing();


  auto highPass = new cryo::PPHighPass(0.92f);
  auto scaleDown0 = new cryo::PPScaleDown();
  auto scaleDown1 = new cryo::PPScaleDown();
  auto blurH = new cryo::PPBlurH(30, 1.f);
  auto blurV = new cryo::PPBlurV(30, 1.f);
  auto combine = new cryo::PPCombine();

  postProcessing->AddProcess(highPass);
  postProcessing->AddProcess(scaleDown0);
  postProcessing->AddProcess(scaleDown1);
  postProcessing->AddProcess(blurH);
  postProcessing->AddProcess(blurV);
  postProcessing->AddProcess(combine);


  postProcessing->Bind({nullptr, (size_t)cryo::PPImageType::Color, highPass, 0});
  postProcessing->Bind({highPass, 0, scaleDown0, 0});
  postProcessing->Bind({scaleDown0, 0, scaleDown1, 0});

  postProcessing->Bind({scaleDown1, 0, blurH, 0 });
  postProcessing->Bind({blurH, 0, blurV, 0});
  postProcessing->Bind({nullptr, (size_t)cryo::PPImageType::Color, combine, 0});
  postProcessing->Bind({blurV, 0, combine, 1});
  postProcessing->Bind({combine, 0, nullptr, (size_t)cryo::PPImageType::Color });


#endif
  return postProcessing;
}

void generate_camera(cryo::csWorld *world)
{
  auto cameraEntity = new cryo::csEntity("Camera");

  auto cameraState  = new cryo::csCameraState();
  cameraState->SetClearMode(cryo::eClearMode::DepthColor);
  cameraState->SetClearColor(cryo::Color4f(0.0f, 0.0f, 0.5f));
  cameraState->SetClearColorMode(cryo::eClearColorMode::Skybox);
  cameraState->SetSkyboxRenderer(new cryo::csSimpleSkybox());

  auto postProcessing = setup_post_processing ();
  cameraState->SetPostProcessing(postProcessing);


  auto cameraHandler = new CameraHandler();
  cameraEntity->Attach(cameraState);
  cameraEntity->Attach(cameraHandler);
  cameraEntity->GetRoot()->GetTransform()
              .SetTranslation(cryo::Vector3f(-5, 5, -5))
              .LookAt(cryo::Vector3f(0, 0, 0))
              .Finish();
  world->Attach(cameraEntity);
  world->SetMainCamera(cameraState);

//  auto mirrorCameraEntity = new cryo::Entity("MirrorCamera");
//  auto mirrorCameraState  = new cryo::CameraState();
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

void generate_terrain(cryo::csWorld *world)
{
  cryo::AssetManager *assetMan = cryo::AssetManager::Get();

  auto greenGrassLayer = assetMan->Get<cryo::csTerrainLayer>("/terrain/green_grass.terrainlayer");
  auto dirtLayer       = assetMan->Get<cryo::csTerrainLayer>("/terrain/dirt.terrainlayer");
  auto fieldstoneLayer = assetMan->Get<cryo::csTerrainLayer>("/terrain/fieldstone.terrainlayer");
  auto terrainLayers   = assetMan->Get<cryo::csTerrainLayerMask>("/terrain/terrain.terrainmask");

  auto terrainMesh  = create_terrain_mesh(40.0f);
  auto entity0      = new cryo::csEntity("Terrain");
  auto terrainState = new cryo::csTerrainMeshState();
  terrainState->SetTerrainMesh(terrainMesh);
  terrainState->SetLayerMask(terrainLayers);
  terrainState->AddLayer(greenGrassLayer);
  terrainState->AddLayer(dirtLayer);
  terrainState->AddLayer(fieldstoneLayer);
  terrainState->GetTransform()
              .SetTranslation(cryo::Vector3f(0, 0, 0))
              .Finish();
  terrainState->SetStatic(true);
  entity0->Attach(terrainState);
  world->Attach(entity0);
}


void generate_test_grid(cryo::csWorld *world, cryo::iMaterial *material)
{
  auto sphere = create_sphere_mesh(0.25, 16, 12.0f);
  auto mesh   = new cryo::csMesh();
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
      auto entity = new cryo::csEntity(std::string("Sphere: ") + std::to_string(i + 1) + ":" + std::to_string(j + 1));

      auto meshStateSphere = new cryo::csStaticMeshState("Mesh");
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

          auto testHandler01 = new TestHandler01(cryo::Vector3f(i - start, 0.25f, j - start), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler01);
          break;
        }
        case 1:
        {
          auto testHandler02 = new TestHandler02(cryo::Vector3f(i - start, 0.25f, j - start), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler02);
          break;
        }
        case 2:
        {
          auto testHandler03 = new TestHandler03(cryo::Vector3f(i - start, 0.25f, j - start), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler03);
          break;
        }
        case 3:
        {
          auto testHandler04 = new TestHandler04(cryo::Vector3f(i - start, 0.25f, j - start), 0.25f, 0.5f + rnd);
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

cryo::csSkeleton          *global_skeleton  = nullptr;
cryo::csSkeletonAnimation       *global_animation = nullptr;
cryo::csSkeletonAnimationPlayer *global_player    = nullptr;

cryo::csEntity *bones[4];

cryo::csEntity *add_bone(cryo::csWorld *world, cryo::iMaterial *material)
{

  cryo::csMesh *mesh = cryo::AssetManager::Get()->Load<cryo::csMesh>("/bone_x.fbx");
  if (!mesh)
  {
    return nullptr;
  }

  cryo::csEntity          *entity    = new cryo::csEntity("Bone");
  cryo::csStaticMeshState *meshState = new cryo::csStaticMeshState();
  meshState->SetMesh(mesh);
  meshState->SetMaterial(0, material);
  entity->Attach(meshState);
  entity->GetRoot()->SetLocalMatrix(cryo::Matrix4f());

  world->Attach(entity);
  return entity;
}

void add_skeleton_mesh(cryo::csWorld *world, cryo::iMaterial *material)
{

  cryo::csSkeletonMesh *mesh   = cryo::AssetManager::Get()->Load<cryo::csSkeletonMesh>("/skinned_mesh.fbx");
  cryo::csEntity       *entity = new cryo::csEntity("Skeleton Entity");
  cryo::csSkeletonMeshState *meshState = new cryo::csSkeletonMeshState();
  meshState->SetMesh(mesh);
  meshState->SetMaterial(0, material);
  entity->Attach(meshState);

//  entity->GetRoot()->GetTransform()
//        .SetRotation(cryo::Quaternion::FromAxisAngle(0.0f, 1.0f, 0.0f, M_PI / 1.0f * 1.0f))
//        .Finish();

//  world->Attach(entity);

  global_skeleton = &meshState->GetSkeleton();


  cryo::SkeletonAnimationPack
      *animationPack = cryo::AssetManager::Get()->Load<cryo::SkeletonAnimationPack>("/skinned_mesh.fbx");
  global_animation = animationPack->Get("Armature|MyAnimation01");
  global_animation->SetLoop(true);

  global_player = new cryo::csSkeletonAnimationPlayer();
  global_player->SetSkeleton(global_skeleton);
  global_player->SetAnimation(global_animation);
}


void generate_batched_test_grid(cryo::csWorld *world, cryo::iMaterial *material)
{

  auto sphere = create_multi_sphere_mesh(0.25, 16, 12.0f, 25, new cryo::Vector3f[] {
      cryo::Vector3f(-2, 0.0f, -2.0f),
      cryo::Vector3f(-1, 0.0f, -2.0f),
      cryo::Vector3f(0, 0.0f, -2.0f),
      cryo::Vector3f(1, 0.0f, -2.0f),
      cryo::Vector3f(2, 0.0f, -2.0f),

      cryo::Vector3f(-2, 0.0f, -1.0f),
      cryo::Vector3f(-1, 0.0f, -1.0f),
      cryo::Vector3f(0, 0.0f, -1.0f),
      cryo::Vector3f(1, 0.0f, -1.0f),
      cryo::Vector3f(2, 0.0f, -1.0f),

      cryo::Vector3f(-2, 0.0f, 0.0f),
      cryo::Vector3f(-1, 0.0f, 0.0f),
      cryo::Vector3f(0, 0.0f, 0.0f),
      cryo::Vector3f(1, 0.0f, 0.0f),
      cryo::Vector3f(2, 0.0f, 0.0f),

      cryo::Vector3f(-2, 0.0f, 1.0f),
      cryo::Vector3f(-1, 0.0f, 1.0f),
      cryo::Vector3f(0, 0.0f, 1.0f),
      cryo::Vector3f(1, 0.0f, 1.0f),
      cryo::Vector3f(2, 0.0f, 1.0f),

      cryo::Vector3f(-2, 0.0f, 2.0f),
      cryo::Vector3f(-1, 0.0f, 2.0f),
      cryo::Vector3f(0, 0.0f, 2.0f),
      cryo::Vector3f(1, 0.0f, 2.0f),
      cryo::Vector3f(2, 0.0f, 2.0f)
  });
  auto mesh   = new cryo::csMesh();
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
      auto entity = new cryo::csEntity(std::string("Sphere: ") + std::to_string(i + 1) + ":" + std::to_string(j + 1));

      auto meshStateSphere = new cryo::csStaticMeshState("Mesh");
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


void generate_physics(cryo::csWorld *world, cryo::iMaterial *material)
{

  cryo::iPhysicsSystem *physics = cryo::ObjectRegistry::Get<cryo::iPhysicsSystem>();
  // add the ground plane
  /*
  cryo::BoxShapeDesc floorDesc{ cryo::Vector3f(100.0f, 1.0f, 100.0f) };
  cryo::iCollisionShape* floorShape    = physics->CreateShape(floorDesc);
  cryo::iStaticCollider* floorCollider = physics->CreateStaticCollider();
  floorCollider->Attach(floorShape);
  floorCollider->SetTransform(cryo::Matrix4f::Translation(0.0f, -1.0f, 0.0f));
  physWorld->AddCollider(floorCollider);
  */

  cryo::csEntity           *floorEntity      = new cryo::csEntity("Floor");
  cryo::csBoxColliderState    *floorBoxCollider    = new cryo::csBoxColliderState();
  cryo::csStaticColliderState *floorStaticCollider = new cryo::csStaticColliderState();
  floorBoxCollider->SetHalfExtends(cryo::Vector3f(100.0f, 1.0f, 100.0f));
  floorEntity->Attach(floorBoxCollider);
  floorEntity->Attach(floorStaticCollider);
  floorEntity->GetRoot()->GetTransform().SetTranslation(cryo::Vector3f(0.0f, -1.0f, 0.0f)).Finish();
  world->Attach(floorEntity);

  float sphereRadius = 0.5f;
  if (false)
  {
    cryo::iRenderMesh *renderMeshSphere = create_sphere_mesh(sphereRadius, 16, 4.0f);

    for (int i = 0; i < 10; i++)
    {
      {
        cryo::csMesh   *meshSphere   = new cryo::csMesh();
        cryo::csEntity *entitySphere = new cryo::csEntity("Sphere");
        cryo::csStaticMeshState     *meshStateSphere     = new cryo::csStaticMeshState("Mesh.Sphere");
        cryo::csSphereColliderState *sphereColliderState = new cryo::csSphereColliderState();
        cryo::csRigidBodyState      *rigidBodyState      = new cryo::csRigidBodyState("RigidBody.Sphere");


        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        sphereColliderState->SetRadius(sphereRadius);

        entitySphere->Attach(sphereColliderState);
        entitySphere->Attach(rigidBodyState);
        rigidBodyState->Attach(meshStateSphere);

        rigidBodyState->GetTransform()
                      .SetTranslation(cryo::Vector3f(0.0f, sphereRadius * 2.5f, 0.0f) * ((float) i + 2.0f))
                      .Finish();
        meshStateSphere->SetMesh(meshSphere);
        world->Attach(entitySphere);



        /*
        cryo::iDynamicCollider* sphereCollider = physics->CreateDynamicCollider();
        sphereCollider->Attach(sphereShape);
        sphereCollider->SetTransform(entitySphere->GetRoot()->GetGlobalMatrix());
        sphereCollider->SetUserData(meshStateSphere);
        //physWorld->AddCollider(sphereCollider);
         */
      }
      {
        cryo::csMesh   *meshSphere   = new cryo::csMesh();
        cryo::csEntity *entitySphere = new cryo::csEntity("Sphere");
        cryo::csStaticMeshState *meshStateSphere = new cryo::csStaticMeshState("Mesh.Sphere");
        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        meshStateSphere->GetTransform()
                       .SetTranslation(cryo::Vector3f(i * sphereRadius * 0.5, 0.0f, 0.0f))
                       .Finish();
        meshStateSphere->SetMesh(meshSphere);
        entitySphere->Attach(meshStateSphere);
        world->Attach(entitySphere);
      }
      {
        cryo::csMesh   *meshSphere   = new cryo::csMesh();
        cryo::csEntity *entitySphere = new cryo::csEntity("Sphere");
        cryo::csStaticMeshState *meshStateSphere = new cryo::csStaticMeshState("Mesh.Sphere");
        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        meshStateSphere->GetTransform()
                       .SetTranslation(cryo::Vector3f(0.0f, i * sphereRadius, 0.0f))
                       .Finish();
        meshStateSphere->SetMesh(meshSphere);
        entitySphere->Attach(meshStateSphere);
        world->Attach(entitySphere);
      }
      {
        cryo::csMesh   *meshSphere   = new cryo::csMesh();
        cryo::csEntity *entitySphere = new cryo::csEntity("Sphere");
        cryo::csStaticMeshState *meshStateSphere = new cryo::csStaticMeshState("Mesh.Sphere");
        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        meshStateSphere->GetTransform()
                       .SetTranslation(cryo::Vector3f(0.0f, 0.0f, i * 2.0f * sphereRadius))
                       .Finish();
        meshStateSphere->SetMesh(meshSphere);
        entitySphere->Attach(meshStateSphere);
        world->Attach(entitySphere);
      }
    }
  }


}



cryo::csLightState *add_directional_light(cryo::csWorld *world,
                                          const cryo::Vector3f &axis,
                                          float rad,
                                          const cryo::Color4f &color,
                                          bool isStatic,
                                          bool castsShadow)
{
  cryo::csEntity     *entity     = new cryo::csEntity("Directional");
  cryo::csLightState *lightState = new cryo::csLightState("DirectionalLight");
  entity->Attach(lightState);
  lightState->SetType(cryo::eLT_Directional);
  lightState->SetColor(color);
  lightState->SetShadowMapBias(0.003f);
  lightState->SetStatic(isStatic);
  lightState->SetCastShadow(castsShadow);
  lightState->GetTransform()
            .SetRotation(cryo::Quaternion::FromAxisAngle(axis.Normalized(), rad))
            .Finish();
  world->Attach(entity);
  const cryo::Vector3f &direction = lightState->GetTransform().GetForward();
  printf ("%.2f %.2f %.2f\n", direction.x, direction.y, direction.z);
  return lightState;
}


cryo::csLightState *add_point_light(cryo::csWorld *world,
                                    const cryo::Vector3f &position,
                                    float range,
                                    const cryo::Color4f &color,
                                    bool castsShadow)
{
  float rnd = (float) rand() / (float) RAND_MAX;

  auto entity     = new cryo::csEntity("Point");
  auto lightState = new cryo::csLightState("PointLight");
//  auto testState  = new TestHandler01(position, range * 0.25f, 0.5f * rnd);
  entity->Attach(lightState);
//  entity->Attach(testState);
  lightState->SetType(cryo::eLT_Point);
  lightState->SetColor(color);
  lightState->SetRange(range);
  lightState->SetShadowMapBias(0.003f);
  lightState->SetStatic(false);
  lightState->SetCastShadow(castsShadow);
  lightState->GetTransform().SetTranslation(position).Finish();


  world->Attach(entity);
  return lightState;
}


cryo::iMaterial *generate_color_material (const cryo::Color4f &color)
{
  auto sg = new cryo::csShaderGraph();


  auto roughness = sg->Add<cryo::csSGConstFloat>("Roughness");
  auto diffuse = sg->Add<cryo::csSGConstColor3>("Diffuse");
  auto alpha = sg->Add<cryo::csSGConstFloat>("Alpha");
  auto normal = sg->Add<cryo::csSGConstColor3>("Normal");

  roughness->SetValue(1.0f);
  diffuse->SetValue(color.r, color.g, color.b);
  alpha->SetValue(1.0f);
  normal->SetValue(0.5f, 0.5f, 1.0f);



  sg->BindDiffuse(diffuse);
  sg->BindAlpha(alpha);
  sg->BindRoughness(roughness);
  sg->BindNormal(normal);
  sg->SetReceiveShadow(false);


  auto compilerFactory = cryo::ObjectRegistry::Get<cryo::iShaderGraphCompilerFactory>();
  if (compilerFactory)
  {
    cryo::iShaderGraphCompiler* compiler = compilerFactory->Create();
    if (compiler)
    {
      cryo::iShaderGraphCompiler::Parameters parameters {};
      memset (&parameters, 0, sizeof(parameters));
      return compiler->Compile(sg, parameters);
    }
  }

  return nullptr;
}



void generate_axis_grid(cryo::csWorld *world)
{
  auto sphere = create_sphere_mesh(0.25, 16, 12.0f);
  auto matR   = cryo::AssetManager::Get()->Get<cryo::iMaterial>("/materials/DefaultRed.matinstance");
  auto matG   = cryo::AssetManager::Get()->Get<cryo::iMaterial>("/materials/DefaultGreen.matinstance");
  auto matB   = cryo::AssetManager::Get()->Get<cryo::iMaterial>("/materials/DefaultBlue.matinstance");

//  matR = generate_color_material(cryo::Color4f(0.5f, 0.0f, 0.0f));
//  matG = generate_color_material(cryo::Color4f(0.0f, 0.5f, 0.0f));
//  matB = generate_color_material(cryo::Color4f(0.0f, 0.0f, 0.5f));

  cryo::s_material_names[matR] = "DefaultRed";
  cryo::s_material_names[matG] = "DefaultGreen";
  cryo::s_material_names[matB] = "DefaultBlue";

  auto meshR = new cryo::csMesh();
  auto meshG = new cryo::csMesh();
  auto meshB = new cryo::csMesh();
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
      auto entity          = new cryo::csEntity();
      auto meshStateSphere = new cryo::csStaticMeshState("Mesh");
      meshStateSphere->SetStatic(true);
      meshStateSphere->GetTransform().SetTranslation((float) (i + 1) * 0.5f, 0.0f, 0.0f).Finish();
      meshStateSphere->SetMesh(meshR);
      entity->Attach(meshStateSphere);
      world->Attach(entity);
    }
    {
      auto entity          = new cryo::csEntity();
      auto meshStateSphere = new cryo::csStaticMeshState("Mesh");
      meshStateSphere->SetStatic(true);
      meshStateSphere->GetTransform().SetTranslation(0.0f, (float) (i + 1) * 0.5f, 0.0f).Finish();
      meshStateSphere->SetMesh(meshG);
      entity->Attach(meshStateSphere);
      world->Attach(entity);
    }
    {
      auto entity          = new cryo::csEntity();
      auto meshStateSphere = new cryo::csStaticMeshState("Mesh");
      meshStateSphere->SetStatic(true);
      meshStateSphere->GetTransform().SetTranslation(0.0f, 0.0f, (float) (i + 1) * 0.5f).Finish();
      meshStateSphere->SetMesh(meshB);
      entity->Attach(meshStateSphere);
      world->Attach(entity);
    }

  }
}

void generate_cube_fbx(cryo::csWorld *world)
{

  cryo::csMesh *mesh = cryo::AssetManager::Get()->Get<cryo::csMesh>("/cube2.fbx");

  mesh = new cryo::csMesh();
  mesh->AddSubMesh(create_sphere_mesh(3.0f, 32, 1.0f), 0);
  mesh->AddMaterialSlot("Default");
  cryo::iMaterial *dustMaterial = cryo::AssetManager::Get()->Get<cryo::iMaterial>("/materials/Dust.sg");
//  cryo::iMaterial *dustMaterial = cryo::AssetManager::Get()->Get<cryo::iMaterial>("/materials/Dust.mat");
  cryo::s_material_names[dustMaterial] = "Dust";

  for (int i = 0; i < mesh->GetNumberOfMaterialSlots(); ++i)
  {
    mesh->SetDefaultMaterial(i, dustMaterial);
  }

  auto entity          = new cryo::csEntity();
  auto meshStateSphere = new cryo::csStaticMeshState("Mesh");
  meshStateSphere->SetStatic(true);
  meshStateSphere->GetTransform().SetTranslation(5.0f, 0.0f, 0.4f).Finish();
  meshStateSphere->SetMesh(mesh);


  entity->Attach(meshStateSphere);
  world->Attach(entity);
}

void generate_exit_game(cryo::csWorld *world)
{
  cryo::csEntity * entity = new cryo::csEntity();
  entity->Attach(new ExitGameState);
  world->Attach(entity);
}

void setup_world(cryo::csWorld *world)
{

  auto assetMan        = cryo::AssetManager::Get();
  auto material        = assetMan->Get<cryo::iMaterial>("/materials/Default.mat");
  auto skinnedMaterial = assetMan->Get<cryo::iMaterial>("/materials/DefaultSkinned.mat");
  cryo::s_material_names[material] = "Default";
  cryo::s_material_names[skinnedMaterial] = "Skinned";


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
                                           cryo::Vector3f(1.0f, 0.2f, 0.0f),
                                           cryo::ceDeg2Rad(-45.0f),
                                           cryo::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 0.9f,
                                           true,
                                           true);

  add_directional_light(world,
                        cryo::Vector3f(1.0f, 0.2f, 0.0f),
                        cryo::ceDeg2Rad(-45.0f),
                        cryo::Color4f(1.0f, 1.0f, 1.0f, 1.0f) * 0.1f,
                        true,
                        false);
#endif

//  add_point_light(world, cryo::Vector3f(0.0f, 5.0f, 0.0f), 50.0f, cryo::Color4f(1.0, 1.0f, 1.0f), false);

#if 0
  for (int i=-5; i<=5; i+= 1) {
    for (int j=-5; j<=5; j+= 1) {

      add_point_light(world,
                      cryo::Vector3f((float)i, 2.0f, (float)j), 5.0f, cryo::Color4f(1.0, 0.8f, 0.4f) * 0.1f, false);
    }

  }
//  add_point_light(world, cryo::Vector3f(10.0f, 10.0f, 10.0f), 10.0f, cryo::Color4f(1.0, 0.0f, 1.0f), false);
//  add_point_light(world, cryo::Vector3f(0.0f, 10.0f, 10.0f), 10.0f, cryo::Color4f(0.5, 0.0f, 1.0f), false);
//  add_point_light(world, cryo::Vector3f(-10.0f, 10.0f, 10.0f), 10.0f, cryo::Color4f(0.0, 0.0f, 1.0f), false);
//
//  add_point_light(world, cryo::Vector3f(10.0f, 10.0f, -10.0f), 25.0f, cryo::Color4f(1.0, 1.0f, 0.0f), false);
//  add_point_light(world, cryo::Vector3f(0.0f, 10.0f, -10.0f), 25.0f, cryo::Color4f(0.5, 1.0f, 0.0f), false);
//  add_point_light(world, cryo::Vector3f(-10.0f, 10.0f, -10.0f), 25.0f, cryo::Color4f(0.0, 1.0f, 0.0f), false);
#endif
}


cryo::iMaterial *create_sg_material ()
{
  auto sg = new cryo::csShaderGraph();
  sg->SetAlphaDiscard(0.5f, cryo::eCF_Never);

  auto color = sg->Add<cryo::csSGConstColor4>("diffuse");
  sg->BindDiffuse(color);

  color->SetValue(0.5f, 0.0f, 0.0f, 1.0f);

  cryo::iShaderGraphCompilerFactory *compilerFactory = cryo::ObjectRegistry::Get<cryo::iShaderGraphCompilerFactory>();
  if (compilerFactory)
  {
    cryo::iShaderGraphCompiler* compiler = compilerFactory->Create();
    if (compiler)
    {
      cryo::iShaderGraphCompiler::Parameters parameters {};
      memset (&parameters, 0, sizeof(parameters));
      cryo::csMaterial * material = compiler->Compile(sg, parameters);
      return material;
    }
  }

  return nullptr;
}

bool Game::Initialize(cryo::Engine *engine)
{

  cryo::AssetManager::Get()->Get<cryo::csMaterial>("/materials/Default.sg");



  setup_world(engine->GetWorld());
  return true;
}

