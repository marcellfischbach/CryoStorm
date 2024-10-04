
#pragma once

#include <csAssimpLoader/csAssimpExport.hh>
#include <csCore/resource/iAssetLoader.hh>


struct aiAnimation;

namespace cryo
{

class csSkeletonAnimation;

namespace assimp
{


CS_CLASS()
class CS_ASSIMP_API csAssimpSkeletonAnimationLoader : public CS_SUPER(iAssetLoader)
{
CS_CLASS_GEN_OBJECT;
public:
  csAssimpSkeletonAnimationLoader();
  ~csAssimpSkeletonAnimationLoader() override = default;

  virtual bool CanLoad(const csClass *cls, const csResourceLocator &locator) const;

  virtual iObject *Load(const csClass *cls, const csResourceLocator &locator) const;

private:

  csSkeletonAnimation *Read(aiAnimation* animation) const;

};


}

}