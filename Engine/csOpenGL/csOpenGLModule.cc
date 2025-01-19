
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
#include <csOpenGL/gl4/pipeline/csGL4DefaultFrameRenderer.hh>
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

  csObjectRegistry::Register<iDevice>(new csGL4Device());
  csObjectRegistry::Register<iFrameRenderer>(new csGL4DefaultFrameRenderer);

  std::string renderPipeline = csSettings::Get().Graphics().GetText("pipeline", "forward");
  if (renderPipeline == std::string("forward"))
  {
    csObjectRegistry::Register<iRenderPipeline>(new csGL4ForwardPipeline());
  }
  else if (renderPipeline == std::string("deferred"))
  {
    csObjectRegistry::Register<iRenderPipeline>(new csGL4DeferredPipeline());
  }
  else
  {
    return false;
  }

  bool compatMode = false;
  for (size_t i=0, in=args.size(); i<in; i++)
  {
    if (args[i] == "--glProfile" && (i+1) < in)
    {
      i++;
      if (args[i] == "compat" || args[i] == "compatibility")
      {
        compatMode = true;
      }
    }
  }

  csObjectRegistry::Register<iShaderGraphCompilerFactory>(new csGL4ShaderGraphCompilerFactory());
  csObjectRegistry::Register<iRenderMeshGeneratorFactory>(new csGL4RenderMeshGeneratorFactory(compatMode));
  csObjectRegistry::Register<iRenderMeshBatchGeneratorFactory>(new csGL4RenderMeshBatchGeneratorFactory(compatMode));
  csObjectRegistry::Register<iTerrainMeshGeneratorFactory>(new csGL4TerrainMeshGeneratorFactory(compatMode));
  return true;
}

bool csOpenGLModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  auto gl4Graphics = (csGL4Device*)csObjectRegistry::Get<iDevice>();
  bool initialized = gl4Graphics->Initialize();
  if (initialized) 
  {
    csRef<csSamplers> samplers = new csSamplers();
    samplers->Load();

    csObjectRegistry::Register<csSamplers>(samplers);

  }


  auto pipeline = csObjectRegistry::Get<iRenderPipeline>();
  if (pipeline)
  {
    pipeline->Initialize();
  }

  return initialized;
}

void csOpenGLModule::Shutdown(const std::vector<std::string> &args, cs::csEngine *engine)
{
  auto gl4Graphics = (csGL4Device*)csObjectRegistry::Get<iDevice>();
  auto pipeline = csObjectRegistry::Get<iRenderPipeline>();
  pipeline->Shutdown();
  gl4Graphics->Shutdown();

  csObjectRegistry::Remove<iRenderPipeline>();

  csObjectRegistry::Remove<iFrameRenderer>();

  csObjectRegistry::Remove<csSamplers>();
  csObjectRegistry::Remove<iTerrainMeshGeneratorFactory>();
  csObjectRegistry::Remove<iRenderMeshBatchGeneratorFactory>();
  csObjectRegistry::Remove<iRenderMeshGeneratorFactory>();
  csObjectRegistry::Remove<iShaderGraphCompilerFactory>();
  csObjectRegistry::Remove<iDevice>();
}

}

CS_DEFINE_LIBRARY(csOpenGL)
{
  return new cs::opengl::csOpenGLModule;
}