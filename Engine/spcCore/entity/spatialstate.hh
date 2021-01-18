
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/entity/entitystate.hh>
#include <spcCore/types.hh>
#include <spcCore/math/matrix4f.hh>
#include <vector>
#include <spcCore/math/transform.hh>

namespace spc
{


SPC_CLASS()
class SPC_CORE_API SpatialState : public SPC_SUPER(EntityState)
{
  SPC_CLASS_GEN;
public:
  SpatialState(const std::string &name = std::string(""));
  virtual ~SpatialState();
  
  bool Attach(SpatialState *child);
  bool DetachSelf();
  bool Detach(SpatialState *child);
  SpatialState* GetParent();
  const SpatialState* GetParent() const;

  Size GetNumberOfChildren() const;
  SpatialState* GetChild(Size idx);
  const SpatialState* GetChild(Size idx) const;

  /**
   * @name Transformation
   * @{
   */
  void SetTransform(const Transform & transform);
  const Transform &GetTransform() const;

  const Matrix4f& GetGlobalMatrix() const;
  /**
   * @}
   */
  
protected:
  void UpdateEntity(Entity* oldEntity, Entity* newEntity) override;

  
private:
  SpatialState* m_parent;
  std::vector<SpatialState*> m_children;

  Transform m_transform;

  mutable bool m_globalMatrixDirty;
  mutable Matrix4f m_globalMatrix;
  void SetGlobalMatrixDirty();

};

}