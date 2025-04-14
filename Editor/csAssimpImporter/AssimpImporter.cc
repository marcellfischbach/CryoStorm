
#include <csAssimpImporter/AssimpImporter.hh>
#include <csAssimpImporter/AssimpMeshExporter.hh>
#include <csAssimpImporter/AssimpSkeletonAnimationExporter.hh>
#include <csAssimpImporter/AssimpSkeletonExporter.hh>
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
    printf ("file name has no extension\n");
    return false;
  }

  const std::filesystem::path &ext = path.extension();

  bool extMatching = ext == ".fbx" || ext == ".glb";

  if (!extMatching)
  {
    printf ("Extension is not matching\n");
  }

  return extMatching;
}

std::fs::path extract_default_out_file(const std::fs::path &importFilename)
{
  const std::string &ext     = importFilename.extension().generic_string();
  std::string       filename = importFilename.generic_string();
  filename = filename.substr(0, filename.length() - ext.length());

  return std::fs::path(filename);
}


bool AssimpImporter::Import(const std::fs::path &path, const std::vector<std::string> &args) const
{
  std::fs::path outFile = std::fs::path(GetOption(args, "--out-file"));
  if (outFile.empty())
  {
    outFile = extract_default_out_file(path);
    if (outFile.empty())
    {
      std::cout << "[ERROR] No --out-file argument found." << std::endl;
      return false;

    }
  }

  bool skeleton   = HasOption(args, "--skeleton");
  bool animations = HasOption(args, "--animations");
  bool materials  = HasOption(args, "--materials");
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

  if (!scene)
  {
    std::cerr << "File could not be imported. Error" << std::endl;
    return false;
  }


  if (skeleton)
  {
    GenerateSkeleton(outFile, scene);
  }

  if (animations)
  {
    GenerateAnimations(outFile, scene);
  }


  if (materials)
  {
    GenerateMaterials(outFile, scene);
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
    return str;
  }
  return str.substr(i + 1);
}

void AssimpImporter::GenerateMesh(const std::fs::path &path, const aiScene *scene) const
{
  std::string        outputFileName = path.generic_string() + ".mesh";
  AssimpMeshExporter exporter(scene);
  exporter.combine(scene->mRootNode);
  exporter.Export(outputFileName, extract_file_name(path.generic_string()));
}




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
  AssimpSkeletonExporter skeletonExporter(scene);
  skeletonExporter.ScanBones();
  std::string baseMaterial = skeletonExporter.HasBones()
      ? "/materials/Default-Skinned.mat"
      : "/materials/Default.mat";

  for (size_t i = 0; i < scene->mNumMaterials; i++)
  {
    aiMaterial *material = scene->mMaterials[i];

    aiString name = material->GetName();

    std::string materialFileName = path.generic_string() + "_" + name.C_Str() + ".matinstance";

    std::ofstream out;
    out.open(materialFileName, std::ios::out | std::ios::trunc);

    aiColor4D color4;

    out << "materialinstance {" << std::endl
        << "  material \"" << baseMaterial << "\"," << std::endl
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

void AssimpImporter::GenerateSkeleton(const std::fs::path &path, const aiScene *scene) const
{
  AssimpSkeletonExporter exp(scene);
  exp.ScanBones();
  if (!exp.HasBones())
  {
    std::cerr << "File does not contain any bone data." << std::endl;
    return;
  }

  std::string skeletonFileName = path.generic_string() + ".skeleton";

  exp.Export(skeletonFileName);

}


// Armature|MyAnimation01
std::string sanitize_filename(const std::string &filename)
{
  // Define illegal characters for filenames
  std::set<char> illegalChars = {'/', '\\', ':', '*', '?', '"', '<', '>', '|'};
  std::string    sanitized    = filename;

  // Replace illegal characters with '_'
  for (char &c: sanitized)
  {
    if (illegalChars.find(c) != illegalChars.end())
    {
      c = '_';
    }
  }
  return sanitized;
}


void AssimpImporter::GenerateAnimations(const std::fs::path &path, const aiScene *scene) const
{
  AssimpSkeletonExporter skelExp(scene);
  skelExp.ScanBones();

  if (!skelExp.HasBones())
  {
    std::cerr << "Cannot export animations because the file does not contain any bone data." << std::endl;
    return;
  }

  for (int i = 0; i < scene->mNumAnimations; ++i)
  {
    aiAnimation *animation    = scene->mAnimations[i];
    std::string animationName = sanitize_filename(animation->mName.C_Str());

    std::string skeletonFileName = path.generic_string() + "_" + animationName + ".skeleton_animation";

    AssimpSkeletonAnimationExporter animExp(scene, animation, &skelExp);
    animExp.Export(skeletonFileName);
  }
}

void AssimpImporter::PrintUsage() const
{
  std::cout << "Assimp importer [options]: *.fbx *.obj" << std::endl;
  std::cout << "   --out-file     prefix of the generated files. " << std::endl;
  std::cout << "   --skeleton     export skeleton" << std::endl;
  std::cout << "   --animations   export animations" << std::endl;
  std::cout << "   --materials    export materials. Must be assigned in editor" << std::endl;
  std::cout << "   --meshes       export each mesh" << std::endl;
  std::cout << "   --mesh         export one single mesh" << std::endl;
  std::cout << "   --entity       export entities" << std::endl;
}


}
