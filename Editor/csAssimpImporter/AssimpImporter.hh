//
// Created by MCEL on 24.02.2025.
//

#pragma once

#include <csAssimpImporter/Export.hh>
#include <csImporter/Importer.hh>
#include <ostream>

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

  void GenerateMesh(const std::fs::path &path, const aiScene *scene) const;
  void GenerateMeshEntity(const std::fs::path& path, const aiScene* scene) const;


  void GenerateMaterials (const std::fs::path &path, const aiScene* scene) const;

  void GenerateSkeleton(const std::fs::path &path, const aiScene* scene) const;
  void GenerateAnimations(const std::fs::path &path, const aiScene* scene) const;

};


}

extern "C" CS_ASS_IMP_API void cs__imp__AssimpImporter_create();
