
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/erenderpass.hh>

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

  void Render(iDevice * device, eRenderPass pass);


private:

  iMaterial* m_material;
  iRenderMesh* m_mesh;

};

}