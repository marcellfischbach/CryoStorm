#include <csAssimpLoader/csAssimpSkeletonAnimationLoader.hh>
#include <csCore/animation/csSkeletonAnimation.hh>
#include <csCore/resource/csVFS.hh>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace cs::assimp
{

csAssimpSkeletonAnimationLoader::csAssimpSkeletonAnimationLoader()
{

}


bool csAssimpSkeletonAnimationLoader::CanLoad(const cs::csClass *cls, const cs::csAssetLocator &locator) const
{
  const std::string &ext = locator.GetExtension();
  return cls == csSkeletonAnimationPack::GetStaticClass()
         && ext == std::string("FBX");
}


iObject *csAssimpSkeletonAnimationLoader::Load(const cs::csClass *cls, const cs::csAssetLocator &locator) const
{
  iFile *file = cs::csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!file)
  {
    return nullptr;
  }

  file->Seek(eSM_End, 0);
  long size = file->Tell();
  file->Seek(eSM_Set, 0);

  uint8_t *buffer = new uint8_t[size];
  file->Read(sizeof(uint8_t), size, buffer);

  Assimp::Importer importer;
  const aiScene    *scene = importer.ReadFileFromMemory(buffer, size,
                                                        aiProcess_Triangulate
                                                        | aiProcess_CalcTangentSpace
                                                        | aiProcess_GenNormals
                                                        | aiProcess_MakeLeftHanded
                                                        | aiProcess_FlipWindingOrder
                                                        | aiProcess_JoinIdenticalVertices
                                                        | aiProcess_OptimizeMeshes
  );

  csSkeletonAnimationPack *pack = new csSkeletonAnimationPack();

  for (int i = 0; i < scene->mNumAnimations; ++i)
  {
    aiAnimation *assimpAnimation = scene->mAnimations[i];

    csSkeletonAnimation *animation = Read(assimpAnimation);
    pack->AddAnimation(animation);

    CS_RELEASE(animation);
  }


  return pack;

}

csSkeletonAnimation *csAssimpSkeletonAnimationLoader::Read(aiAnimation *animation) const
{
  csSkeletonAnimation *result = new csSkeletonAnimation();
  result->SetName(std::string(animation->mName.C_Str()));
  result->SetNumberOfFrames((float) animation->mDuration);
  result->SetFramesPerSecond((float)animation->mTicksPerSecond);

  for (int i = 0; i < animation->mNumChannels; ++i)
  {

    aiNodeAnim  *channel = animation->mChannels[i];
    std::string channelName(channel->mNodeName.C_Str());

    for (int r = 0; r < channel->mNumRotationKeys; ++r)
    {
      aiQuatKey &rotation = channel->mRotationKeys[r];
      result->AddRotationFrame(channelName, rotation.mTime, csQuaternion(rotation.mValue.x,
                                                                         rotation.mValue.y,
                                                                         rotation.mValue.z,
                                                                         -rotation.mValue.w));
    }

    for (int r = 0; r < channel->mNumPositionKeys; ++r)
    {
      aiVectorKey &position = channel->mPositionKeys[r];
      result->AddPositionFrame(channelName, position.mTime, csVector3f(position.mValue.x,
                                                                       position.mValue.y,
                                                                       position.mValue.z));
    }
    for (int r = 0; r < channel->mNumScalingKeys; ++r)
    {
      aiVectorKey &scale = channel->mScalingKeys[r];
      result->AddScaleFrame(channelName, scale.mTime, csVector3f(scale.mValue.x,
                                                                 scale.mValue.y,
                                                                 scale.mValue.z));
    }
  }

  return result;
}


}