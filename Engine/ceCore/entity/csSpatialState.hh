
#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/entity/csEntityState.hh>
#include <ceCore/entity/csTransform.hh>
#include <ceCore/csTypes.hh>
#include <ceCore/math/csMatrix4f.hh>
#include <vector>

namespace cryo
{


CS_CLASS()
class CS_CORE_API csSpatialState : public CS_SUPER(csEntityState)
{
  CS_CLASS_GEN;
public:
  csSpatialState(const std::string &name = std::string(""));
  virtual ~csSpatialState();

  void SetStatic(bool _static);
  bool IsStatic() const;
  
  bool Attach(csSpatialState *child);
  bool DetachSelf();
  bool Detach(csSpatialState *child);
  csSpatialState* GetParent();
  const csSpatialState* GetParent() const;

  Size GetNumberOfChildren() const;
  csSpatialState* GetChild(Size idx);
  const csSpatialState* GetChild(Size idx) const;

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