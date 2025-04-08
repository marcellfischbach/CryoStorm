#pragma  once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/graphics/iTerrainMesh.hh>
#include <csCore/graphics/csVertexDeclaration.hh>

namespace cs
{
struct iVertexBuffer;
struct iIndexBuffer;
}

namespace cs::opengl
{


CS_CLASS()
class csGL4TerrainMeshCPU : public CS_SUPER(iTerrainMesh)
{
CS_CLASS_GEN_OBJECT;
public:
  struct Line
  {
    size_t   v0;
    size_t     v1;
    csVector3f min;
    csVector3f max;
    csVector3f center;
    size_t     absStepSize;
    size_t   useStepSize = 0;
    size_t   patchSize;
    bool UpdateStepSize(const csVector3f& refPoint, size_t override);
  };

  struct Patch
  {
    bool         first;
    eTerrainSize patchSize;
    size_t       vcenter;
    size_t       v00;
    size_t       v01;
    size_t       v10;
    size_t       v11;
    Line         lineTop;
    Line         lineBottom;
    Line         lineLeft;
    Line         lineRight;
    uint32_t     * buffer;
    size_t       bufferCount;
    bool UpdateIndices(const csVector3f& refPoint, eTerrainSize size);

  private:
    void RegenerateIndices(eTerrainSize size);
    size_t GenerateGeneric(int64_t majorUnitSize,
                           int64_t minorUnitSize,
                           size_t stepSize,
                           size_t adjacentStepSize,
                           bool flip,
                           size_t bufferIdx);
  };
public:
  csGL4TerrainMeshCPU(uint32_t vao,
                      const csVertexDeclaration& vd,
                      iVertexBuffer* vb,
                      iIndexBuffer* ib,
                      const csBoundingBox& bbox,
                      const std::vector<Patch>& patches,
                      eTerrainSize terrainSize,
                      eTerrainSize patchSize);
  ~csGL4TerrainMeshCPU() override = default;

  CS_NODISCARD virtual const csBoundingBox& GetBoundingBox() const override;
  CS_NODISCARD virtual const csVertexDeclaration& GetVertexDeclaration() const override;
  void SetReferencePoint(const csVector3f& refPoint) override;

  csOwned<iRenderMeshModifier> Modify () override;

  void Render(iDevice* graphics, eRenderPass pass) override;

#if _DEBUG
  CS_NODISCARD  Size GetNumberOfTriangles() const override;
#endif

private:
  void Update();
  bool UpdatePatches();
  void RebuildIndices();


  uint32_t            m_vao;
  csVertexDeclaration m_vd;
  csRef<iVertexBuffer> m_vb;
  csRef<iIndexBuffer> m_ib;
  csBoundingBox      m_bbox;
  std::vector<Patch> m_patches;
  std::vector<uint32_t>     m_indexBuffer;
  size_t                    m_indexBufferSize;
  eTerrainSize              m_terrainSize;
  eTerrainSize m_patchSize;
  csVector3f   m_referencePoint;
};

CS_CLASS()
class csGL4TerrainMeshGeneratorCPU : public CS_SUPER(iTerrainMeshGenerator)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4TerrainMeshGeneratorCPU(bool compat);
  ~csGL4TerrainMeshGeneratorCPU() override = default;

  void SetSize(eTerrainSize size) override;
  void SetPatchSize(eTerrainSize size) override;
  void SetSize(const csVector3f& min, const csVector3f& max) override;
  void SetNormalizedHeightData(const std::vector<float>& heightData) override;
  void SetHeightData(const std::vector<float>& heightData) override;

  csOwned<iTerrainMesh> Generate() override;
private:
  void GenerateVerticesUVs(std::vector<csVector3f>& vertices, std::vector<csVector2f>& uv);
  void GenerateNormals(std::vector<csVector3f>& vertices, std::vector<csVector3f>& normals);
  void GeneratePatches(std::vector<csVector3f>& vertices, std::vector<csGL4TerrainMeshCPU::Patch>& patches);
  static void EvalLine(std::vector<csVector3f>& vertices,
                       size_t v0,
                       size_t v1,
                       size_t stepSize,
                       size_t patchSize,
                       csGL4TerrainMeshCPU::Line& line);

  bool m_compat;
  eTerrainSize       m_size      = eTerrainSize::TS_1025;
  eTerrainSize       m_patchSize = eTerrainSize::TS_65;
  csVector3f         m_min       = csVector3f(-1000.0f, 0.0f, -1000.0f);
  csVector3f         m_max       = csVector3f(1000.0f, 100.0f, 1000.0f);
  std::vector<float> m_heightData;
};

}