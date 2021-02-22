
#include <spcOpenGL/gl4/loading/gl4programloader.hh>
#include <spcOpenGL/gl4/shading/gl4program.hh>
#include <spcOpenGL/gl4/shading/gl4shader.hh>
#include <spcOpenGL/gl4/gl4exceptions.hh>
#include <spcCore/resource/assetmanager.hh>
#include <spcCore/resource/resourcelocator.hh>
#include <spcCore/resource/vfs.hh>

#include <tinyxml2/tinyxml2.h>


using namespace tinyxml2;

namespace spc::opengl
{
GL4ProgramLoader::GL4ProgramLoader()
: iAssetLoaderSpc()
{
  SPC_CLASS_GEN_CONSTR;
}

bool GL4ProgramLoader::CanLoad(const Class* cls, const file::File* file, const ResourceLocator* locator) const
{
  return GL4Program::GetStaticClass()->IsInstanceOf(cls)
         && file->Root()->HasChild("program");

}

iObject* GL4ProgramLoader::Load(const Class* cls, const file::File* file, const ResourceLocator* locator) const
{

  const file::Element* programElement = file->Root()->GetChild("program");
  if (!programElement)
  {
    return nullptr;
  }

  const file::Element* shadersElement = programElement->GetChild("shaders");
  if (!shadersElement)
  {
    return nullptr;
  }

  GL4Program* program = new GL4Program();
  for (Size i=0, in=shadersElement->GetNumberOfChildren(); i<in; i++)
  {
    const file::Element* shaderElement = shadersElement->GetChild(i);
    if (shaderElement && shaderElement->GetTagName() == "shader")
    {
      GL4Shader *shader = AssetManager::Get()->Load<GL4Shader>(ResourceLocator(shaderElement->GetAttribute(0)->GetValue()));
      program->AttachShader(shader);
    }
  }
  try
  {
    program->Link();
  }
  catch (GL4ProgramLinkException& ple)
  {
    printf("Unable to link program:\n%s\n", ple.what());
    program->Release();
    return nullptr;
  }

  const file::Element* attributesElement = programElement->GetChild("attributes");
  if (attributesElement)
  {
    for (Size i=0, in=attributesElement->GetNumberOfChildren(); i<in; i++)
    {
      const file::Element* attributeElement = attributesElement->GetChild(i);
      if (attributeElement && attributeElement->GetTagName() == std::string ("attribute") && attributeElement->GetNumberOfAttributes () >= 1)
      {
        program->RegisterAttribute(attributeElement->GetAttribute(0)->GetValue());
      }
    }
  }

  return program;

}
}