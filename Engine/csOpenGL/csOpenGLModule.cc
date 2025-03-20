
#include <csOpenGL/csOpenGLModule.hh>
#include <master.refl.cc>

#include <csCore/csEngine.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/csSettings.hh>
#include <csCore/graphics/csSamplers.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csVFS.hh>
#include <csOpenGL/gl4/csGL4Device.hh>
#include <csOpenGL/gl4/csGL4RenderMesh.hh>
#include <csOpenGL/gl4/csGL4TerrainMeshGeneratorFactory.hh>
#include <csOpenGL/gl4/loading/csGL4ProgramLoader.hh>
#include <csOpenGL/gl4/loading/csGL4ShaderLoader.hh>
#include <csOpenGL/gl4/pipeline/csGL4FrameRenderer.hh>
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
  csObjectRegistry::Register<iFrameRendererFactory>(new csGL4FrameRendererFactory);


  std::string renderingApi = "gl4";

  bool        compatMode = false;
  for (size_t i          = 0, in = args.size(); i < in; i++)
  {
    if (args[i] == "--glProfile" && (i + 1) < in)
    {
      i++;
      if (args[i] == "compat" || args[i] == "compatibility")
      {
        printf("Initialize OpenGL in compatibility mode\n");
        fflush(stdout);
        compatMode = true;
      }
    }
    if (args[i] == "--glApi" && (i + 1) < in)
    {
      i++;
      renderingApi = args[i];
    }
  }

  csObjectRegistry::Register<iShaderGraphCompilerFactory>(new csGL4ShaderGraphCompilerFactory());
  csObjectRegistry::Register<iRenderMeshGeneratorFactory>(new csGL4RenderMeshGeneratorFactory(compatMode));
  csObjectRegistry::Register<iRenderMeshBatchGeneratorFactory>(new csGL4RenderMeshBatchGeneratorFactory(compatMode));
  csObjectRegistry::Register<iTerrainMeshGeneratorFactory>(new csGL4TerrainMeshGeneratorFactory(compatMode));

  csVFS::Get()->SetRenderingApi(renderingApi);

  return true;
}

bool csOpenGLModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  auto gl4Graphics = (csGL4Device *) csObjectRegistry::Get<iDevice>();
  bool initialized = gl4Graphics->Initialize();
  if (initialized)
  {
    csRef<csSamplers> samplers = new csSamplers();
    samplers->Load();

    csObjectRegistry::Register<csSamplers>(samplers);

  }


  return initialized;
}

void csOpenGLModule::Shutdown(const std::vector<std::string> &args, cs::csEngine *engine)
{
  auto gl4Graphics = (csGL4Device *) csObjectRegistry::Get<iDevice>();
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