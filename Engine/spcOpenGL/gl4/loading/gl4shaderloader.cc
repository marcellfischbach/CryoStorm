
#include <spcOpenGL/gl4/loading/gl4shaderloader.hh>
#include <spcOpenGL/gl4/shading/gl4program.hh>
#include <spcOpenGL/gl4/shading/gl4shader.hh>
#include <spcOpenGL/gl4/gl4exceptions.hh>
#include <spcCore/resource/vfs.hh>
#include <tinyxml2/tinyxml2.h>


using namespace tinyxml2;

namespace spc
{

GL4ShaderLoader::GL4ShaderLoader()
{
  SPC_CLASS_GEN_CONSTR;
}


GL4Shader* LoadShader(const std::string& typeText, const ResourceLocator& locator)
{
  eGL4ShaderType shaderType;
  if (typeText == "vertex") shaderType = eST_Vertex;
  else if (typeText == "tessEval") shaderType = eST_TessEval;
  else if (typeText == "tessControl") shaderType = eST_TessControl;
  else if (typeText == "geometry") shaderType = eST_Geometry;
  else if (typeText == "fragment") shaderType = eST_Fragment;
  else if (typeText == "compute") shaderType = eST_Compute;
  else return nullptr;


  std::string source;
  FILE* file = VFS::Get()->Open(locator, eAM_Read, eOM_Text);
  while (!feof(file))
  {
    char buffer[2048];
    size_t read = fread(buffer, sizeof(char), 2048, file);
    buffer[read] = '\0';
    source += buffer;
  }

  fclose(file);

  try 
  {
    GL4Shader* shader = new GL4Shader(shaderType);
    shader->SetSource(source);
    shader->Compile();
    return shader;
  }
  catch (GL4ShaderCompileException &sce)
  {
    printf("Unable to compile shader: %s\n%s\n", locator.GetLocator().c_str(), sce.what());
  }

  return nullptr;
}


iShader* GL4ShaderLoader::Load(const ResourceLocator& locator)
{

  XMLDocument doc;
  FILE* file = VFS::Get()->Open(locator, eAM_Read, eOM_Text);
  char buffer[2048];
  size_t read = fread(buffer, sizeof(char), 2048, file);
  buffer[read] = '\0';
  fclose(file);


  GL4Program* program = new GL4Program();
  XMLError error = doc.Parse(buffer);
  if (error != XML_SUCCESS)
  {
    printf("Unable to parse document:\n%s\n", doc.ErrorStr());
    return nullptr;
  }
  XMLElement* element = doc.FirstChildElement();
  for (XMLElement* shaderElement = element->FirstChildElement("shader");
    shaderElement;
    shaderElement = shaderElement->NextSiblingElement("shader"))
  {
    GL4Shader* shader = LoadShader(shaderElement->FindAttribute("type")->Value(), ResourceLocator(shaderElement->GetText()));
    program->AttachShader(shader);
  }

  try
  {
    program->Link();
  }
  catch (GL4ProgramLinkException& ple)
  {
    printf("Unable to link program:\n%s\n", ple.what());
  }

  return program;
}





}
