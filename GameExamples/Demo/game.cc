//
// Created by MCEL on 08.05.2024.
//

#include "game.hh"
#include <csCore/graphics/shadergraph/iShaderGraphCompiler.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <csCore/graphics/shadergraph/csSGNodes.hh>
#include <csCore/csObjectRegistry.hh>

CS_DEFINE_GAME(Game)


#include <csLauncher/csLauncherModule.hh>
#include "exitgamestate.hh"
#include "testhandler.hh"
#include <csCore/csEngine.hh>
#include <csCore/csSettings.hh>
#include <csCore/csTicker.hh>
#include <csCore/animation/csSkeletonAnimation.hh>
#include <csCore/animation/csSkeletonAnimationPlayer.hh>
#include <csCore/entity/csCameraState.hh>
#include <csCore/entity/csCollisionState.hh>
#include <csCore/entity/csEntity.hh>
#include <csCore/entity/csLightState.hh>
#include <csCore/entity/csRigidBodyState.hh>
#include <csCore/entity/csSkeletonMeshState.hh>
#include <csCore/entity/csSpatialState.hh>
#include <csCore/entity/csStaticColliderState.hh>
#include <csCore/entity/csStaticMeshState.hh>
#include <csCore/entity/csTerrainMeshState.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/input/csInput.hh>
#include <csCore/math/csMath.hh>
#include <csCore/math/csColor4f.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iFrameRenderer.hh>
#include <csCore/graphics/csImage.hh>
#include <csCore/graphics/iPointLight.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/graphics/iRenderPipeline.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/graphics/iTerrainMesh.hh>
#include <csCore/graphics/csSkeletonMesh.hh>
#include <csCore/graphics/csMesh.hh>
#include <csCore/graphics/csPostProcessing.hh>
#include <csCore/graphics/skybox/csSimpleSkybox.hh>
#include <csCore/graphics/pp/csPP.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/physics/csPhysics.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/window/iWindow.hh>
#include <csCore/csTime.hh>


#include "../../GameExamples/Demo/camerahandler.hh"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>


cs::csLightState *shadowLightState = nullptr;




cs::iTerrainMesh *create_terrain_mesh(float size)
{
#define FLAT
  cs::iTerrainMeshGenerator *generator = cs::csObjectRegistry::Get<cs::iTerrainMeshGeneratorFactory>()->Create();

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

  generator->SetSize(cs::eTerrainSize::TS_129);
  generator->SetPatchSize(cs::eTerrainSize::TS_33);
  generator->SetSize(cs::eTerrainSize::TS_1025);
  generator->SetPatchSize(cs::eTerrainSize::TS_129);
  generator->SetNormalizedHeightData(heightData);
  generator->SetSize(cs::csVector3f(-size, 0.0f, -size), cs::csVector3f(size, 10.0f, size));

  cs::iTerrainMesh *terrainMesh = generator->Generate();
  generator->Release();
  return terrainMesh;
}

cs::iRenderMesh *create_sphere_mesh(float radius, uint32_t detail, float uv_f)
{
  cs::iRenderMeshGenerator    *generator = cs::csObjectRegistry::Get<cs::iRenderMeshGeneratorFactory>()->Create();
  std::vector<cs::csVector3f> positions;
  std::vector<cs::csVector3f> normals;
  std::vector<cs::csVector3f> tangents;
  std::vector<cs::csVector2f> uv;
  std::vector<cs::csColor4f>  colors;
  std::vector<uint32_t>        indices;

  for (uint32_t v = 0; v < detail; v++)
  {
    float factV  = (float) v / (float) (detail - 1);
    float angleV = -(float) M_PI_2 + factV * (float) M_PI;

    for (uint32_t h = 0; h < detail * 2; h++)
    {
      float factH  = (float) h / (float) (detail * 2 - 1);
      float angleH = factH * (float) M_PI * 2.0f;

      cs::csVector3f normal(
          cosf(angleV) * cosf(angleH),
          sinf(angleV),
          cosf(angleV) * sinf(angleH)
      );
      cs::csVector3f tangent(
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
  cs::iRenderMesh *renderMesh = generator->Generate();
  generator->Release();
  return renderMesh;

}


cs::iRenderMesh *
create_multi_sphere_mesh(float radius, uint32_t detail, float uv_f, size_t num_spheres, cs::csVector3f *sphere_positions)
{
  cs::iRenderMeshGenerator    *generator = cs::csObjectRegistry::Get<cs::iRenderMeshGeneratorFactory>()->Create();
  std::vector<cs::csVector3f> positions;
  std::vector<cs::csVector3f> normals;
  std::vector<cs::csVector3f> tangents;
  std::vector<cs::csVector2f> uv;
  std::vector<cs::csColor4f>  colors;
  std::vector<uint32_t>        indices;

  size_t      idxOrigin = 0;
  for (size_t i         = 0; i < num_spheres; i++)
  {
    cs::csVector3f origin = sphere_positions[i];
    for (uint32_t v      = 0; v < detail; v++)
    {
      float factV  = (float) v / (float) (detail - 1);
      float angleV = -(float) M_PI_2 + factV * (float) M_PI;

      for (uint32_t h = 0; h < detail * 2; h++)
      {
        float factH  = (float) h / (float) (detail * 2 - 1);
        float angleH = factH * (float) M_PI * 2.0f;

        cs::csVector3f normal(
            cosf(angleV) * cosf(angleH),
            sinf(angleV),
            cosf(angleV) * sinf(angleH)
        );
        cs::csVector3f tangent(
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
  cs::iRenderMesh *renderMesh = generator->Generate();
  generator->Release();
  return renderMesh;

}


void debug(cs::csSpatialState *state, int indent)
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
  for (cs::Size i = 0, in = state->GetNumberOfChildren(); i < in; i++)
  {
    debug(state->GetChild(i), indent + 1);
  }
}


cs::iRenderTarget2D *create_render_target(cs::iDevice *device, uint32_t width, uint32_t height, uint16_t multiSamples)
{
  cs::iSampler *colorSampler = device->CreateSampler();
  colorSampler->SetFilterMode(cs::eFM_MinMagNearest);

  cs::iSampler *depthSampler = device->CreateSampler();
  depthSampler->SetFilterMode(cs::eFM_MinMagNearest);
  depthSampler->SetTextureCompareFunc(cs::eCF_LessOrEqual);
  depthSampler->SetTextureCompareMode(cs::eTCM_None);

  cs::iTexture2D::Descriptor rt_col_desc = {};
  rt_col_desc.Width        = width;
  rt_col_desc.Height       = height;
  rt_col_desc.Format       = cs::ePF_RGBA;
  rt_col_desc.MipMaps      = false;
  rt_col_desc.MultiSamples = multiSamples;
  cs::iTexture2D *color_texture = device->CreateTexture(rt_col_desc);
  color_texture->SetSampler(colorSampler);

  cs::iTexture2D::Descriptor rt_dpth_desc = {};
  rt_dpth_desc.Width        = width;
  rt_dpth_desc.Height       = height;
  rt_dpth_desc.Format       = cs::ePF_DepthStencil;
  rt_dpth_desc.MipMaps      = false;
  rt_dpth_desc.MultiSamples = multiSamples;
  cs::iTexture2D *depth_texture = device->CreateTexture(rt_dpth_desc);
  depth_texture->SetSampler(depthSampler);
  printf("CreateDepthTexture: %p\n", depth_texture);


  cs::iRenderTarget2D::Descriptor rt_desc = {};
  rt_desc.Width  = width;
  rt_desc.Height = height;

  cs::iRenderTarget2D *renderTarget = device->CreateRenderTarget(rt_desc);
  renderTarget->AddColorTexture(color_texture);
//  renderTarget->SetDepthBuffer(cs::ePF_Depth);
  renderTarget->SetDepthTexture(depth_texture);
  if (!renderTarget->Compile())
  {
    printf("Unable to compile render target: %s\n", renderTarget->GetCompileLog().c_str());
    return nullptr;
  }
  return renderTarget;
}


cs::csPostProcessing* setup_post_processing()
{
  cs::csPostProcessing * postProcessing = nullptr;
#if 0
  
  postProcessing = new cs::PostProcessing();


  auto highPass = new cs::PPHighPass(0.92f);
  auto scaleDown0 = new cs::PPScaleDown();
  auto scaleDown1 = new cs::PPScaleDown();
  auto blurH = new cs::PPBlurH(30, 1.f);
  auto blurV = new cs::PPBlurV(30, 1.f);
  auto combine = new cs::PPCombine();

  postProcessing->AddProcess(highPass);
  postProcessing->AddProcess(scaleDown0);
  postProcessing->AddProcess(scaleDown1);
  postProcessing->AddProcess(blurH);
  postProcessing->AddProcess(blurV);
  postProcessing->AddProcess(combine);


  postProcessing->Bind({nullptr, (size_t)cs::PPImageType::Color, highPass, 0});
  postProcessing->Bind({highPass, 0, scaleDown0, 0});
  postProcessing->Bind({scaleDown0, 0, scaleDown1, 0});

  postProcessing->Bind({scaleDown1, 0, blurH, 0 });
  postProcessing->Bind({blurH, 0, blurV, 0});
  postProcessing->Bind({nullptr, (size_t)cs::PPImageType::Color, combine, 0});
  postProcessing->Bind({blurV, 0, combine, 1});
  postProcessing->Bind({combine, 0, nullptr, (size_t)cs::PPImageType::Color });


#endif
  return postProcessing;
}

void generate_camera(cs::csWorld *world)
{
  auto cameraEntity = new cs::csEntity("Camera");

  auto cameraState  = new cs::csCameraState();
  cameraState->SetClearMode(cs::eClearMode::DepthColor);
  cameraState->SetClearColor(cs::csColor4f(0.0f, 0.0f, 0.5f));
  cameraState->SetClearColorMode(cs::eClearColorMode::Skybox);
  cameraState->SetSkyboxRenderer(new cs::csSimpleSkybox());

  auto postProcessing = setup_post_processing ();
  cameraState->SetPostProcessing(postProcessing);


  auto cameraHandler = new CameraHandler();
  cameraEntity->Attach(cameraState);
  cameraEntity->Attach(cameraHandler);
  cameraEntity->GetRoot()->GetTransform()
              .SetTranslation(cs::csVector3f(-5, 5, -5))
              .LookAt(cs::csVector3f(0, 0, 0))
              .Finish();
  world->Attach(cameraEntity);
  world->SetMainCamera(cameraState);

//  auto mirrorCameraEntity = new cs::Entity("MirrorCamera");
//  auto mirrorCameraState  = new cs::CameraState();
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

void generate_terrain(cs::csWorld *world)
{
  cs::csAssetManager *assetMan = cs::csAssetManager::Get();

  auto greenGrassLayer = assetMan->Get<cs::csTerrainLayer>("/terrain/green_grass.terrainlayer");
  auto dirtLayer       = assetMan->Get<cs::csTerrainLayer>("/terrain/dirt.terrainlayer");
  auto fieldstoneLayer = assetMan->Get<cs::csTerrainLayer>("/terrain/fieldstone.terrainlayer");
  auto terrainLayers   = assetMan->Get<cs::csTerrainLayerMask>("/terrain/terrain.terrainmask");

  auto terrainMesh  = create_terrain_mesh(40.0f);
  auto entity0      = new cs::csEntity("Terrain");
  auto terrainState = new cs::csTerrainMeshState();
  terrainState->SetTerrainMesh(terrainMesh);
  terrainState->SetLayerMask(terrainLayers);
  terrainState->AddLayer(greenGrassLayer);
  terrainState->AddLayer(dirtLayer);
  terrainState->AddLayer(fieldstoneLayer);
  terrainState->GetTransform()
              .SetTranslation(cs::csVector3f(0, 0, 0))
              .Finish();
  terrainState->SetStatic(true);
  entity0->Attach(terrainState);
  world->Attach(entity0);
}


void generate_test_grid(cs::csWorld *world, cs::iMaterial *material)
{
  auto sphere = create_sphere_mesh(0.25, 16, 12.0f);
  auto mesh   = new cs::csMesh();
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
      auto entity = new cs::csEntity(std::string("Sphere: ") + std::to_string(i + 1) + ":" + std::to_string(j + 1));

      auto meshStateSphere = new cs::csStaticMeshState("Mesh");
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

          auto testHandler01 = new TestHandler01(cs::Vector3f(i - start, 0.25f, j - start), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler01);
          break;
        }
        case 1:
        {
          auto testHandler02 = new TestHandler02(cs::Vector3f(i - start, 0.25f, j - start), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler02);
          break;
        }
        case 2:
        {
          auto testHandler03 = new TestHandler03(cs::Vector3f(i - start, 0.25f, j - start), 0.25f, 0.5f + rnd);
          entity->Attach(testHandler03);
          break;
        }
        case 3:
        {
          auto testHandler04 = new TestHandler04(cs::Vector3f(i - start, 0.25f, j - start), 0.25f, 0.5f + rnd);
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

cs::csSkeleton          *global_skeleton  = nullptr;
cs::csSkeletonAnimation       *global_animation = nullptr;
cs::csSkeletonAnimationPlayer *global_player    = nullptr;

cs::csEntity *bones[4];

cs::csEntity *add_bone(cs::csWorld *world, cs::iMaterial *material)
{

  cs::csMesh *mesh = cs::csAssetManager::Get()->Load<cs::csMesh>("/bone_x.fbx");
  if (!mesh)
  {
    return nullptr;
  }

  cs::csEntity          *entity    = new cs::csEntity("Bone");
  cs::csStaticMeshState *meshState = new cs::csStaticMeshState();
  meshState->SetMesh(mesh);
  meshState->SetMaterial(0, material);
  entity->Attach(meshState);
  entity->GetRoot()->SetLocalMatrix(cs::csMatrix4f());

  world->Attach(entity);
  return entity;
}

void add_skeleton_mesh(cs::csWorld *world, cs::iMaterial *material)
{

  cs::csSkeletonMesh *mesh   = cs::csAssetManager::Get()->Load<cs::csSkeletonMesh>("/skinned_mesh.fbx");
  cs::csEntity       *entity = new cs::csEntity("Skeleton Entity");
  cs::csSkeletonMeshState *meshState = new cs::csSkeletonMeshState();
  meshState->SetMesh(mesh);
  meshState->SetMaterial(0, material);
  entity->Attach(meshState);

//  entity->GetRoot()->GetTransform()
//        .SetRotation(cs::Quaternion::FromAxisAngle(0.0f, 1.0f, 0.0f, M_PI / 1.0f * 1.0f))
//        .Finish();

//  world->Attach(entity);

  global_skeleton = &meshState->GetSkeleton();


  cs::csSkeletonAnimationPack
      *animationPack = cs::csAssetManager::Get()->Load<cs::csSkeletonAnimationPack>("/skinned_mesh.fbx");
  global_animation = animationPack->Get("Armature|MyAnimation01");
  global_animation->SetLoop(true);

  global_player = new cs::csSkeletonAnimationPlayer();
  global_player->SetSkeleton(global_skeleton);
  global_player->SetAnimation(global_animation);
}


void generate_batched_test_grid(cs::csWorld *world, cs::iMaterial *material)
{

  auto sphere = create_multi_sphere_mesh(0.25, 16, 12.0f, 25, new cs::csVector3f[] {
      cs::csVector3f(-2, 0.0f, -2.0f),
      cs::csVector3f(-1, 0.0f, -2.0f),
      cs::csVector3f(0, 0.0f, -2.0f),
      cs::csVector3f(1, 0.0f, -2.0f),
      cs::csVector3f(2, 0.0f, -2.0f),

      cs::csVector3f(-2, 0.0f, -1.0f),
      cs::csVector3f(-1, 0.0f, -1.0f),
      cs::csVector3f(0, 0.0f, -1.0f),
      cs::csVector3f(1, 0.0f, -1.0f),
      cs::csVector3f(2, 0.0f, -1.0f),

      cs::csVector3f(-2, 0.0f, 0.0f),
      cs::csVector3f(-1, 0.0f, 0.0f),
      cs::csVector3f(0, 0.0f, 0.0f),
      cs::csVector3f(1, 0.0f, 0.0f),
      cs::csVector3f(2, 0.0f, 0.0f),

      cs::csVector3f(-2, 0.0f, 1.0f),
      cs::csVector3f(-1, 0.0f, 1.0f),
      cs::csVector3f(0, 0.0f, 1.0f),
      cs::csVector3f(1, 0.0f, 1.0f),
      cs::csVector3f(2, 0.0f, 1.0f),

      cs::csVector3f(-2, 0.0f, 2.0f),
      cs::csVector3f(-1, 0.0f, 2.0f),
      cs::csVector3f(0, 0.0f, 2.0f),
      cs::csVector3f(1, 0.0f, 2.0f),
      cs::csVector3f(2, 0.0f, 2.0f)
  });
  auto mesh   = new cs::csMesh();
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
      auto entity = new cs::csEntity(std::string("Sphere: ") + std::to_string(i + 1) + ":" + std::to_string(j + 1));

      auto meshStateSphere = new cs::csStaticMeshState("Mesh");
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


void generate_physics(cs::csWorld *world, cs::iMaterial *material)
{

  cs::iPhysicsSystem *physics = cs::csObjectRegistry::Get<cs::iPhysicsSystem>();
  // add the ground plane
  /*
  cs::BoxShapeDesc floorDesc{ cs::Vector3f(100.0f, 1.0f, 100.0f) };
  cs::iCollisionShape* floorShape    = physics->CreateShape(floorDesc);
  cs::iStaticCollider* floorCollider = physics->CreateStaticCollider();
  floorCollider->Attach(floorShape);
  floorCollider->SetTransform(cs::Matrix4f::Translation(0.0f, -1.0f, 0.0f));
  physWorld->AddCollider(floorCollider);
  */

  cs::csEntity           *floorEntity      = new cs::csEntity("Floor");
  cs::csBoxColliderState    *floorBoxCollider    = new cs::csBoxColliderState();
  cs::csStaticColliderState *floorStaticCollider = new cs::csStaticColliderState();
  floorBoxCollider->SetHalfExtends(cs::csVector3f(100.0f, 1.0f, 100.0f));
  floorEntity->Attach(floorBoxCollider);
  floorEntity->Attach(floorStaticCollider);
  floorEntity->GetRoot()->GetTransform().SetTranslation(cs::csVector3f(0.0f, -1.0f, 0.0f)).Finish();
  world->Attach(floorEntity);

  float sphereRadius = 0.5f;
  if (false)
  {
    cs::iRenderMesh *renderMeshSphere = create_sphere_mesh(sphereRadius, 16, 4.0f);

    for (int i = 0; i < 10; i++)
    {
      {
        cs::csMesh   *meshSphere   = new cs::csMesh();
        cs::csEntity *entitySphere = new cs::csEntity("Sphere");
        cs::csStaticMeshState     *meshStateSphere     = new cs::csStaticMeshState("Mesh.Sphere");
        cs::csSphereColliderState *sphereColliderState = new cs::csSphereColliderState();
        cs::csRigidBodyState      *rigidBodyState      = new cs::csRigidBodyState("RigidBody.Sphere");


        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        sphereColliderState->SetRadius(sphereRadius);

        entitySphere->Attach(sphereColliderState);
        entitySphere->Attach(rigidBodyState);
        rigidBodyState->Attach(meshStateSphere);

        rigidBodyState->GetTransform()
                      .SetTranslation(cs::csVector3f(0.0f, sphereRadius * 2.5f, 0.0f) * ((float) i + 2.0f))
                      .Finish();
        meshStateSphere->SetMesh(meshSphere);
        world->Attach(entitySphere);



        /*
        cs::iDynamicCollider* sphereCollider = physics->CreateDynamicCollider();
        sphereCollider->Attach(sphereShape);
        sphereCollider->SetTransform(entitySphere->GetRoot()->GetGlobalMatrix());
        sphereCollider->SetUserData(meshStateSphere);
        //physWorld->AddCollider(sphereCollider);
         */
      }
      {
        cs::csMesh   *meshSphere   = new cs::csMesh();
        cs::csEntity *entitySphere = new cs::csEntity("Sphere");
        cs::csStaticMeshState *meshStateSphere = new cs::csStaticMeshState("Mesh.Sphere");
        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        meshStateSphere->GetTransform()
                       .SetTranslation(cs::csVector3f(i * sphereRadius * 0.5, 0.0f, 0.0f))
                       .Finish();
        meshStateSphere->SetMesh(meshSphere);
        entitySphere->Attach(meshStateSphere);
        world->Attach(entitySphere);
      }
      {
        cs::csMesh   *meshSphere   = new cs::csMesh();
        cs::csEntity *entitySphere = new cs::csEntity("Sphere");
        cs::csStaticMeshState *meshStateSphere = new cs::csStaticMeshState("Mesh.Sphere");
        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        meshStateSphere->GetTransform()
                       .SetTranslation(cs::csVector3f(0.0f, i * sphereRadius, 0.0f))
                       .Finish();
        meshStateSphere->SetMesh(meshSphere);
        entitySphere->Attach(meshStateSphere);
        world->Attach(entitySphere);
      }
      {
        cs::csMesh   *meshSphere   = new cs::csMesh();
        cs::csEntity *entitySphere = new cs::csEntity("Sphere");
        cs::csStaticMeshState *meshStateSphere = new cs::csStaticMeshState("Mesh.Sphere");
        meshSphere->AddMaterialSlot("Default", material);
        meshSphere->AddSubMesh(renderMeshSphere, 0);
        meshStateSphere->GetTransform()
                       .SetTranslation(cs::csVector3f(0.0f, 0.0f, i * 2.0f * sphereRadius))
                       .Finish();
        meshStateSphere->SetMesh(meshSphere);
        entitySphere->Attach(meshStateSphere);
        world->Attach(entitySphere);
      }
    }
  }


}



cs::csLightState *add_directional_light(cs::csWorld *world,
                                          const cs::csVector3f &axis,
                                          float rad,
                                          const cs::csColor4f &color,
                                          bool isStatic,
                                          bool castsShadow)
{
  cs::csEntity     *entity     = new cs::csEntity("Directional");
  cs::csLightState *lightState = new cs::csLightState("DirectionalLight");
  entity->Attach(lightState);
  lightState->SetType(cs::eLT_Directional);
  lightState->SetColor(color);
  lightState->SetShadowMapBias(0.003f);
  lightState->SetStatic(isStatic);
  lightState->SetCastShadow(castsShadow);
  lightState->GetTransform()
            .SetRotation(cs::csQuaternion::csQuaternion(axis.Normalized(), rad))
            .Finish();
  world->Attach(entity);
  const cs::csVector3f &direction = lightState->GetTransform().GetForward();
  printf ("%.2f %.2f %.2f\n", direction.x, direction.y, direction.z);
  return lightState;
}


cs::csLightState *add_point_light(cs::csWorld *world,
                                    const cs::csVector3f &position,
                                    float range,
                                    const cs::csColor4f &color,
                                    bool castsShadow)
{
  float rnd = (float) rand() / (float) RAND_MAX;

  auto entity     = new cs::csEntity("Point");
  auto lightState = new cs::csLightState("PointLight");
//  auto testState  = new TestHandler01(position, range * 0.25f, 0.5f * rnd);
  entity->Attach(lightState);
//  entity->Attach(testState);
  lightState->SetType(cs::eLT_Point);
  lightState->SetColor(color);
  lightState->SetRange(range);
  lightState->SetShadowMapBias(0.003f);
  lightState->SetStatic(false);
  lightState->SetCastShadow(castsShadow);
  lightState->GetTransform().SetTranslation(position).Finish();


  world->Attach(entity);
  return lightState;
}


cs::iMaterial *generate_color_material(const cs::csColor4f &color)
{
  auto sg = new cs::csShaderGraph();


  auto roughness = sg->Add<cs::csSGConstFloat>("Roughness");
  auto diffuse = sg->Add<cs::csSGConstColor3>("Diffuse");
  auto alpha = sg->Add<cs::csSGConstFloat>("Alpha");
  auto normal = sg->Add<cs::csSGConstColor3>("Normal");

  roughness->SetValue(1.0f);
  diffuse->SetValue(color.r, color.g, color.b);
  alpha->SetValue(1.0f);
  normal->SetValue(0.5f, 0.5f, 1.0f);



  sg->BindDiffuse(diffuse);
  sg->BindAlpha(alpha);
  sg->BindRoughness(roughness);
  sg->BindNormal(normal);
  sg->SetReceiveShadow(false);


  auto compilerFactory = cs::csObjectRegistry::Get<cs::iShaderGraphCompilerFactory>();
  if (compilerFactory)
  {
    cs::iShaderGraphCompiler* compiler = compilerFactory->Create();
    if (compiler)
    {
      cs::iShaderGraphCompiler::Parameters parameters {};
      memset (&parameters, 0, sizeof(parameters));
      return compiler->Compile(sg, parameters);
    }
  }

  return nullptr;
}



void generate_axis_grid(cs::csWorld *world)
{
  auto sphere = create_sphere_mesh(0.25, 16, 12.0f);
  auto matR   = cs::csAssetManager::Get()->Get<cs::iMaterial>("/materials/DefaultRed.matinstance");
  auto matG   = cs::csAssetManager::Get()->Get<cs::iMaterial>("/materials/DefaultGreen.matinstance");
  auto matB   = cs::csAssetManager::Get()->Get<cs::iMaterial>("/materials/DefaultBlue.matinstance");

//  matR = generate_color_material(cs::Color4f(0.5f, 0.0f, 0.0f));
//  matG = generate_color_material(cs::Color4f(0.0f, 0.5f, 0.0f));
//  matB = generate_color_material(cs::Color4f(0.0f, 0.0f, 0.5f));

  cs::s_material_names[matR] = "DefaultRed";
  cs::s_material_names[matG] = "DefaultGreen";
  cs::s_material_names[matB] = "DefaultBlue";

  auto meshR = new cs::csMesh();
  auto meshG = new cs::csMesh();
  auto meshB = new cs::csMesh();
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
      auto entity          = new cs::csEntity();
      auto meshStateSphere = new cs::csStaticMeshState("Mesh");
      meshStateSphere->SetStatic(true);
      meshStateSphere->GetTransform().SetTranslation((float) (i + 1) * 0.5f, 0.0f, 0.0f).Finish();
      meshStateSphere->SetMesh(meshR);
      entity->Attach(meshStateSphere);
      world->Attach(entity);
    }
    {
      auto entity          = new cs::csEntity();
      auto meshStateSphere = new cs::csStaticMeshState("Mesh");
      meshStateSphere->SetStatic(true);
      meshStateSphere->GetTransform().SetTranslation(0.0f, (float) (i + 1) * 0.5f, 0.0f).Finish();
      meshStateSphere->SetMesh(meshG);
      entity->Attach(meshStateSphere);
      world->Attach(entity);
    }
    {
      auto entity          = new cs::csEntity();
      auto meshStateSphere = new cs::csStaticMeshState("Mesh");
      meshStateSphere->SetStatic(true);
      meshStateSphere->GetTransform().SetTranslation(0.0f, 0.0f, (float) (i + 1) * 0.5f).Finish();
      meshStateSphere->SetMesh(meshB);
      entity->Attach(meshStateSphere);
      world->Attach(entity);
    }

  }
}

void generate_cube_fbx(cs::csWorld *world)
{

  cs::csMesh *mesh = cs::csAssetManager::Get()->Get<cs::csMesh>("/cube2.fbx");

  mesh = new cs::csMesh();
  mesh->AddSubMesh(create_sphere_mesh(3.0f, 32, 1.0f), 0);
  mesh->AddMaterialSlot("Default");
  cs::iMaterial *dustMaterial = cs::csAssetManager::Get()->Get<cs::iMaterial>("/materials/Dust.sg");
//  cs::iMaterial *dustMaterial = cs::AssetManager::Get()->Get<cs::iMaterial>("/materials/Dust.mat");
  cs::s_material_names[dustMaterial] = "Dust";

  for (int i = 0; i < mesh->GetNumberOfMaterialSlots(); ++i)
  {
    mesh->SetDefaultMaterial(i, dustMaterial);
  }

  auto entity          = new cs::csEntity();
  auto meshStateSphere = new cs::csStaticMeshState("Mesh");
  meshStateSphere->SetStatic(true);
  meshStateSphere->GetTransform().SetTranslation(5.0f, 0.0f, 0.4f).Finish();
  meshStateSphere->SetMesh(mesh);


  entity->Attach(meshStateSphere);
  world->Attach(entity);
}

void generate_exit_game(cs::csWorld *world)
{
  cs::csEntity * entity = new cs::csEntity();
  entity->Attach(new ExitGameState);
  world->Attach(entity);
}

void setup_world(cs::csWorld *world)
{

  auto assetMan        = cs::csAssetManager::Get();
  auto material        = assetMan->Get<cs::iMaterial>("/materials/Default.mat");
  auto skinnedMaterial = assetMan->Get<cs::iMaterial>("/materials/DefaultSkinned.mat");
  cs::s_material_names[material] = "Default";
  cs::s_material_names[skinnedMaterial] = "Skinned";


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
                                           cs::csVector3f(1.0f, 0.2f, 0.0f),
                                           cs::ceDeg2Rad(-45.0f),
                                           cs::csColor4f(1.0f, 1.0f, 1.0f, 1.0f) * 0.9f,
                                           true,
                                           true);

  add_directional_light(world,
                        cs::csVector3f(1.0f, 0.2f, 0.0f),
                        cs::ceDeg2Rad(-45.0f),
                        cs::csColor4f(1.0f, 1.0f, 1.0f, 1.0f) * 0.1f,
                        true,
                        false);
#endif

//  add_point_light(world, cs::Vector3f(0.0f, 5.0f, 0.0f), 50.0f, cs::Color4f(1.0, 1.0f, 1.0f), false);

#if 0
  for (int i=-5; i<=5; i+= 1) {
    for (int j=-5; j<=5; j+= 1) {

      add_point_light(world,
                      cs::Vector3f((float)i, 2.0f, (float)j), 5.0f, cs::Color4f(1.0, 0.8f, 0.4f) * 0.1f, false);
    }

  }
//  add_point_light(world, cs::Vector3f(10.0f, 10.0f, 10.0f), 10.0f, cs::Color4f(1.0, 0.0f, 1.0f), false);
//  add_point_light(world, cs::Vector3f(0.0f, 10.0f, 10.0f), 10.0f, cs::Color4f(0.5, 0.0f, 1.0f), false);
//  add_point_light(world, cs::Vector3f(-10.0f, 10.0f, 10.0f), 10.0f, cs::Color4f(0.0, 0.0f, 1.0f), false);
//
//  add_point_light(world, cs::Vector3f(10.0f, 10.0f, -10.0f), 25.0f, cs::Color4f(1.0, 1.0f, 0.0f), false);
//  add_point_light(world, cs::Vector3f(0.0f, 10.0f, -10.0f), 25.0f, cs::Color4f(0.5, 1.0f, 0.0f), false);
//  add_point_light(world, cs::Vector3f(-10.0f, 10.0f, -10.0f), 25.0f, cs::Color4f(0.0, 1.0f, 0.0f), false);
#endif
}


cs::iMaterial *create_sg_material ()
{
  auto sg = new cs::csShaderGraph();
  sg->SetAlphaDiscard(0.5f, cs::eCF_Never);

  auto color = sg->Add<cs::csSGConstColor4>("diffuse");
  sg->BindDiffuse(color);

  color->SetValue(0.5f, 0.0f, 0.0f, 1.0f);

  cs::iShaderGraphCompilerFactory *compilerFactory = cs::csObjectRegistry::Get<cs::iShaderGraphCompilerFactory>();
  if (compilerFactory)
  {
    cs::iShaderGraphCompiler* compiler = compilerFactory->Create();
    if (compiler)
    {
      cs::iShaderGraphCompiler::Parameters parameters {};
      memset (&parameters, 0, sizeof(parameters));
      cs::csMaterial * material = compiler->Compile(sg, parameters);
      return material;
    }
  }

  return nullptr;
}

bool Game::Initialize(cs::csEngine *engine)
{

  cs::csAssetManager::Get()->Get<cs::csMaterial>("/materials/Default.sg");



  setup_world(engine->GetWorld());
  return true;
}

