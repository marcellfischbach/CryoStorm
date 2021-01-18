

#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/entity/spatialstate.hh>


namespace spc
{

class Mesh;
class GfxSceneMesh;

SPC_CLASS()
class SPC_CORE_API StaticMeshState : public SPC_SUPER(SpatialState)
{
  SPC_CLASS_GEN;
public:
  StaticMeshState(const std::string & name = std::string(""));
  virtual ~StaticMeshState();


  void SetMesh(Mesh * mesh);
  const Mesh* GetMesh() const;
  Mesh* GetMesh();

  /**
 * @name Livecycle
 * @{
 */
  virtual void OnAttachedToWorld(World * world);
  virtual void OnDetachedFromWorld(World * world);

  /**
   * @}
   */

protected:
  void TransformationUpdated() override;

private:
  void AddMeshToScene(World * world);
  void RemoveMeshFromScene(World * world);
  Mesh* m_mesh;
  std::vector<GfxSceneMesh*>  m_gfxMeshes;
};


}