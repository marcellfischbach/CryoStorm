
#pragma once

#include <ceAssimpLoader/assimpexport.hh>
#include <ceCore/resource/iassetloader.hh>


struct aiAnimation;

namespace cryo
{

class SkeletonAnimation;

namespace assimp
{


CS_CLASS()
class CS_ASSIMP_API AssimpSkeletonAnimationLoader : public CS_SUPER(iAssetLoader)
{
CS_CLASS_GEN_OBJECT;
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