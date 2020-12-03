
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/matrix4f.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <spcCore/graphics/ilight.hh>
#include <vector>

namespace spc
{

struct iDevice;
struct iMaterial;
struct iRenderMesh;

SPC_CLASS()
class SPC_CORE_API SceneMesh : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  SceneMesh();
  virtual ~SceneMesh();

  void SetMaterial(iMaterial * material);
  iMaterial* GetMaterial();
  const iMaterial* GetMaterial() const;

  void SetMesh(iRenderMesh * mesh);
  iRenderMesh* GetMesh();
  const iRenderMesh* GetMesh() const;

  void SetModelMatrix(const Matrix4f & modelMatrix);
  const Matrix4f& GetModelMatrix() const;

  void ClearLights();
  void AddLight(iLight * light);
  const std::vector<const iLight*>& GetLights() const;

  void Render(iDevice * device, eRenderPass pass);


private:

  iMaterial* m_material;
  iRenderMesh* m_mesh;

  Matrix4f m_modelMatrix;

  std::vector<iLight*> m_lights;

};

}