//
// Created by MCEL on 11.04.2025.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>
#include <csCore/math/csVector3f.hh>
#include <csCore/math/csQuaternion.hh>
namespace cs
{
class csBinaryInputStream;
class csSkeletonAnimationStrip;

CS_CLASS()
class CS_CORE_API csSkeletonAnimationStripLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
CS_CLASS_GEN_OBJECT;
public:
  csSkeletonAnimationStripLoader();
  ~csSkeletonAnimationStripLoader() override = default;


protected:
  csOwned<iAsset> Load(const file::csCryoFile *file, const csAssetLocator &locator) const override;

  struct PosKey
  {
    float frame;
    csVector3f pos;
  };

  struct RotKey
  {
    float frame;
    csQuaternion rot;
  };
  struct ScaleKey
  {
    float frame;
    csVector3f scale;
  };

  static void ReadChannel(csSkeletonAnimationStrip *animation, const file::csCryoFile* file, const file::csCryoFileElement* channelElement);
  static std::vector<PosKey> ReadPosKey (const file::csCryoFile *file, const std::string &name);
  static std::vector<PosKey> ReadPosKey_V1 (csBinaryInputStream &is);
  static std::vector<RotKey> ReadRotKey (const file::csCryoFile *file, const std::string &name);
  static std::vector<RotKey> ReadRotKey_V1 (csBinaryInputStream &is);
  static std::vector<ScaleKey> ReadScaleKey (const file::csCryoFile *file, const std::string &name);
  static std::vector<ScaleKey> ReadScaleKey_V1 (csBinaryInputStream &is);


};
} // cs
