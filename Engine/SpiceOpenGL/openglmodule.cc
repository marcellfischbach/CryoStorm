
#include <SpiceOpenGL/openglmodule.hh>
#include <SpiceOpenGL/master.refl.cc>
#include <SpiceCore/objectregistry.hh>
#include <SpiceOpenGL/gl4/gl4graphics.hh>

namespace Spice
{

bool OpenGLModule::Register(int argc, char** argv)
{
  register_classes();

  ObjectRegistry::Register<iGraphics>(new GL4Graphics());
  return true;
}

bool OpenGLModule::Initialize(int argc, char** argv)
{
  GL4Graphics* gl4Graphics = ObjectRegistry::Get<iGraphics>()->Query<GL4Graphics>();
  return gl4Graphics->Initialize();
}


}