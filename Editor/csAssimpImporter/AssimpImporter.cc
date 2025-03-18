
#include <csAssimpImporter/AssimpImporter.hh>
#include <csAssimpImporter/AssimpMeshExporter.hh>
#include <csCryoFile/csCryoFile.hh>
#include <iostream>
#include <fstream>
#include <set>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <Windows.h>

using namespace cs::file;

namespace cs::imp
{


bool AssimpImporter::CanImport(const std::fs::path &path, const std::vector<std::string> &args) const
{
  if (!path.has_extension())
  {
    return false;
  }

  if (!HasOption(args, "--out-file"))
  {
    return false;
  }


  const std::filesystem::path &ext = path.extension();

  return ext == ".fbx";
}

bool AssimpImporter::Import(const std::fs::path &path, const std::vector<std::string> &args) const
{
  std::fs::path outFile = std::fs::path(GetOption(args, "--out-file"));
  if (outFile.empty())
  {
    std::cout << "[ERROR] No --out-file argument found." << std::endl;
    return false;
  }

  bool skeletons  = HasOption(args, "--skeletons");
  bool animations = HasOption(args, "--animations");
  bool materials  = HasOption(args, "--materials");
  bool meshes     = HasOption(args, "--meshes");
  bool mesh       = HasOption(args, "--mesh");
  bool entity     = HasOption(args, "--entity");

  std::cout << "Import: " << path << " -> " << outFile << std::endl;

  Assimp::Importer importer;
  const aiScene    *scene = importer.ReadFile(path.generic_string(),
                                              aiProcess_Triangulate
                                              | aiProcess_CalcTangentSpace
                                              | aiProcess_GenNormals
                                              | aiProcess_MakeLeftHanded
                                              | aiProcess_FlipWindingOrder
                                              | aiProcess_JoinIdenticalVertices
                                              | aiProcess_OptimizeMeshes
  );


  if (skeletons)
  {
    std::cout << "Generate skeletons: " << outFile.generic_string() << "_skeleton.skel" << std::endl;
  }

  if (animations)
  {
    std::cout << "Generate animations: " << outFile.generic_string() << "_animation.skelAnim" << std::endl;
  }


  if (materials)
  {
    GenerateMaterials(outFile, scene);
  }

  if (meshes)
  {
    GenerateMeshes(outFile, scene);

    if (entity)
    {
      GenerateMeshesEntity(outFile, scene);
    }
  }

  if (mesh)
  {
    GenerateMesh(outFile, scene);
    if (entity)
    {
      GenerateMeshEntity(outFile, scene);
    }
  }


  return scene != nullptr;
}


static std::string create_mesh_filename(const aiMesh *mesh)
{
  return mesh->mName.C_Str() + std::string("_") + std::to_string(mesh->mMaterialIndex);
}

static std::string extract_file_name(const std::string &str)
{
  size_t i = str.rfind("/");
  if (i == std::string::npos)
  {
    return "";
  }
  return str.substr(i + 1);
}


void AssimpImporter::GenerateMeshes(const std::fs::path &path, const aiScene *scene) const
{
  for (int i = 0; i < scene->mNumMeshes; ++i)
  {
    aiMesh *mesh = scene->mMeshes[i];

    std::string outputFileName = path.generic_string() + "_" + create_mesh_filename(mesh) + ".mesh";

    AssimpMeshExporter exporter(scene);
    exporter.combine(mesh);
    exporter.Export(outputFileName, extract_file_name(path.generic_string()));
  }
}


void AssimpImporter::GenerateMesh(const std::fs::path &path, const aiScene *scene) const
{
  std::string        outputFileName = path.generic_string() + ".mesh";
  AssimpMeshExporter exporter(scene);
  exporter.combine(scene->mRootNode);
  exporter.Export(outputFileName, extract_file_name(path.generic_string()));
}


void
export_aiNode(const std::fs::path &path, std::ofstream &out, std::string indent, aiNode *node, const aiScene *scene)
{
  int oldPrecision = out.precision();
  out.precision(2);
  out.setf(std::ios::fixed, std::ios::floatfield);

  auto &trans = node->mTransformation;
  out
      << indent << "entity name:\"" << node->mName.C_Str() << "\" {" << std::endl
      << indent << "  states {" << std::endl;

  for (unsigned i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh      *mesh       = scene->mMeshes[node->mMeshes[i]];
    std::string meshLocator = extract_file_name(path.generic_string()) + "_" + create_mesh_filename(mesh) + ".mesh";
    out
        << indent << "    state cls:\"cs::csStaticMeshState\" {" << std::endl
        << indent << "      transform {" << std::endl
        << indent << "        matrix4 " << trans.a1 << " " << trans.b1 << " " << trans.c1 << "  " << trans.d1
        << std::endl
        << indent << "                " << trans.a2 << " " << trans.b2 << " " << trans.c2 << "  " << trans.d2
        << std::endl
        << indent << "                " << trans.a3 << " " << trans.b3 << " " << trans.c3 << "  " << trans.d3
        << std::endl
        << indent << "                " << trans.a4 << " " << trans.b4 << " " << trans.c4 << "  " << trans.d4 << ", "
        << std::endl
        << indent << "      }," << std::endl

        << indent << "      mesh locator:\"" << meshLocator << "\", " << std::endl
        << indent << "    }," << std::endl;
  }

  out << indent << "  }," << std::endl;


  if (node->mNumChildren)
  {
    out << indent << "  children {" << std::endl;
    for (unsigned i = 0; i < node->mNumChildren; i++)
    {
      auto child = node->mChildren[i];
      export_aiNode(path, out, indent + "    ", child, scene);
    }
    out << indent << "  }," << std::endl;
  }

  out << indent << "}," << std::endl;

  out.precision(oldPrecision);
}


void AssimpImporter::GenerateMeshesEntity(const std::fs::path &path, const aiScene *scene) const
{
  std::string   outputFileName = path.generic_string() + "_compound.entity";
  std::fs::path outFile(outputFileName);
  std::ofstream out;
  out.open(outputFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

  export_aiNode(path, out, "", scene->mRootNode, scene);


  out.close();
};


void AssimpImporter::GenerateMeshEntity(const std::fs::path &path, const aiScene *scene) const
{
  std::string entityFileName = path.generic_string() + ".entity";
  std::string meshLocator    = extract_file_name(path.generic_string()) + ".mesh";


  csCryoFile        file;
  csCryoFileElement *entityElement = file.Root()->AddChild("entity");

  csCryoFileElement *transformElement = entityElement->AddChild("transform");
  csCryoFileElement *matrixElement    = transformElement->AddChild("matrix4");
  matrixElement->AddAttribute("1");
  matrixElement->AddAttribute("0");
  matrixElement->AddAttribute("0");
  matrixElement->AddAttribute("0");

  matrixElement->AddAttribute("0");
  matrixElement->AddAttribute("1");
  matrixElement->AddAttribute("0");
  matrixElement->AddAttribute("0");

  matrixElement->AddAttribute("0");
  matrixElement->AddAttribute("0");
  matrixElement->AddAttribute("1");
  matrixElement->AddAttribute("0");

  matrixElement->AddAttribute("0");
  matrixElement->AddAttribute("0");
  matrixElement->AddAttribute("0");
  matrixElement->AddAttribute("1");


  csCryoFileElement *statesElement = entityElement->AddChild("states");
  csCryoFileElement *stateElement  = statesElement->AddChild("state");
  stateElement->AddStringAttribute("cls", "cs::csStaticMeshState");

  csCryoFileElement *meshElement = stateElement->AddChild("mesh");
  meshElement->AddStringAttribute("locator", meshLocator);


  std::ofstream out;
  out.open(entityFileName, std::ios::out | std::ios::binary | std::ios::trunc);
  file.Write(out, true, 2);
  out.close();
};

void AssimpImporter::GenerateMaterials(const std::fs::path &path, const aiScene *scene) const
{
  for (size_t i = 0; i < scene->mNumMaterials; i++)
  {
    aiMaterial *material = scene->mMaterials[i];

    aiString name = material->GetName();

    std::string materialFileName = path.generic_string() + "_" + name.C_Str() + ".matinstance";

    std::ofstream out;
    out.open(materialFileName, std::ios::out | std::ios::trunc);

    aiColor4D color4;

    out << "materialinstance {" << std::endl
        << "  material \"/materials/Default.mat\"," << std::endl
        << "  attributes {" << std::endl;

    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color4) == aiReturn_SUCCESS)
    {
      out << "    attribute Color4 \"Color\" "
          << color4.r << " " << color4.g << " " << color4.b << " " << color4.a << "," << std::endl;
    }

    out << "  }," << std::endl
        << "}," << std::endl;

    out.close();

  }
}


void AssimpImporter::PrintUsage() const
{
  std::cout << "Assimp importer [options]: *.fbx *.obj" << std::endl;
  std::cout << "   --out-file     prefix of the generated files. " << std::endl;
  std::cout << "   --skeletons    export skeletons" << std::endl;
  std::cout << "   --animations   export animations" << std::endl;
  std::cout << "   --materials    export materials. Must be assigned in editor" << std::endl;
  std::cout << "   --meshes       export each mesh" << std::endl;
  std::cout << "   --mesh         export one single mesh" << std::endl;
  std::cout << "   --entity       export entities" << std::endl;
}


}
