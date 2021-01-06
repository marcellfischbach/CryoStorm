
#include <spcOpenGL/gl4/loading/gl4shaderloader.hh>
#include <spcOpenGL/gl4/shading/gl4program.hh>
#include <spcOpenGL/gl4/shading/gl4shader.hh>
#include <spcOpenGL/gl4/gl4exceptions.hh>
#include <spcCore/resource/vfs.hh>
#include <spcCore/graphics/evertexstream.hh>
#include <tinyxml2/tinyxml2.h>
#include <regex>
#include <iostream>


using namespace tinyxml2;

namespace spc::opengl
{

GL4ShaderLoader::GL4ShaderLoader()
{
  SPC_CLASS_GEN_CONSTR;
}


bool GL4ShaderLoader::CanLoad(const Class* cls, const ResourceLocator& locator) const
{
  if (cls->IsAssignableFrom<GL4Program>())
  {
    return true;
  }
  return false;
}



std::vector<std::string> split(const std::string& string)
{
  std::vector<std::string> res;
  size_t offset = 0;
  size_t idx = 0;
  while ((idx = string.find('\n', offset)) != std::string::npos)
  {
    std::string part = string.substr(offset, idx - offset);
    res.push_back(part);
    offset = idx + 1;
  }
  std::string part = string.substr(offset, string.length() - offset);
  res.push_back(part);

  return res;
}

std::string merge(const std::vector<std::string>& lines)
{
  std::string res;
  for (const std::string& str : lines)
  {
    res += str + "\n";
  }
  return res;
}


void replace(std::vector<std::string>& lines, const std::string& vertexStreamName, eVertexStream stream)
{
  std::regex reg("(.*layout\\s*\\(location\\s*=\\s*)(" + vertexStreamName + ")(\\s*\\).*)");
  for (std::string& line : lines)
  {
    std::smatch sm;
    if (std::regex_match(line, sm, reg))
    {
      std::string part1 = sm[1];
      std::string part3 = sm[3];
      line = part1 + std::to_string(stream) + part3;
    }
  }

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
  iFile* file = VFS::Get()->Open(locator, eAM_Read, eOM_Text);
  if (!file)
  {
    return nullptr;
  }
  while (!file->IsEOF())
  {
    char buffer[2048];
    Size read = file->Read(sizeof(char), 2048, buffer);
    buffer[read] = '\0';
    source += buffer;
  }
  file->Close();

  std::cout << "Orig source:" << std::endl << source << std::endl;

  std::vector<std::string> lines = split(source);
  replace(lines, "eVS_Vertices", eVS_Vertices);
  replace(lines, "eVS_Normals", eVS_Normals);
  replace(lines, "eVS_Tangents", eVS_Tangents);
  replace(lines, "eVS_UV", eVS_UV);
  replace(lines, "eVS_UV0", eVS_UV0);
  replace(lines, "eVS_UV1", eVS_UV1);
  replace(lines, "eVS_UV2", eVS_UV2);
  replace(lines, "eVS_UV3", eVS_UV3);
  replace(lines, "eVS_UV", eVS_UV);
  replace(lines, "eVS_Colors", eVS_Colors);
  source = merge(lines);

  std::cout << "Replaced source:" << std::endl << source << std::endl;

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


iObject* GL4ShaderLoader::Load(const Class* cls, const ResourceLocator& locator) const
{

  XMLDocument doc;
  iFile* file = VFS::Get()->Open(locator, eAM_Read, eOM_Text);
  if (!file)
  {
    return nullptr;
  }

  char buffer[2048];
  Size read = file->Read(sizeof(char), 2048, buffer);
  buffer[read] = '\0';
  file->Close();


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
