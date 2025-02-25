
#include <csAssimpImporter/AssimpImporter.hh>
#include <iostream>
#include <fstream>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


namespace cs::imp
{

void write_mesh(std::ofstream &out, const aiMesh *mesh);


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

  bool noSkeletons  = HasOption(args, "--no-skeletons");
  bool noAnimations = HasOption(args, "--no-animations");
  bool noMaterials  = HasOption(args, "--no-materials");
  bool noMeshes     = HasOption(args, "--no-meshes");
  bool noSingleMesh = HasOption(args, "--no-single-mesh");
  bool noEntity     = HasOption(args, "--no-entity");

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


  if (!noSkeletons)
  {
    std::cout << "Generate skeletons: " << outFile.generic_string() << "_skeleton.skel" << std::endl;
  }

  if (!noAnimations)
  {
    std::cout << "Generate animations: " << outFile.generic_string() << "_animation.skelAnim" << std::endl;
  }


  if (!noMaterials)
  {
    std::cout << "Generate materials: " << outFile.generic_string() << "_skeleton.materialInstance" << std::endl;
  }

  if (!noMeshes)
  {
    GenerateRenderMeshes(outFile, scene);
  }

  if (!noSingleMesh)
  {
    std::cout << "Generate single mesh: " << outFile.generic_string() << "_singleMesh.mesh" << std::endl;
  }


  if (!noEntity)
  {
    std::cout << "Generate entity: " << outFile.generic_string() << "_entity.entity" << std::endl;
  }


  return scene != nullptr;
}


void AssimpImporter::GenerateRenderMeshes(const std::fs::path &path, const aiScene *scene) const
{
  for (int i = 0; i < scene->mNumMeshes; ++i)
  {
    GenerateMesh(path, scene->mMeshes[i], scene);
    GenerateRenderMesh(path, scene->mMeshes[i], scene);
  }
}

void write_string(std::ofstream& out, const std::string& string)
{
  uint32_t length = string.length();
  out.write(reinterpret_cast<const char*>(&length), sizeof(uint32_t));
  out.write(string.c_str(), sizeof(char) * length);
}

void AssimpImporter::GenerateMesh(const std::fs::path& path, const aiMesh* mesh, const aiScene* scene) const
{
  std::string   outputFileName = path.generic_string() + "_" + mesh->mName.C_Str() + ".mesh";
  std::fs::path outFile(outputFileName);
  std::ofstream out;
  out.open(outputFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);


  uint32_t magic = 0x12341234;
  uint32_t version = 1;
  out.write(reinterpret_cast<char*>(&magic), sizeof(uint32_t));
  out.write(reinterpret_cast<char*>(&version), sizeof(uint32_t));


  uint32_t numMaterialSlots = 1;
  out.write(reinterpret_cast<char*>(&numMaterialSlots), sizeof(uint32_t));
  write_string(out, "Default");
  write_string(out, "/materials/Default.mat");

  uint32_t numSubMeshes = 1;
  out.write(reinterpret_cast<char*>(&numSubMeshes), sizeof(uint32_t));
  uint32_t materialSlot = 0;
  out.write(reinterpret_cast<char*>(&materialSlot), sizeof(uint32_t));
  write_mesh(out, mesh);
  out.close();
}


void AssimpImporter::GenerateRenderMesh(const std::fs::path &path, const aiMesh *mesh, const aiScene *scene) const
{
  std::string   outputFileName = path.generic_string() + "_" + mesh->mName.C_Str() + ".rmesh";
  std::fs::path outFile(outputFileName);
  std::ofstream out;
  out.open(outputFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);


  uint32_t magic = 0x12341234;
  uint32_t version = 1;
  uint32_t numVertices = mesh->mNumVertices;

  out.write(reinterpret_cast<char*>(&magic), sizeof(uint32_t));
  out.write(reinterpret_cast<char*>(&version), sizeof(uint32_t));

  write_mesh(out, mesh);
  out.close();
}

enum VDataType
{
  END,
  VERTEX,
  NORMAL,
  TANGENT,
  COLOR0,
  COLOR1,
  TEX_COORD0_1,
  TEX_COORD0_2,
  TEX_COORD0_3,
  TEX_COORD1_1,
  TEX_COORD1_2,
  TEX_COORD1_3,
  TEX_COORD2_1,
  TEX_COORD2_2,
  TEX_COORD2_3
};

enum PrimitiveType
{
  POINTS,
  LINES,
  TRIANGLES,
};

enum IndexType
{
  UINT16,
  UINT32
};

void write_values_v1(std::ofstream &out, VDataType dataType, size_t numValues, aiVector3D *vertices)
{
  if (!vertices)
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = numValues * sizeof(float) * 1;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));
  for (size_t i = 0; i < numValues; i++)
  {
    aiVector3D &v = vertices[i];
    out.write(reinterpret_cast<char *>(&v), sizeof(float) * 1);
  }
}

void write_values_v2(std::ofstream &out, VDataType dataType, size_t numValues, aiVector3D *vertices)
{
  if (!vertices)
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = numValues * sizeof(float) * 2;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));
  for (size_t i = 0; i < numValues; i++)
  {
    aiVector3D &v = vertices[i];
    out.write(reinterpret_cast<char *>(&v), sizeof(float) * 2);
  }
}


void write_values_v3(std::ofstream &out, VDataType dataType, size_t numValues, aiVector3D *vertices)
{
  if (!vertices)
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = numValues * sizeof(float) * 3;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));
  for (size_t i = 0; i < numValues; i++)
  {
    aiVector3D &v = vertices[i];
    out.write(reinterpret_cast<char *>(&v), sizeof(float) * 3);
  }
}


void write_values_c4(std::ofstream &out, VDataType dataType, size_t numValues, aiColor4D *colors)
{
  if (!colors)
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = numValues * sizeof(float) * 4;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));
  for (size_t i = 0; i < numValues; i++)
  {
    aiColor4D &v = colors[i];
    out.write(reinterpret_cast<char *>(&v), sizeof(float) * 4);
  }
}


void write_mesh(std::ofstream &out, const aiMesh *mesh)
{
  uint32_t numVertices = mesh->mNumVertices;
  out.write(reinterpret_cast<char *>(&numVertices), sizeof(uint32_t));

  write_values_v3(out, VERTEX, mesh->mNumVertices, mesh->mVertices);
  write_values_v3(out, NORMAL, mesh->mNumVertices, mesh->mNormals);
  write_values_v3(out, TANGENT, mesh->mNumVertices, mesh->mTangents);
  write_values_c4(out, COLOR0, mesh->mNumVertices, mesh->mColors[0]);
  write_values_c4(out, COLOR1, mesh->mNumVertices, mesh->mColors[1]);

  write_values_v3(out, NORMAL, mesh->mNumVertices, mesh->mNormals);
  write_values_v3(out, NORMAL, mesh->mNumVertices, mesh->mNormals);
  write_values_v3(out, NORMAL, mesh->mNumVertices, mesh->mNormals);

  switch (mesh->mNumUVComponents[0])
  {
    case 1:
      write_values_v1(out, TEX_COORD0_1, mesh->mNumVertices, mesh->mTextureCoords[0]);
      break;
    case 2:
      write_values_v2(out, TEX_COORD0_2, mesh->mNumVertices, mesh->mTextureCoords[0]);
      break;
    case 3:
      write_values_v3(out, TEX_COORD0_3, mesh->mNumVertices, mesh->mTextureCoords[0]);
      break;
    default:
      break;
  }
  switch (mesh->mNumUVComponents[1])
  {
    case 1:
      write_values_v1(out, TEX_COORD1_1, mesh->mNumVertices, mesh->mTextureCoords[1]);
      break;
    case 2:
      write_values_v2(out, TEX_COORD1_2, mesh->mNumVertices, mesh->mTextureCoords[1]);
      break;
    case 3:
      write_values_v3(out, TEX_COORD1_3, mesh->mNumVertices, mesh->mTextureCoords[1]);
      break;
    default:
      break;
  }
  switch (mesh->mNumUVComponents[2])
  {
    case 1:
      write_values_v1(out, TEX_COORD2_1, mesh->mNumVertices, mesh->mTextureCoords[2]);
      break;
    case 2:
      write_values_v2(out, TEX_COORD2_2, mesh->mNumVertices, mesh->mTextureCoords[2]);
      break;
    case 3:
      write_values_v3(out, TEX_COORD2_3, mesh->mNumVertices, mesh->mTextureCoords[2]);
      break;
    default:
      break;
  }
  uint8_t  type = END;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));




  uint8_t primType = TRIANGLES;
  out.write(reinterpret_cast<char *>(&primType), sizeof(uint8_t));

  uint32_t numIndex = mesh->mNumFaces * 3;
  out.write(reinterpret_cast<char *>(&numIndex), sizeof(uint32_t));


  if (numVertices >= 65336)
  {
    uint8_t indexType = UINT32;
    out.write(reinterpret_cast<char *>(&indexType), sizeof(uint8_t));
    uint32_t indexSize = mesh->mNumFaces * 3 * sizeof(uint32_t);
    out.write(reinterpret_cast<char *>(&indexSize), sizeof(uint32_t));
    for (int i = 0; i < mesh->mNumFaces; ++i)
    {
      const aiFace &face = mesh->mFaces[i];
      uint32_t     f0    = face.mIndices[0];
      uint32_t     f1    = face.mIndices[1];
      uint32_t     f2    = face.mIndices[2];
      out.write(reinterpret_cast<char *>(&f0), sizeof(uint32_t));
      out.write(reinterpret_cast<char *>(&f1), sizeof(uint32_t));
      out.write(reinterpret_cast<char *>(&f2), sizeof(uint32_t));
    }
  }
  else
  {
    uint8_t indexType = UINT16;
    out.write(reinterpret_cast<char *>(&indexType), sizeof(uint8_t));
    uint32_t indexSize = mesh->mNumFaces * 3 * sizeof(uint16_t);
    out.write(reinterpret_cast<char *>(&indexSize), sizeof(uint32_t));
    for (int i = 0; i < mesh->mNumFaces; ++i)
    {
      const aiFace &face = mesh->mFaces[i];
      uint16_t f0 = face.mIndices[0];
      uint16_t f1 = face.mIndices[1];
      uint16_t f2 = face.mIndices[2];
      out.write(reinterpret_cast<char *>(&f0), sizeof(uint16_t));
      out.write(reinterpret_cast<char *>(&f1), sizeof(uint16_t));
      out.write(reinterpret_cast<char *>(&f2), sizeof(uint16_t));
    }
  }

}

void AssimpImporter::PrintUsage() const
{
  std::cout << "Assimp importer [options]: *.fbx *.obj" << std::endl;
  std::cout << "   --out-file        prefix of the generated files. " << std::endl;
  std::cout << "   --no-skeletons     skip export of skeletons" << std::endl;
  std::cout << "   --no-animations   skip export of animations" << std::endl;
  std::cout << "   --no-materials    skip export of materials. Must be assigned in editor" << std::endl;
  std::cout << "   --no-meshes       skip export of each mesh" << std::endl;
  std::cout << "   --no-single-mesh  skip export of one single mesh" << std::endl;
  std::cout << "   --no-entity       skip export of entities" << std::endl;
}


}
