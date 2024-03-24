
#pragma once

#include <ceAssimpLoader/assimpexport.hh>
#include <ceCore/resource/iassetloader.hh>


struct aiAnimation;

namespace ce
{

class SkeletonAnimation;

namespace assimp
{


CE_CLASS()
class CE_ASSIMP_API AssimpSkeletonAnimationLoader : public CE_SUPER(iAssetLoader)
{
CE_CLASS_GEN_OBJECT;
public:
  AssimpSkeletonAnimationLoader();
  ~AssimpSkeletonAnimationLoader() override = default;

  virtual bool CanLoad(const Class *cls, const ResourceLocator &locator) const;

  virtual iObject *Load(const Class *cls, const ResourceLocator &locator) const;

private:

  SkeletonAnimation *Read(aiAnimation* animation) const;

};


}

}