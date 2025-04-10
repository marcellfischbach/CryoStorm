
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>

namespace cs
{

class csSkeleton;

CS_CLASS()
class csSkeletonLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
CS_CLASS_GEN_OBJECT;
public:
  csSkeletonLoader();
  ~csSkeletonLoader() override = default;


protected:
  csOwned<iAsset> Load(const file::csCryoFile *file, const csAssetLocator &locator) const override ;

private:
  void ScanBone (const file::csCryoFileElement *boneElement, csSkeleton *skeleton, int32_t parentBondIdx) const;

  void ReadTransform (const file::csCryoFileElement *transformElement, csSkeleton* skeleton, int32_t boneIdx) const;

};

} // cs
