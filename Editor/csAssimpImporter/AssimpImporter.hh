//
// Created by MCEL on 24.02.2025.
//

#pragma once

#include <csAssimpImporter/Export.hh>
#include <csImporter/Importer.hh>

struct aiMesh;
struct aiScene;

namespace cs::imp
{

class CS_ASS_IMP_API AssimpImporter : public iImporter
{
public:
  AssimpImporter() = default;
  ~AssimpImporter() override = default;

  bool CanImport(const std::fs::path &path, const std::vector<std::string> &args) const override;

  bool Import(const std::fs::path &path, const std::vector<std::string> &args) const override;

  void PrintUsage() const override;


private:
  void GenerateRenderMeshes(const std::fs::path &path, const aiScene *scene) const;
  void GenerateMesh(const std::fs::path &path, const aiMesh* mesh, const aiScene *scene) const;
  void GenerateRenderMesh(const std::fs::path &path, const aiMesh* mesh, const aiScene *scene) const;

  void GeneratesMeshesEntity(const std::fs::path& path, const aiScene* scene) const;
};


}