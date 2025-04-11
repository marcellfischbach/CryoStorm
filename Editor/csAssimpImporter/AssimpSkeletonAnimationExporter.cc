//
// Created by MCEL on 11.04.2025.
//

#include <csAssimpImporter/AssimpSkeletonAnimationExporter.hh>
#include <csCryoFile/csCryoFile.hh>
#include <assimp/scene.h>
#include <sstream>

using namespace cs::file;

namespace cs::imp
{


AssimpSkeletonAnimationExporter::AssimpSkeletonAnimationExporter(const aiScene *scene,
                                                                 aiAnimation *animation,
                                                                 AssimpSkeletonExporter *skeletonExporter)
    : m_animation(animation)
    , m_skeletonExporter(skeletonExporter)
{

}

void AssimpSkeletonAnimationExporter::Export(const std::string &filename) const
{
  if (m_animation->mChannels == 0)
  {
    return;
  }

  csCryoFile file;

  uint32_t posId = 0;
  uint32_t rotId = 0;
  uint32_t scaId = 0;

  auto animationElement = file.Root()->AddChild("skeletonAnimation");
  animationElement->AddStringAttribute("name", m_animation->mName.C_Str());
  animationElement->AddChild("duration")->AddAttribute(std::to_string(m_animation->mDuration));
  animationElement->AddChild("fps")->AddAttribute(std::to_string(m_animation->mTicksPerSecond));

  auto channelsElement = animationElement->AddChild("channels");
  for (int i = 0; i < m_animation->mNumChannels; ++i)
  {
    auto channel = m_animation->mChannels[i];
    std::string channelName (channel->mNodeName.C_Str());
    auto boneIdx = m_skeletonExporter->GetBoneIndex(channelName);
    if (boneIdx == AssimpSkeletonExporter::IllegalBoneID)
    {
      continue;
    }

    auto channelElement = channelsElement->AddChild("channel");
    channelElement->AddStringAttribute("name", channelName);

    // output the positions
    {
      std::ostringstream ostream;
      uint32_t           version      = 0x01;
      uint32_t           numPositions = channel->mNumPositionKeys;
      ostream.write(reinterpret_cast<const char *>(&version), sizeof(uint32_t));
      ostream.write(reinterpret_cast<const char *>(&numPositions), sizeof(uint32_t));
      for (size_t j = 0, jn = channel->mNumPositionKeys; j < jn; j++)
      {
        auto positionKey = channel->mPositionKeys[j];

        float time = (float) positionKey.mTime;
        ostream.write(reinterpret_cast<const char *>(&time), sizeof(float));
        ostream.write(reinterpret_cast<const char *>(&positionKey.mValue.x), sizeof(float));
        ostream.write(reinterpret_cast<const char *>(&positionKey.mValue.y), sizeof(float));
        ostream.write(reinterpret_cast<const char *>(&positionKey.mValue.z), sizeof(float));
      }

      std::string streamData = ostream.str();
      std::string streamName = "#Pos" + std::to_string(posId++);
      file.AddData(streamName,
                   streamData.size(),
                   reinterpret_cast<uint8_t *>(streamData.data()));

      channelElement->AddStringAttribute("positions", streamName);
    }


    // output the rotations
    {
      std::ostringstream ostream;
      uint32_t version      = 0x01;
      uint32_t numRotations = channel->mNumRotationKeys;
      ostream.write(reinterpret_cast<const char *>(&version), sizeof(uint32_t));
      ostream.write(reinterpret_cast<const char *>(&numRotations), sizeof(uint32_t));
      for (size_t j = 0, jn = channel->mNumRotationKeys; j < jn; j++)
      {
        auto rotationKey = channel->mRotationKeys[j];

        float time = (float) rotationKey.mTime;
        ostream.write(reinterpret_cast<const char *>(&time), sizeof(float));
        ostream.write(reinterpret_cast<const char *>(&rotationKey.mValue.x), sizeof(float));
        ostream.write(reinterpret_cast<const char *>(&rotationKey.mValue.y), sizeof(float));
        ostream.write(reinterpret_cast<const char *>(&rotationKey.mValue.z), sizeof(float));
        ostream.write(reinterpret_cast<const char *>(&rotationKey.mValue.w), sizeof(float));
      }

      std::string streamData = ostream.str();
      std::string streamName = "#Rot" + std::to_string(rotId++);
      file.AddData(streamName,
                   streamData.size(),
                   reinterpret_cast<uint8_t *>(streamData.data()));

      channelElement->AddStringAttribute("rotations", streamName);
    }

    // output the scaling
    {
      std::ostringstream ostream;
      uint32_t version     = 0x01;
      uint32_t numScalings = channel->mNumScalingKeys;
      ostream.write(reinterpret_cast<const char *>(&version), sizeof(uint32_t));
      ostream.write(reinterpret_cast<const char *>(&numScalings), sizeof(uint32_t));
      for (size_t j = 0, jn = channel->mNumScalingKeys; j < jn; j++)
      {
        auto positionKey = channel->mScalingKeys[j];

        float time = (float) positionKey.mTime;
        ostream.write(reinterpret_cast<const char *>(&time), sizeof(float));
        ostream.write(reinterpret_cast<const char *>(&positionKey.mValue.x), sizeof(float));
        ostream.write(reinterpret_cast<const char *>(&positionKey.mValue.y), sizeof(float));
        ostream.write(reinterpret_cast<const char *>(&positionKey.mValue.z), sizeof(float));
      }

      std::string streamData = ostream.str();
      std::string streamName = "#Scale" + std::to_string(scaId++);
      file.AddData(streamName,
                   streamData.size(),
                   reinterpret_cast<uint8_t *>(streamData.data()));

      channelElement->AddStringAttribute("scalings", streamName);
    }

  }

  std::ofstream out;
  out.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  file.Write(out, true, 2);
  out.close();
}


}