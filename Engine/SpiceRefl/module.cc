

#include <SpiceRefl/module.hh>
#include <SpiceRefl/class.hh>
#include <SpiceRefl/classregistry.hh>

namespace Spice
{

void Module::Initialize()
{
  ClassRegistry::Get()->Register(Object::GetStaticClass());
}

}
