
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/entity/entitystate.hh>
#include <spcCore/entity/transform.hh>
#include <spcCore/types.hh>
#include <spcCore/math/matrix4f.hh>
#include <vector>

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
  Transform &GetTransform();
  const Transform &GetTransform() const;
  void FinishTransformation();
  const Matrix4f& GetGlobalMatrix() const;
  void UpdateTransformation();
  /**
   * @}
   */


  
protected:
  void UpdateEntity(Entity* oldEntity, Entity* newEntity) override;
  virtual void TransformationUpdated();

  
private:
  void UpdateGlobalMatrix() const;
  void UpdateFlagUpdateGlobalMatrix();
  void UpdateFlagRequestHierarchyTransformationUpdate();

  SpatialState* m_parent;
  std::vector<SpatialState*> m_children;

  Transform m_transform;

  enum TransformationState
  {
    eTS_GlobalMatrixDirty = 1,
    eTS_RequestHierarchyTransformationUpdate = 2,
  };

  mutable UInt8 m_transformationState;
  mutable Matrix4f m_globalMatrix;

};

}