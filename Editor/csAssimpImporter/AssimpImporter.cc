
#include <csAssimpImporter/AssimpImporter.hh>
#include <iostream>
#include <fstream>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


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
    GenerateRenderMesh(path, scene->mMeshes[i], scene);
  }
}

void AssimpImporter::GenerateRenderMesh(const std::fs::path &path, const aiMesh *mesh, const aiScene *scene) const
{
  std::string outputFileName = path.generic_string() + "_" + mesh->mName.C_Str() + ".rmesh";
  std::fs::path outFile (outputFileName);
  std::ofstream out;
  out.open(outputFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

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


  uint32_t magic = 0x12341234;
  uint32_t version = 1;
  out.write(reinterpret_cast<char*>(&magic), sizeof(uint32_t));
  out.write(reinterpret_cast<char*>(&version), sizeof(uint32_t));



  VDataType type = VERTEX;
  out.write(reinterpret_cast<char*>(&type), sizeof(uint8_t));

  if (mesh->mNormals)
  {
    type = NORMAL;
    out.write(reinterpret_cast<char*>(&type), sizeof(uint8_t));
  }
  if (mesh->mTangents)
  {
    type = TANGENT;
    out.write(reinterpret_cast<char*>(&type), sizeof(uint8_t));
  }
  if (mesh->mColors[0])
  {
    type = COLOR0;
    out.write(reinterpret_cast<char*>(&type), sizeof(uint8_t));
  }
  if (mesh->mColors[1])
  {
    type = COLOR1;
    out.write(reinterpret_cast<char*>(&type), sizeof(uint8_t));
  }

  if (mesh->mTextureCoords[0] && mesh->mNumUVComponents[0] == 2)
  {
    switch (mesh->mNumUVComponents[0])
    {
      case 1:
        type = TEX_COORD0_1;
        out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
        break;
      case 2:
        type = TEX_COORD0_2;
        out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
        break;
      case 3:
        type = TEX_COORD0_2;
        out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
        break;
    }
  }
  if (mesh->mTextureCoords[1] && mesh->mNumUVComponents[1] == 2)
  {
    switch (mesh->mNumUVComponents[1])
    {
      case 1:
        type = TEX_COORD1_1;
        out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
        break;
      case 2:
        type = TEX_COORD1_2;
        out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
        break;
      case 3:
        type = TEX_COORD1_2;
        out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
        break;
    }
  }
  if (mesh->mTextureCoords[2] && mesh->mNumUVComponents[2] == 2)
  {
    switch (mesh->mNumUVComponents[2])
    {
      case 1:
        type = TEX_COORD2_1;
        out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
        break;
      case 2:
        type = TEX_COORD2_2;
        out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
        break;
      case 3:
        type = TEX_COORD2_2;
        out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
        break;
    }
  }
  type = END;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));


  
  uint32_t numVertices = mesh->mNumVertices;
  out.write(reinterpret_cast<char *>(&numVertices), sizeof(uint32_t));
  
  for (int i = 0; i < mesh->mNumVertices; ++i)
  {
    aiVector3D &pos = mesh->mVertices[i];
    out.write (reinterpret_cast<char*>(&pos), sizeof(aiVector3D));
    

    if (mesh->mNormals)
    {
      aiVector3D &norm = mesh->mNormals[i];
      out.write (reinterpret_cast<char*>(&norm), sizeof(aiVector3D));
    }

    if (mesh->mTangents)
    {
      aiVector3D &tan = mesh->mTangents[i];
      out.write (reinterpret_cast<char*>(&tan), sizeof(aiVector3D));
    }
    for (size_t j=0; j<2; j++)
    {
      if (mesh->mColors[j])
      {
        aiColor4D &color = mesh->mColors[j][i];
        out.write (reinterpret_cast<char*>(&color), sizeof(aiColor4D));
      }
    }

    for (size_t j = 0; j < 3; j++)
    {

      switch (mesh->mNumUVComponents[j])
      {
        case 1:
          out.write (reinterpret_cast<char*>(&mesh->mTextureCoords[j][i]), sizeof(float)*1);
          break;
        case 2:
          out.write (reinterpret_cast<char*>(&mesh->mTextureCoords[j][i]), sizeof(float)*2);
          break;
        case 3:
          out.write (reinterpret_cast<char*>(&mesh->mTextureCoords[j][i]), sizeof(float)*3);
          break;
        default:
          break;
      }
    }
  }


  uint32_t numFaces = mesh->mNumFaces;
  out.write(reinterpret_cast<char*>(&numFaces), sizeof(uint32_t));
  for (int i = 0; i < mesh->mNumFaces; ++i)
  {
    const aiFace &face = mesh->mFaces[i];
    if (numVertices >= 65336)
    {
      uint32_t f0 = face.mIndices[0];
      uint32_t f1 = face.mIndices[1];
      uint32_t f2 = face.mIndices[2];
      out.write(reinterpret_cast<char *>(&f0), sizeof(uint32_t));
      out.write(reinterpret_cast<char *>(&f1), sizeof(uint32_t));
      out.write(reinterpret_cast<char *>(&f2), sizeof(uint32_t));
    }
    else
    {
      uint16_t f0 = face.mIndices[0];
      uint16_t f1 = face.mIndices[1];
      uint16_t f2 = face.mIndices[2];
      out.write(reinterpret_cast<char *>(&f0), sizeof(uint16_t));
      out.write(reinterpret_cast<char *>(&f1), sizeof(uint16_t));
      out.write(reinterpret_cast<char *>(&f2), sizeof(uint16_t));
    }

  }


  out.close();
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
