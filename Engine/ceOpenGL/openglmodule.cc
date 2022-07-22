
#include <ceOpenGL/openglmodule.hh>
#include <master.refl.cc>

#include <ceCore/objectregistry.hh>
#include <ceCore/graphics/samplers.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4rendermesh.hh>
#include <ceOpenGL/gl4/gl4terrainmesh.hh>
#include <ceOpenGL/gl4/loading/gl4programloader.hh>
#include <ceOpenGL/gl4/loading/gl4shaderloader.hh>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwardpipeline.hh>

namespace ce::opengl
{

bool OpenGLModule::Register(int argc, char** argv)
{
  register_classes();

  AssetManager::Get()->RegisterLoader(new GL4ProgramLoader());
  AssetManager::Get()->RegisterLoader(new GL4ShaderLoader());
  ObjectRegistry::Register<iDevice>(new GL4Device());
  ObjectRegistry::Register<iRenderPipeline>(new GL4ForwardPipeline());
  ObjectRegistry::Register<iRenderMeshGeneratorFactory>(new GL4RenderMeshGeneratorFactory());
  ObjectRegistry::Register<iRenderMeshBatchGeneratorFactory>(new GL4RenderMeshBatchGeneratorFactory());
  ObjectRegistry::Register<iTerrainMeshGeneratorFactory>(new GL4TerrainMeshGeneratorFactory());
  return true;
}

bool OpenGLModule::Initialize(int argc, char** argv)
{
  GL4Device* gl4Graphics = ObjectRegistry::Get<iDevice>()->Query<GL4Device>();
  bool initialized = gl4Graphics->Initialize();
  if (initialized) 
  {
    Samplers* samplers = new Samplers();
    samplers->Load();

    ObjectRegistry::Register<Samplers>(samplers);

  }

  iRenderPipeline* pipeline = ObjectRegistry::Get<iRenderPipeline>();
  if (pipeline)
  {
    pipeline->Initialize();
  }

  return initialized;
}


}