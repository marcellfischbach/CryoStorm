
#include <spcOpenGL/openglmodule.hh>
#include <master.refl.cc>

#include <spcCore/objectregistry.hh>
#include <spcCore/graphics/samplers.hh>
#include <spcCore/resource/assetmanager.hh>
#include <spcOpenGL/gl4/gl4device.hh>
#include <spcOpenGL/gl4/gl4rendermesh.hh>
#include <spcOpenGL/gl4/loading/gl4programloader.hh>
#include <spcOpenGL/gl4/loading/gl4shaderloader.hh>
#include <spcOpenGL/gl4/pipeline/forward/gl4forwardpipeline.hh>

namespace spc::opengl
{
bool OpenGLModule::Register(int argc, char** argv)
{
  register_classes();

  AssetManager::Get()->RegisterLoader(new GL4ProgramLoader());
  AssetManager::Get()->RegisterLoader(new GL4ShaderLoader());
  ObjectRegistry::Register<iDevice>(new GL4Device());
  ObjectRegistry::Register<iRenderPipeline>(new GL4ForwardPipeline());
  ObjectRegistry::Register<iRenderMeshGeneratorFactory>(new GL4RenderMeshGeneratorFactory());
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

  return initialized;
}


}