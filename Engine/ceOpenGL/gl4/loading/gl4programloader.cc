
#include <ceOpenGL/gl4/loading/gl4programloader.hh>
#include <ceOpenGL/gl4/shading/gl4program.hh>
#include <ceOpenGL/gl4/shading/gl4shader.hh>
#include <ceOpenGL/gl4/gl4exceptions.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/resource/resourcelocator.hh>
#include <ceCore/resource/vfs.hh>

#include <tinyxml2/tinyxml2.h>


using namespace tinyxml2;

namespace ce::opengl
{
GL4ProgramLoader::GL4ProgramLoader()
  : BaseCEFAssetLoader()
{
  AddValidFile(GL4Program::GetStaticClass(), "SHADER");
}

iObject* GL4ProgramLoader::Load(const CrimsonFile* file, const Class* cls, const ResourceLocator& locator) const
{

  const CrimsonFileElement* programElement = file->Root()->GetChild("program");
  if (!programElement)
  {
    return nullptr;
  }

  const CrimsonFileElement* shadersElement = programElement->GetChild("shaders");
  if (!shadersElement)
  {
    return nullptr;
  }

  GL4Program* program = new GL4Program();
  for (Size i = 0, in = shadersElement->GetNumberOfChildren(); i < in; i++)
  {
    const CrimsonFileElement* shaderElement = shadersElement->GetChild(i);
    if (shaderElement && shaderElement->GetTagName() == "shader")
    {
      ResourceLocator shaderResourceLocator = ResourceLocator(locator, shaderElement->GetAttribute(0)->GetValue());

      GL4Shader* shader = AssetManager::Get()->Load<GL4Shader>(shaderResourceLocator);
      program->AttachShader(shader);
    }
  }
  try
  {
    program->Link();
  }
  catch (GL4ProgramLinkException& ple)
  {
    printf("Unable to link program [%s]:\n%s\n", locator.Encoded().c_str(), ple.what());
    program->Release();
    return nullptr;
  }

  const CrimsonFileElement* attributesElement = programElement->GetChild("attributes");
  if (attributesElement)
  {
    for (Size i = 0, in = attributesElement->GetNumberOfChildren(); i < in; i++)
    {
      const CrimsonFileElement* attributeElement = attributesElement->GetChild(i);
      if (attributeElement && attributeElement->GetTagName() == std::string("attribute")
        && attributeElement->GetNumberOfAttributes() >= 1)
      {
        program->RegisterAttribute(attributeElement->GetAttribute(0)->GetValue());
      }
    }
  }

  return program;

}

}