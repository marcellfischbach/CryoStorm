#include <ceAssimpLoader/assimpskeletonanimationloader.hh>
#include <ceCore/animation/skeletonanimation.hh>
#include <ceCore/resource/vfs.hh>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace ce::assimp
{

AssimpSkeletonAnimationLoader::AssimpSkeletonAnimationLoader()
{
  CS_CLASS_GEN_CONSTR;
}


bool AssimpSkeletonAnimationLoader::CanLoad(const ce::Class *cls, const ce::ResourceLocator &locator) const
{
  const std::string &ext = locator.GetExtension();
  return cls == SkeletonAnimationPack::GetStaticClass()
         && ext == std::string("FBX");
}


iObject *AssimpSkeletonAnimationLoader::Load(const ce::Class *cls, const ce::ResourceLocator &locator) const
{
  iFile *file = ce::VFS::Get()->Open(locator, eAM_Read, eOM_Binary);
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

  SkeletonAnimationPack *pack = new SkeletonAnimationPack();

  for (int i = 0; i < scene->mNumAnimations; ++i)
  {
    aiAnimation *assimpAnimation = scene->mAnimations[i];

    SkeletonAnimation *animation = Read(assimpAnimation);
    pack->AddAnimation(animation);

    CS_RELEASE(animation);
  }


  return pack;

}

SkeletonAnimation *AssimpSkeletonAnimationLoader::Read(aiAnimation *animation) const
{
  SkeletonAnimation *result = new SkeletonAnimation();
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
      result->AddRotationFrame(channelName, rotation.mTime, Quaternion(rotation.mValue.x,
                                                                       rotation.mValue.y,
                                                                       rotation.mValue.z,
                                                                       -rotation.mValue.w));
    }

    for (int r = 0; r < channel->mNumPositionKeys; ++r)
    {
      aiVectorKey &position = channel->mPositionKeys[r];
      result->AddPositionFrame(channelName, position.mTime, Vector3f(position.mValue.x,
                                                                     position.mValue.y,
                                                                     position.mValue.z));
    }
    for (int r = 0; r < channel->mNumScalingKeys; ++r)
    {
      aiVectorKey &scale = channel->mScalingKeys[r];
      result->AddScaleFrame(channelName, scale.mTime, Vector3f(scale.mValue.x,
                                                               scale.mValue.y,
                                                               scale.mValue.z));
    }
  }

  return result;
}


}