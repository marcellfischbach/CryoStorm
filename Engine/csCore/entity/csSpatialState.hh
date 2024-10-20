
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/entity/csEntityState.hh>
#include <csCore/entity/csTransform.hh>
#include <csCore/csTypes.hh>
#include <csCore/math/csMatrix4f.hh>
#include <vector>

namespace cs
{


CS_CLASS(jclass="org.cryo.core.entity.SpatialState")
class CS_CORE_API csSpatialState : public CS_SUPER(csEntityState)
{
  CS_CLASS_GEN;
public:
  csSpatialState(const std::string &name = std::string(""));
  virtual ~csSpatialState();

  CS_FUNCTION()
  void SetStatic(bool _static);
  CS_FUNCTION()
  bool IsStatic() const;

  CS_FUNCTION()
  bool Attach(cs::csSpatialState *child);
  CS_FUNCTION()
  bool DetachSelf();
  CS_FUNCTION()
  bool Detach(cs::csSpatialState *child);
  CS_FUNCTION()
  cs::csSpatialState* GetParent();
  const csSpatialState* GetParent() const;

  CS_FUNCTION()
  size_t GetNumberOfChildren() const;
  CS_FUNCTION()
  cs::csSpatialState* GetChild(size_t idx);
  const csSpatialState* GetChild(size_t idx) const;

  /**
   * @name Transformation
   * @{
   */
  csTransform GetTransform();
  void SetTransform(const csTransform &transform);
  const csMatrix4f& GetGlobalMatrix() const;
  const csMatrix4f& GetLocalMatrix() const;
  void SetLocalMatrix(const csMatrix4f &matrix);
//  void LookAt(const Vector3f & from, const Vector3f & at, const Vector3f & up = Vector3f(0.0f, 1.0f, 0.0f));
  void UpdateTransformation();
  /**
   * @}
   */


  
protected:
  void UpdateEntity(csEntity* oldEntity, csEntity* newEntity) override;
  virtual void TransformationUpdatedPreChildren();
  virtual void TransformationUpdatedPostChildren();

  
private:
  void UpdateGlobalMatrix();


  bool m_static;
  csSpatialState * m_parent;
  std::vector<csSpatialState*> m_children;

  csMatrix4f m_localMatrix;

  csMatrix4f m_globalMatrix;

};

}