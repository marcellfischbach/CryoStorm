
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/entitystate.hh>
#include <ceCore/entity/transform.hh>
#include <ceCore/types.hh>
#include <ceCore/math/matrix4f.hh>
#include <vector>

namespace ce
{


CS_CLASS()
class CS_CORE_API SpatialState : public CS_SUPER(EntityState)
{
  CS_CLASS_GEN;
public:
  SpatialState(const std::string &name = std::string(""));
  virtual ~SpatialState();

  void SetStatic(bool _static);
  bool IsStatic() const;
  
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
  Transform GetTransform();
  void SetTransform (const Transform &transform);
  const Matrix4f& GetGlobalMatrix() const;
  const Matrix4f& GetLocalMatrix() const;
  void SetLocalMatrix(const Matrix4f &matrix);
//  void LookAt(const Vector3f & from, const Vector3f & at, const Vector3f & up = Vector3f(0.0f, 1.0f, 0.0f));
  void UpdateTransformation();
  /**
   * @}
   */


  
protected:
  void UpdateEntity(Entity* oldEntity, Entity* newEntity) override;
  virtual void TransformationUpdatedPreChildren();
  virtual void TransformationUpdatedPostChildren();

  
private:
  void UpdateGlobalMatrix();


  bool m_static;
  SpatialState* m_parent;
  std::vector<SpatialState*> m_children;

  Matrix4f m_localMatrix;

  Matrix4f m_globalMatrix;

};

}