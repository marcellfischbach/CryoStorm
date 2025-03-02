
#include <csAssimpImporter/AssimpImporter.hh>
#include <csCryoFile/csCryoFile.hh>
#include <iostream>
#include <fstream>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

using namespace cs::file;

namespace cs::imp
{




bool AssimpImporter::CanImport(const std::fs::path& path, const std::vector<std::string>& args) const
{
  if (!path.has_extension())
  {
    return false;
  }

  if (!HasOption(args, "--out-file"))
  {
    return false;
  }


  const std::filesystem::path& ext = path.extension();

  return ext == ".fbx";
}

bool AssimpImporter::Import(const std::fs::path& path, const std::vector<std::string>& args) const
{
  std::fs::path outFile = std::fs::path(GetOption(args, "--out-file"));
  if (outFile.empty())
  {
    std::cout << "[ERROR] No --out-file argument found." << std::endl;
    return false;
  }

  bool skeletons = HasOption(args, "--skeletons");
  bool animations = HasOption(args, "--animations");
  bool materials = HasOption(args, "--materials");
  bool meshes = HasOption(args, "--meshes");
  bool singleMesh = HasOption(args, "--single-mesh");
  bool entity = HasOption(args, "--entity");

  std::cout << "Import: " << path << " -> " << outFile << std::endl;

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path.generic_string(),
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
    std::cout << "Generate materials: " << outFile.generic_string() << "_skeleton.materialInstance" << std::endl;
  }

  if (meshes)
  {
    GenerateRenderMeshes(outFile, scene);

    if (entity)
    {
      GeneratesMeshesEntity(outFile, scene);
    }
  }

  if (singleMesh)
  {
    std::cout << "Generate single mesh: " << outFile.generic_string() << "_singleMesh.mesh" << std::endl;
  }


  if (entity)
  {
    std::cout << "Generate entity: " << outFile.generic_string() << "_entity.entity" << std::endl;
  }


  return scene != nullptr;
}


void AssimpImporter::GenerateRenderMeshes(const std::fs::path& path, const aiScene* scene) const
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


static std::string create_mesh_filename(const aiMesh* mesh)
{
  return mesh->mName.C_Str() + std::string("_") + std::to_string(mesh->mMaterialIndex);
}

static std::string extract_file_name(const std::string& str)
{
  size_t i = str.rfind("/");
  if (i == std::string::npos)
  {
    return "";
  }
  return str.substr(i + 1);
}

void AssimpImporter::GenerateMesh(const std::fs::path& path, const aiMesh* mesh, const aiScene* scene) const
{
  std::string outputFileName = path.generic_string() + "_" + create_mesh_filename(mesh) + ".mesh";
  std::string renderMeshName = extract_file_name(path.generic_string()) + "_" + create_mesh_filename(mesh) + ".rmesh";
  std::fs::path outFile(outputFileName);
  std::ofstream out;
  out.open(outputFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

  /*
  out
    << "mesh {" << std::endl
    << "  materialSlots {" << std::endl
    << "    materialSlot name:\"Default\" locator:\"/materials/Default.mat\", " << std::endl
    << "  }," << std::endl
    << "  subMeshes {" << std::endl
    << "    subMesh slot: 0 locator: \"" << renderMeshName << "\"," << std::endl
    << "  }," << std::endl
    << "}" << std::endl;
  */

  csCryoFile file;
  auto elemMesh = new csCryoFileElement("mesh", file.Root());
  auto elemMaterialSlots = new csCryoFileElement("materialSlots", elemMesh);
  auto elemMaterialSlot0 = new csCryoFileElement("materialSlot", elemMaterialSlots);
  auto elemSubMeshes = new csCryoFileElement("meshes", elemMesh);
  auto elemSubMesh0 = new csCryoFileElement("mesh", elemSubMeshes);

  elemMaterialSlot0->AddStringAttribute("name", "Default");
  elemMaterialSlot0->AddStringAttribute("locator", "/materials/Default.mat");

  elemSubMesh0->AddAttribute("slot", "0");
  elemSubMesh0->AddStringAttribute("dataIdx", create_mesh_filename(mesh));


  std::ostringstream ostream;
  WriteMesh(ostream, mesh);
  std::string dataStream = ostream.str();

  file.AddData(create_mesh_filename(mesh), dataStream.size(), reinterpret_cast<uint8_t*>(dataStream.data()));




  file.Write(out, true, 2);

  out.close();

}


void AssimpImporter::GenerateRenderMesh(const std::fs::path& path, const aiMesh* mesh, const aiScene* scene) const
{
  std::string   outputFileName = path.generic_string() + "_" + create_mesh_filename(mesh) + ".rmesh";
  std::fs::path outFile(outputFileName);

  std::cout << "Generate Render Mesh: " << outputFileName << " numFaces : " << mesh->mNumFaces << std::endl;
  std::ofstream out;
  out.open(outputFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);


  uint32_t magic = 0x12341234;
  uint32_t version = 1;
  uint32_t numVertices = mesh->mNumVertices;

  out.write(reinterpret_cast<char*>(&magic), sizeof(uint32_t));
  out.write(reinterpret_cast<char*>(&version), sizeof(uint32_t));

  WriteMesh(out, mesh);
  out.close();
}


void export_aiNode(const std::fs::path& path, std::ofstream& out, std::string indent, aiNode* node, const aiScene* scene)
{
  int oldPrecision = out.precision();
  out.precision(2);
  out.setf(std::ios::fixed, std::ios::floatfield);

  auto& trans = node->mTransformation;
  out
    << indent << "entity name:\"" << node->mName.C_Str() << "\" {" << std::endl
    << indent << "  transform {" << std::endl
    << indent << "    matrix4 " << trans.a1 << " " << trans.b1 << " " << trans.c1 << "  " << trans.d1 << std::endl
    << indent << "            " << trans.a2 << " " << trans.b2 << " " << trans.c2 << "  " << trans.d2 << std::endl
    << indent << "            " << trans.a3 << " " << trans.b3 << " " << trans.c3 << "  " << trans.d3 << std::endl
    << indent << "            " << trans.a4 << " " << trans.b4 << " " << trans.c4 << "  " << trans.d4 << ", " << std::endl
    << indent << "  }," << std::endl;

  for (unsigned i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    std::string meshLocator = extract_file_name(path.generic_string()) + "_" + create_mesh_filename(mesh) + ".mesh";
    out
      << indent << "  state cls:\"cs::csStaticMeshState\" {" << std::endl
      << indent << "    mesh locator:\"" << meshLocator << "\", " << std::endl
      << indent << "  }," << std::endl;
  }


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


void AssimpImporter::GeneratesMeshesEntity(const std::fs::path& path, const aiScene* scene) const
{
  std::string outputFileName = path.generic_string() + "_compound.entity";
  std::fs::path outFile(outputFileName);
  std::ofstream out;
  out.open(outputFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

  export_aiNode(path, out, "", scene->mRootNode, scene);


  out.close();
};

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

void write_values_v1(std::ostream& out, VDataType dataType, size_t numValues, aiVector3D* vertices)
{
  if (!vertices)
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = numValues * sizeof(float) * 1;
  out.write(reinterpret_cast<char*>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char*>(&size), sizeof(uint32_t));
  for (size_t i = 0; i < numValues; i++)
  {
    aiVector3D& v = vertices[i];
    out.write(reinterpret_cast<char*>(&v), sizeof(float) * 1);
  }
}

void write_values_v2(std::ostream& out, VDataType dataType, size_t numValues, aiVector3D* vertices)
{
  if (!vertices)
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = numValues * sizeof(float) * 2;
  out.write(reinterpret_cast<char*>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char*>(&size), sizeof(uint32_t));
  for (size_t i = 0; i < numValues; i++)
  {
    aiVector3D& v = vertices[i];
    out.write(reinterpret_cast<char*>(&v), sizeof(float) * 2);
  }
}


void write_values_v3(std::ostream& out, VDataType dataType, size_t numValues, aiVector3D* vertices)
{
  if (!vertices)
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = numValues * sizeof(float) * 3;
  out.write(reinterpret_cast<char*>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char*>(&size), sizeof(uint32_t));
  for (size_t i = 0; i < numValues; i++)
  {
    aiVector3D& v = vertices[i];
    out.write(reinterpret_cast<char*>(&v), sizeof(float) * 3);
  }
}


void write_values_c4(std::ostream& out, VDataType dataType, size_t numValues, aiColor4D* colors)
{
  if (!colors)
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = numValues * sizeof(float) * 4;
  out.write(reinterpret_cast<char*>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char*>(&size), sizeof(uint32_t));
  for (size_t i = 0; i < numValues; i++)
  {
    aiColor4D& v = colors[i];
    out.write(reinterpret_cast<char*>(&v), sizeof(float) * 4);
  }
}


void AssimpImporter::WriteMesh(std::ostream& out, const aiMesh* mesh) const
{
  uint32_t version = 0x01;
  out.write(reinterpret_cast<char*>(&version), sizeof(uint32_t));


  uint32_t numVertices = mesh->mNumVertices;
  out.write(reinterpret_cast<char*>(&numVertices), sizeof(uint32_t));

  write_values_v3(out, VERTEX, mesh->mNumVertices, mesh->mVertices);
  write_values_v3(out, NORMAL, mesh->mNumVertices, mesh->mNormals);
  write_values_v3(out, TANGENT, mesh->mNumVertices, mesh->mTangents);
  write_values_c4(out, COLOR0, mesh->mNumVertices, mesh->mColors[0]);
  write_values_c4(out, COLOR1, mesh->mNumVertices, mesh->mColors[1]);

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
  out.write(reinterpret_cast<char*>(&type), sizeof(uint8_t));




  uint8_t primType = TRIANGLES;
  out.write(reinterpret_cast<char*>(&primType), sizeof(uint8_t));

  uint32_t numIndex = mesh->mNumFaces * 3;
  out.write(reinterpret_cast<char*>(&numIndex), sizeof(uint32_t));


  if (numVertices >= 65336)
  {
    uint8_t indexType = UINT32;
    out.write(reinterpret_cast<char*>(&indexType), sizeof(uint8_t));
    uint32_t indexSize = mesh->mNumFaces * 3 * sizeof(uint32_t);
    out.write(reinterpret_cast<char*>(&indexSize), sizeof(uint32_t));
    for (int i = 0; i < mesh->mNumFaces; ++i)
    {
      const aiFace& face = mesh->mFaces[i];
      uint32_t     f0 = face.mIndices[0];
      uint32_t     f1 = face.mIndices[1];
      uint32_t     f2 = face.mIndices[2];
      out.write(reinterpret_cast<char*>(&f0), sizeof(uint32_t));
      out.write(reinterpret_cast<char*>(&f1), sizeof(uint32_t));
      out.write(reinterpret_cast<char*>(&f2), sizeof(uint32_t));
    }
  }
  else
  {
    uint8_t indexType = UINT16;
    out.write(reinterpret_cast<char*>(&indexType), sizeof(uint8_t));
    uint32_t indexSize = mesh->mNumFaces * 3 * sizeof(uint16_t);
    out.write(reinterpret_cast<char*>(&indexSize), sizeof(uint32_t));
    for (int i = 0; i < mesh->mNumFaces; ++i)
    {
      const aiFace& face = mesh->mFaces[i];
      uint16_t f0 = face.mIndices[0];
      uint16_t f1 = face.mIndices[1];
      uint16_t f2 = face.mIndices[2];
      out.write(reinterpret_cast<char*>(&f0), sizeof(uint16_t));
      out.write(reinterpret_cast<char*>(&f1), sizeof(uint16_t));
      out.write(reinterpret_cast<char*>(&f2), sizeof(uint16_t));
    }
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
  std::cout << "   --single-mesh  export one single mesh" << std::endl;
  std::cout << "   --entity       export entities" << std::endl;
}


}
