
#include <ceOpenGL/gl4/loading/gl4programloader.hh>
#include <ceOpenGL/gl4/shading/gl4program.hh>
#include <ceOpenGL/gl4/shading/gl4shader.hh>
#include <ceOpenGL/gl4/gl4exceptions.hh>
#include <ceCore/resource/csAssetManager.hh>
#include <ceCore/resource/csResourceLocator.hh>
#include <ceCore/resource/csVFS.hh>

#include <tinyxml2/tinyxml2.h>


using namespace tinyxml2;

namespace cryo::opengl
{
GL4ProgramLoader::GL4ProgramLoader()
  : csBaseCSFAssetLoader()
{
  AddValidFile(GL4Program::GetStaticClass(), "SHADER");
}

iObject* GL4ProgramLoader::Load(const csCryoFile* file, const csClass* cls, const csResourceLocator& locator) const
{

  const csCryoFileElement * programElement = file->Root()->GetChild("program");
  if (!programElement)
  {
    return nullptr;
  }

  const csCryoFileElement * shadersElement = programElement->GetChild("shaders");
  if (!shadersElement)
  {
    return nullptr;
  }

  GL4Program* program = new GL4Program();
  for (Size i = 0, in = shadersElement->GetNumberOfChildren(); i < in; i++)
  {
    const csCryoFileElement * shaderElement = shadersElement->GetChild(i);
    if (shaderElement && shaderElement->GetTagName() == "shader")
    {
      csResourceLocator shaderResourceLocator = csResourceLocator(locator, shaderElement->GetAttribute(0)->GetValue());

      GL4Shader* shader = csAssetManager::Get()->Load<GL4Shader>(shaderResourceLocator);
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

  const csCryoFileElement * attributesElement = programElement->GetChild("attributes");
  if (attributesElement)
  {
    for (Size i = 0, in = attributesElement->GetNumberOfChildren(); i < in; i++)
    {
      const csCryoFileElement * attributeElement = attributesElement->GetChild(i);
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