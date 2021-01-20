

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
    StaticMeshState(const std::string& name = std::string(""));
    virtual ~StaticMeshState();


    void SetMesh(Mesh* mesh);
    const Mesh* GetMesh() const;
    Mesh* GetMesh();

    /**
     * @name Livecycle
    * @{
    */
    void OnAttachedToWorld(World* world) override;
    void OnDetachedFromWorld(World* world) override;

    void Update(float tpf) override;
    /**
     * @}
     */

protected:
    void TransformationUpdated() override;

private:
    void AddMeshToScene(World* world);
    void RemoveMeshFromScene(World* world);
    Mesh* m_mesh;
    std::vector<GfxSceneMesh*> m_gfxMeshes;
};


}