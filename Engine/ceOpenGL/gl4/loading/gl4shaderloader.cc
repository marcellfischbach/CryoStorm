
#include <ceOpenGL/gl4/loading/gl4shaderloader.hh>
#include <ceOpenGL/gl4/shading/gl4program.hh>
#include <ceOpenGL/gl4/shading/gl4shader.hh>
#include <ceOpenGL/gl4/gl4exceptions.hh>
#include <ceCore/resource/csVFS.hh>
#include <ceCore/graphics/eVertexStream.hh>
#include "ceCore/math/csMath.hh"
#include <ceCore/resource/csCryoFile.hh>
#include <regex>
#include <iostream>
#include <set>


namespace cryo::opengl
{

std::vector<std::string> replace_includes(const csResourceLocator *parent,
                                          std::vector<std::string> &lines,
                                          std::set<csResourceLocator> &included);


std::vector<std::string> split(const std::string &string)
{
  std::vector<std::string> res;
  size_t                   offset = 0;
  size_t                   idxn   = 0;
  size_t                   idxr   = 0;
  while (true)
  {
    idxn = string.find('\n', offset);
    idxr = string.find('\r', offset);

    if (idxn != std::string::npos && idxr != std::string::npos)
    {
      if (idxn + 1 == idxr || idxr + 1 == idxn)
      {
        Size min = ceMin(idxn, idxr);

        std::string part = string.substr(offset, min - offset);
        res.push_back(part);
        offset = min + 2;
      }
      else
      {
        Size        min  = ceMin(idxn, idxr);
        std::string part = string.substr(offset, min - offset);
        res.push_back(part);
        offset = min + 1;
      }
    }
    else if (idxn != std::string::npos || idxr != std::string::npos)
    {
      Size        pos  = idxn != std::string::npos ? idxn : idxr;
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

std::string merge(const std::vector<std::string> &lines)
{
  std::string            res;
  for (const std::string &str: lines)
  {
    res += str + "\n";
  }
  return res;
}


void replace(std::vector<std::string> &lines, const std::string &vertexStreamName, eVertexStream stream)
{
  std::regex       reg("(.*layout\\s*\\(\\s*location\\s*=\\s*)(" + vertexStreamName + ")(\\s*\\).*)");
  for (std::string &line: lines)
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

std::vector<std::string>
loadExternalLinesRaw(const csResourceLocator &locator, iFile *file, std::set<csResourceLocator> &included)
{
  file->Seek(eSM_End, 0);
  long size = file->Tell();
  file->Seek(eSM_Set, 0);

  char *buffer = new char[size + 1];
  file->Read(sizeof(char), size, buffer);
  buffer[size] = '\0';
  std::string source(buffer);

  std::vector<std::string> lines = split(source);
  delete[] buffer;

  return replace_includes(&locator, lines, included);
}

std::vector<std::string>
loadExternalLinesSpc(const csResourceLocator &locator, iFile *file, std::set<csResourceLocator> &included)
{
  cryo::csCryoFile fFile;
  bool             res = fFile.Parse(file);
  if (!res)
  {
    return std::vector<std::string>();
  }

  csCryoFileElement *fragment = fFile.Root()->GetChild("fragment");
  if (!fragment || fragment->GetNumberOfAttributes() == 0)
  {
    return std::vector<std::string>();
  }

  std::string              source = fragment->GetAttribute(0)->GetValue();
  std::vector<std::string> lines  = split(source);
  return replace_includes(&locator, lines, included);

}


std::vector<std::string> loadExternalLines(const csResourceLocator &locator, std::set<csResourceLocator> &included)
{
  iFile *file = csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!file)
  {
    return std::vector<std::string>();
  }

  std::vector<std::string> lines;
  std::string              ext = locator.GetExtension();
  if (ext == "VERT"
      || ext == "EVAL"
      || ext == "CTRL"
      || ext == "GEOM"
      || ext == "FRAG"
      || ext == "COMP"
      || ext == "GLSL"
      )
  {
    lines = loadExternalLinesRaw(locator, file, included);
  }
  else if (ext == "CE")
  {
    lines = loadExternalLinesSpc(locator, file, included);
  }
  file->Close();
  file->Release();

  return lines;
}

std::vector<std::string>
replace_includes(const csResourceLocator *parent, std::vector<std::string> &lines, std::set<csResourceLocator> &included)
{
  std::vector<std::string> result;
  std::regex               reg("(#include\\s*\\<\\s*)([^\\>\\s]+)(\\s*\\>)");
  for (std::string         &line: lines)
  {
    std::smatch sm;
    if (std::regex_match(line, sm, reg))
    {

      std::string              part1       = sm[2];
      csResourceLocator        locator(parent, part1);
      std::vector<std::string> loadedLines = loadExternalLines(locator, included);
      included.insert(locator);
      for (std::string &l: loadedLines)
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

GL4Shader *LoadShader(const std::string &typeText,
                      const std::string &origSource,
                      const csResourceLocator *locator)
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


  std::vector<std::string>    lines = split(origSource);
  std::set<csResourceLocator> included;

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
  replace(lines, "eVS_BoneIndices", eVS_BoneIndices);
  replace(lines, "eVS_BoneWeights", eVS_BoneWeights);
  std::string source = merge(lines);

  //printf ("source: %s\n%s\n", locator->Encoded().c_str(), source.c_str());

  GL4Shader *shader = nullptr;
  try
  {
    shader = new GL4Shader(shaderType);
    shader->SetSource(source);
    shader->Compile();
    return shader;
  }
  catch (const GL4ShaderCompileException &sce)
  {
    printf("ShaderDeferred:\n");
    std::vector<std::string> lines = split(source);
    for (Size                i     = 0, in = lines.size(); i < in; ++i)
    {
      printf("(%04zu) %s\n", i + 1, lines[i].c_str());
    }
    printf("Unable to compile shader: %s\n%s\n", locator ? locator->GetLocator().c_str() : "unknown file", sce.what());
  }

  if (shader)
  {
    delete shader;
    shader = nullptr;
  }


  return nullptr;
}

GL4ShaderLoader::GL4ShaderLoader()
{
  CS_CLASS_GEN_CONSTR;
}


bool GL4ShaderLoader::CanLoad(const Class *cls, const csResourceLocator &locator) const
{
  std::string ext = locator.GetExtension();

  return cls->IsAssignableFrom<GL4Shader>() &&
         (ext == std::string("VERT")
          || ext == std::string("EVAL")
          || ext == std::string("CTRL")
          || ext == std::string("GEOM")
          || ext == std::string("FRAG")
          || ext == std::string("COMP")
         );
}


iObject *GL4ShaderLoader::Load(const Class *cls, const csResourceLocator &locator) const
{
  iFile *file = csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!file)
  {
    return nullptr;
  }

  file->Seek(eSM_End, 0);
  long size = file->Tell();
  file->Seek(eSM_Set, 0);

  char *buffer = new char[size + 1];
  file->Read(sizeof(char), size, buffer);
  buffer[size] = '\0';
  file->Close();
  file->Release();

  std::string source(buffer);
  std::string ext = locator.GetExtension();
  if (ext == std::string("VERT"))
  {
    ext = std::string("vertex");
  }
  else if (ext == std::string("EVAL"))
  {
    ext = std::string("tessEval");
  }
  else if (ext == std::string("CTRL"))
  {
    ext = std::string("tessControl");
  }
  else if (ext == std::string("GEOM"))
  {
    ext = std::string("geometry");
  }
  else if (ext == std::string("FRAG"))
  {
    ext = std::string("fragment");
  }
  else if (ext == std::string("COMP"))
  {
    ext = std::string("compute");
  }
  GL4Shader *shader = LoadShader(ext, source, &locator);
  delete[] buffer;
  buffer = nullptr;

  return shader;
}


}
