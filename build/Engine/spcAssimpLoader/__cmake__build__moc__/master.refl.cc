

#include <stdarg.h>
#include "assimpmeshloader.hh"
#include "assimpmeshloader.refl.hh"


#include "assimpmeshloader.refl.cc"


#include <spcCore/classregistry.hh>

static void register_classes()
{
  spc::ClassRegistry *reg = spc::ClassRegistry::Get();
  // assimpmeshloader.hh
  reg->Register(spc::assimp::AssimpMeshLoaderClass::Get());
}

static void unregister_classes()
{
  spc::ClassRegistry *reg = spc::ClassRegistry::Get();
  // assimpmeshloader.hh
  reg->Unregister(spc::assimp::AssimpMeshLoaderClass::Get());
}

