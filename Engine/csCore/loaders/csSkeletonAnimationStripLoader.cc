//
// Created by MCEL on 11.04.2025.
//

#include <csCore/loaders/csSkeletonAnimationStripLoader.hh>
#include <csCore/animation/csSkeletonAnimationStrip.hh>
#include <csCore/resource/csBinaryData.hh>

namespace cs
{

csSkeletonAnimationStripLoader::csSkeletonAnimationStripLoader()
{
  RegisterType("SKELETON_ANIMATION");
}

csOwned<iAsset> csSkeletonAnimationStripLoader::Load(const file::csCryoFile *file, const cs::csAssetLocator &locator) const
{
  const file::csCryoFileElement *root = file->Root();

  const file::csCryoFileElement *animationElement = root->GetChild("skeletonAnimation");
  if (!animationElement)
  {
    return nullptr;
  }


  auto animation = new csSkeletonAnimationStrip();
  animation->SetName(animationElement->GetAttribute("name", locator.GetFilename()));


  auto durationElement = animationElement->GetChild("duration");
  if (durationElement)
  {
    animation->SetNumberOfFrames(durationElement->GetAttribute(0, 0.0f));
  }

  auto fpsElement = animationElement->GetChild("fps");
  if (fpsElement)
  {
    animation->SetFramesPerSecond(fpsElement->GetAttribute(0, 24.0f));
  }

  auto channelsElement = animationElement->GetChild("channels");
  if (channelsElement)
  {

    for (size_t i = 0, in = channelsElement->GetNumberOfChildren(); i < in; ++i)
    {
      auto channelElement = channelsElement->GetChild(i);
      ReadChannel(animation, file, channelElement);
    }
  }
  return animation;
}

void csSkeletonAnimationStripLoader::ReadChannel(csSkeletonAnimationStrip *animation,
                                                 const file::csCryoFile *file,
                                                 const file::csCryoFileElement *channelElement)
{
  if (!channelElement || !channelElement->HasAttribute("name"))
  {
    return;
  }

  std::string channelName = channelElement->GetAttribute("name", "");
  if (channelName.empty())
  {
    return;
  }


  if (channelElement->HasAttribute("positions"))
  {
    const std::vector<PosKey> &posKeys = ReadPosKey(file, channelElement->GetAttribute("positions", ""));
    for (const auto &posKey: posKeys)
    {
      animation->AddPositionFrame(channelName, posKey.frame, posKey.pos);
    }
  }

  if (channelElement->HasAttribute("rotations"))
  {
    const std::vector<RotKey> &rotKeys = ReadRotKey(file, channelElement->GetAttribute("rotations", ""));
    for (const auto &rotKey: rotKeys)
    {
      animation->AddRotationFrame(channelName, rotKey.frame, rotKey.rot);
    }
  }

  if (channelElement->HasAttribute("scalings"))
  {
    const std::vector<ScaleKey> &scaleKeys = ReadScaleKey(file, channelElement->GetAttribute("scalings", ""));
    for (const auto &scaleKey: scaleKeys)
    {
      animation->AddScaleFrame(channelName, scaleKey.frame, scaleKey.scale);
    }
  }
}

std::vector<csSkeletonAnimationStripLoader::PosKey> csSkeletonAnimationStripLoader::ReadPosKey(const file::csCryoFile *file,
                                                                                               const std::string &name)
{
  const std::vector<uint8_t> &data = file->GetData(name);
  if (!data.empty())
  {
    csBinaryInputStream is(data);
    auto                version = is.Read<uint32_t>();
    if (version == 0x01)
    {
      return ReadPosKey_V1(is);
    }
  }
  return std::vector<PosKey>();
}

std::vector<csSkeletonAnimationStripLoader::PosKey> csSkeletonAnimationStripLoader::ReadPosKey_V1(csBinaryInputStream &is)
{
  auto                numValues = is.Read<uint32_t>();
  std::vector<PosKey> result;
  result.resize(numValues);
  is.Read(reinterpret_cast<uint8_t*>(result.data()), sizeof(PosKey) * numValues);
  return result;
}



std::vector<csSkeletonAnimationStripLoader::RotKey> csSkeletonAnimationStripLoader::ReadRotKey(const file::csCryoFile *file,
                                                                                               const std::string &name)
{
  const std::vector<uint8_t> &data = file->GetData(name);
  if (!data.empty())
  {
    csBinaryInputStream is(data);
    auto                version = is.Read<uint32_t>();
    if (version == 0x01)
    {
      return ReadRotKey_V1(is);
    }
  }
  return std::vector<RotKey>();
}

std::vector<csSkeletonAnimationStripLoader::RotKey> csSkeletonAnimationStripLoader::ReadRotKey_V1(csBinaryInputStream &is)
{
  auto                numValues = is.Read<uint32_t>();
  std::vector<RotKey> result;
  result.resize(numValues);
  is.Read(reinterpret_cast<uint8_t*>(result.data()), sizeof(RotKey) * numValues);
  return result;
}


std::vector<csSkeletonAnimationStripLoader::ScaleKey> csSkeletonAnimationStripLoader::ReadScaleKey(const file::csCryoFile *file,
                                                                                                   const std::string &name)
{
  const std::vector<uint8_t> &data = file->GetData(name);
  if (!data.empty())
  {
    csBinaryInputStream is(data);
    auto                version = is.Read<uint32_t>();
    if (version == 0x01)
    {
      return ReadScaleKey_V1(is);
    }
  }
  return std::vector<ScaleKey>();
}

std::vector<csSkeletonAnimationStripLoader::ScaleKey> csSkeletonAnimationStripLoader::ReadScaleKey_V1(csBinaryInputStream &is)
{
  auto                numValues = is.Read<uint32_t>();
  std::vector<ScaleKey> result;
  result.resize(numValues);
  is.Read(reinterpret_cast<uint8_t*>(result.data()), sizeof(ScaleKey) * numValues);
  return result;
}

} // cs