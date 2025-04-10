#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/csMesh.hh>
#include <csCore/graphics/csSkeleton.hh>

namespace cs
{


CS_CLASS()
class CS_CORE_API csSkeletonMesh : public CS_SUPER(csMesh)
{
CS_CLASS_GEN;
public:
  class CS_CORE_API SubMesh : public csMesh::SubMesh
  {
    friend class csSkeletonMesh;
  public:
    SubMesh();

    void SetOriginBoneIndices(const std::vector<csVector4i> &originBoneIndices);
    const std::vector<csVector4i> &GetOriginBoneIndices() const;

    void AddBone (int32_t idx, const std::string &boneName);

    void ApplySkeleton (const csSkeleton* skeleton);

  private:

    std::vector<csVector4i> m_originBoneIndices;
    std::map<int32_t, std::string> m_bones;

  };

  csSkeletonMesh();
  ~csSkeletonMesh() override = default;

  void SetSkeleton (const csSkeleton* skeleton);

protected:
  csMesh::SubMesh *CreateSubMesh() override;
  csRef<const csSkeleton> m_skeleton;

};

}
