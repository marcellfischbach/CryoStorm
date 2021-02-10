
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/matrix4f.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <spcCore/graphics/scene/gfxlight.hh>
#include <vector>

namespace spc
{

struct iDevice;
struct iMaterial;
struct iRenderMesh;

SPC_CLASS()
class SPC_CORE_API GfxSceneMesh : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GfxSceneMesh();
  virtual ~GfxSceneMesh();

  void SetStatic(bool _static);
  bool IsStatic() const;

  void SetMaterial(iMaterial * material);
  iMaterial* GetMaterial();
  const iMaterial* GetMaterial() const;

  void SetMesh(iRenderMesh * mesh);
  iRenderMesh* GetMesh();
  const iRenderMesh* GetMesh() const;

  void SetModelMatrix(const Matrix4f & modelMatrix);
  const Matrix4f& GetModelMatrix() const;

  void ClearLights();
  void AddLight(GfxLight * light);
  void RemoveLight(GfxLight * light);
  Size GetNumberOfLights() const;
  const std::vector< GfxLight*>& GetLights() const;

  void Render(iDevice * device, eRenderPass pass);


private:
  bool m_static;
  iMaterial* m_material;
  iRenderMesh* m_mesh;

  Matrix4f m_modelMatrix;

  std::vector<GfxLight*> m_lights;

};

}