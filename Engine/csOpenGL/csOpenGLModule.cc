
#include <csOpenGL/csOpenGLModule.hh>
#include <master.refl.cc>

#include <csCore/csEngine.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/csSettings.hh>
#include <csCore/graphics/csSamplers.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csOpenGL/gl4/csGL4Device.hh>
#include <csOpenGL/gl4/csGL4RenderMesh.hh>
#include <csOpenGL/gl4/csGL4TerrainMeshGeneratorFactory.hh>
#include <csOpenGL/gl4/loading/csGL4ProgramLoader.hh>
#include <csOpenGL/gl4/loading/csGL4ShaderLoader.hh>
#include <csOpenGL/gl4/pipeline/forward/csGL4ForwardPipeline.hh>
#include <csOpenGL/gl4/pipeline/deferred/csGL4DeferredPipeline.hh>
#include <csOpenGL/gl4/shadergraph/csGL4ShaderGraphCompiler.hh>

namespace cs::opengl
{

bool csOpenGLModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();

  csAssetManager::Get()->RegisterLoader(new csGL4ProgramLoader());
  csAssetManager::Get()->RegisterLoader(new csGL4ShaderLoader());

  csGL4Device *device = new csGL4Device();
  csObjectRegistry::Register<iDevice>(device);
  engine->SetDevice(device);

  std::string renderPipline = csSettings::Get().Graphics().GetText("pipeline", "forward");
  if (renderPipline == std::string("forward"))
  {
    csObjectRegistry::Register<iRenderPipeline>(new csGL4ForwardPipeline());
  }
  else if (renderPipline == std::string("deferred"))
  {
    csObjectRegistry::Register<iRenderPipeline>(new csGL4DeferredPipeline());
  }
  else
  {
    return false;
  }

  csObjectRegistry::Register<iShaderGraphCompilerFactory>(new csGL4ShaderGraphCompilerFactory());
  csObjectRegistry::Register<iRenderMeshGeneratorFactory>(new csGL4RenderMeshGeneratorFactory());
  csObjectRegistry::Register<iRenderMeshBatchGeneratorFactory>(new csGL4RenderMeshBatchGeneratorFactory());
  csObjectRegistry::Register<iTerrainMeshGeneratorFactory>(new csGL4TerrainMeshGeneratorFactory());
  return true;
}

bool csOpenGLModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  csGL4Device * gl4Graphics = (csGL4Device*)engine->GetDevice();
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

CS_DEFINE_LIBRARY(csOpenGL)
{
  printf ("Load library: csOpenGL\n");
  return new cs::opengl::csOpenGLModule;
}