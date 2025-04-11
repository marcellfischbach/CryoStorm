//
// Created by MCEL on 11.04.2025.
//

#pragma once

#include <csAssimpImporter/AssimpSkeletonExporter.hh>
#include <string>

struct aiAnimation;
struct aiScene;

namespace cs::imp
{

class AssimpSkeletonAnimationExporter
{
public:
  AssimpSkeletonAnimationExporter(const aiScene *scene,
                                  aiAnimation *animation,
                                  AssimpSkeletonExporter *skeletonExporter);


  void Export(const std::string &filename) const;
private:
  aiAnimation *m_animation;

  AssimpSkeletonExporter *m_skeletonExporter;
};

}