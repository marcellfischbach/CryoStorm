
#include <spcOpenGL/gl4/loading/gl4shaderloader.hh>
#include <spcOpenGL/gl4/shading/gl4program.hh>
#include <spcOpenGL/gl4/shading/gl4shader.hh>
#include <spcOpenGL/gl4/gl4exceptions.hh>
#include <spcCore/resource/vfs.hh>
#include <spcCore/graphics/evertexstream.hh>
#include <spcCore/math/math.hh>
#include <spcCore/resource/file.hh>
#include <tinyxml2/tinyxml2.h>
#include <regex>
#include <iostream>
#include <set>

using namespace tinyxml2;

namespace spc::opengl
{

std::vector<std::string> replace_includes(const ResourceLocator* parent, std::vector<std::string>& lines,std::set<ResourceLocator>& included);

GL4ShaderLoader::GL4ShaderLoader()
{
  SPC_CLASS_GEN_CONSTR;
}


std::vector<std::string> split(const std::string& string)
{
  std::vector<std::string> res;
  size_t offset = 0;
  size_t idxn = 0;
  size_t idxr = 0;
  while (true)
  {
    idxn = string.find('\n', offset);
    idxr = string.find('\r', offset);

    if (idxn != std::string::npos && idxr != std::string::npos)
    {
      if (idxn + 1 == idxr || idxr + 1 == idxn)
      {
        Size min = spcMin(idxn, idxr);

        std::string part = string.substr(offset, min - offset);
        res.push_back(part);
        offset = min + 2;
      }
      else
      {
        Size min = spcMin(idxn, idxr);
        std::string part = string.substr(offset, min - offset);
        res.push_back(part);
        offset = min + 1;
      }
    }
    else if (idxn != std::string::npos || idxr != std::string::npos)
    {
      Size pos = idxn != std::string::npos ? idxn : idxr;
      std::string part = string.substr(offset, pos - offset);
      res.push_back(part);
      offset = pos + 1;
    }
    else
    {
      break;
    }

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
  std::regex reg("(.*layout\\s*\\(\\s*location\\s*=\\s*)(" + vertexStreamName + ")(\\s*\\).*)");
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

std::vector<std::string> loadExternalLines(const ResourceLocator& locator, std::set<ResourceLocator> &included)
{
  iFile *file = VFS::Get()->Open(locator, eAM_Read, eOM_Text);
  if (!file)
  {
    return std::vector<std::string>();
  }

  spc::file::File fFile;
  bool res = fFile.Parse(file);
  file->Release();
  if (!res)
  {
    return std::vector<std::string>();
  }

  file::Element* fragment = fFile.Root()->GetChild("fragment");
  if (!fragment || fragment->GetNumberOfAttributes() == 0)
  {
    return std::vector<std::string>();
  }

  std::string source = fragment->GetAttribute(0)->GetValue();
  std::vector<std::string> lines = split(source);
  return replace_includes(&locator, lines, included);
  
}

std::vector<std::string>  replace_includes(const ResourceLocator *parent, std::vector<std::string>& lines, std::set<ResourceLocator> &included)
{
  std::vector<std::string> result;
  std::regex reg("(#include\\s*\\<\\s*)([^\\>\\s]+)(\\s*\\>)");
  for (std::string& line : lines)
  {
    std::smatch sm;
    if (std::regex_match(line, sm, reg))
    {

      std::string part1 = sm[2];
      ResourceLocator locator(parent, part1);
      std::vector<std::string> loadedLines = loadExternalLines(locator, included);
      included.insert(locator);
      for (std::string& l : loadedLines)
      {
        result.push_back(l);
      }
    }
    else
    {
      result.push_back(line);
    }
  }
  return result;
}

GL4Shader* LoadShader(const std::string& typeText,
  const std::string& origSource,
  const ResourceLocator* locator)
{
  eGL4ShaderType shaderType;
  if (typeText == "vertex")
  {
    shaderType = eST_Vertex;
  }
  else if (typeText == "tessEval")
  {
    shaderType = eST_TessEval;
  }
  else if (typeText == "tessControl")
  {
    shaderType = eST_TessControl;
  }
  else if (typeText == "geometry")
  {
    shaderType = eST_Geometry;
  }
  else if (typeText == "fragment")
  {
    shaderType = eST_Fragment;
  }
  else if (typeText == "compute")
  {
    shaderType = eST_Compute;
  }
  else
  {
    return nullptr;
  }

  std::cout << "Orig source:" << std::endl << origSource << std::endl;

  std::vector<std::string> lines = split(origSource);
  std::set<ResourceLocator> included;

  lines = replace_includes(locator, lines, included);
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
  std::string source = merge(lines);

  std::cout << "Replaced source:" << std::endl << source << std::endl;

  try
  {
    GL4Shader* shader = new GL4Shader(shaderType);
    shader->SetSource(source);
    shader->Compile();
    return shader;
  }
  catch (GL4ShaderCompileException& sce)
  {
    printf("Unable to compile shader: %s\n%s\n", locator ? locator->GetLocator().c_str() : "unknown file", sce.what());
  }

  return nullptr;
}


bool GL4ShaderLoader::CanLoad(const Class* cls, const file::File* file, const ResourceLocator* locator) const
{
  return cls->IsAssignableFrom<GL4Shader>() && file->Root()->HasChild("shader");
}


iObject* GL4ShaderLoader::Load(const Class* cls, const file::File* file, const ResourceLocator* locator) const
{
  const file::Element* shaderElement = file->Root()->GetChild("shader");
  if (!shaderElement)
  {
    return nullptr;
  }


  return LoadShader(shaderElement->GetAttribute(0)->GetValue(),
    shaderElement->GetAttribute(1)->GetValue(),
    locator);

}


}
