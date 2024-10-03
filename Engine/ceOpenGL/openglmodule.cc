
#include <ceOpenGL/openglmodule.hh>
#include <master.refl.cc>

#include <ceCore/engine.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/settings.hh>
#include <ceCore/graphics/samplers.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4rendermesh.hh>
#include <ceOpenGL/gl4/gl4terrainmesh.hh>
#include <ceOpenGL/gl4/loading/gl4programloader.hh>
#include <ceOpenGL/gl4/loading/gl4shaderloader.hh>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwardpipeline.hh>
#include <ceOpenGL/gl4/pipeline/deferred/gl4deferredpipeline.hh>
#include <ceOpenGL/gl4/shadergraph/gl4shadergraphcompiler.hh>

namespace cryo::opengl
{

bool OpenGLModule::Register(const std::vector<std::string> &args, Engine *engine)
{
  register_classes();

  AssetManager::Get()->RegisterLoader(new GL4ProgramLoader());
  AssetManager::Get()->RegisterLoader(new GL4ShaderLoader());

  GL4Device *device = new GL4Device();
  ObjectRegistry::Register<iDevice>(device);
  engine->SetDevice(device);

  std::string renderPipline = Settings::Get().Graphics().GetText("pipeline", "forward");
  if (renderPipline == std::string("forward"))
  {
    ObjectRegistry::Register<iRenderPipeline>(new GL4ForwardPipeline());
  }
  else if (renderPipline == std::string("deferred"))
  {
    ObjectRegistry::Register<iRenderPipeline>(new GL4DeferredPipeline());
  }
  else
  {
    return false;
  }

  ObjectRegistry::Register<iShaderGraphCompilerFactory>(new GL4ShaderGraphCompilerFactory());
  ObjectRegistry::Register<iRenderMeshGeneratorFactory>(new GL4RenderMeshGeneratorFactory());
  ObjectRegistry::Register<iRenderMeshBatchGeneratorFactory>(new GL4RenderMeshBatchGeneratorFactory());
  ObjectRegistry::Register<iTerrainMeshGeneratorFactory>(new GL4TerrainMeshGeneratorFactory());
  return true;
}

bool OpenGLModule::Initialize(const std::vector<std::string> &args, Engine *engine)
{
  GL4Device* gl4Graphics = (GL4Device*)engine->GetDevice();
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

CS_DEFINE_LIBRARY(ceOpenGL)
{
  printf ("Load library: ceOpenGL\n");
  return new cryo::opengl::OpenGLModule;
}