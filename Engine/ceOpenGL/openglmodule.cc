
#include <ceOpenGL/openglmodule.hh>
#include <master.refl.cc>

#include <ceCore/csEngine.hh>
#include <ceCore/csObjectRegistry.hh>
#include <ceCore/csSettings.hh>
#include <ceCore/graphics/csSamplers.hh>
#include <ceCore/resource/csAssetManager.hh>
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

bool OpenGLModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();

  csAssetManager::Get()->RegisterLoader(new GL4ProgramLoader());
  csAssetManager::Get()->RegisterLoader(new GL4ShaderLoader());

  GL4Device *device = new GL4Device();
  csObjectRegistry::Register<iDevice>(device);
  engine->SetDevice(device);

  std::string renderPipline = csSettings::Get().Graphics().GetText("pipeline", "forward");
  if (renderPipline == std::string("forward"))
  {
    csObjectRegistry::Register<iRenderPipeline>(new GL4ForwardPipeline());
  }
  else if (renderPipline == std::string("deferred"))
  {
    csObjectRegistry::Register<iRenderPipeline>(new GL4DeferredPipeline());
  }
  else
  {
    return false;
  }

  csObjectRegistry::Register<iShaderGraphCompilerFactory>(new GL4ShaderGraphCompilerFactory());
  csObjectRegistry::Register<iRenderMeshGeneratorFactory>(new GL4RenderMeshGeneratorFactory());
  csObjectRegistry::Register<iRenderMeshBatchGeneratorFactory>(new GL4RenderMeshBatchGeneratorFactory());
  csObjectRegistry::Register<iTerrainMeshGeneratorFactory>(new GL4TerrainMeshGeneratorFactory());
  return true;
}

bool OpenGLModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  GL4Device* gl4Graphics = (GL4Device*)engine->GetDevice();
  bool initialized = gl4Graphics->Initialize();
  if (initialized) 
  {
    csSamplers * samplers = new csSamplers();
    samplers->Load();

    csObjectRegistry::Register<csSamplers>(samplers);

  }


  iRenderPipeline* pipeline = csObjectRegistry::Get<iRenderPipeline>();
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