#pragma  once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/graphics/iterrainmesh.hh>
#include <ceCore/graphics/vertexdeclaration.hh>

namespace ce
{
struct iVertexBuffer;
struct iIndexBuffer;
}

namespace ce::opengl
{


CE_CLASS()
class GL4TerrainMeshCPU : public CE_SUPER(iTerrainMesh)
{
CE_CLASS_GEN_OBJECT;
public:
  struct Line
  {
    size_t   v0;
    size_t   v1;
    Vector3f min;
    Vector3f max;
    size_t   absStepSize;
    size_t   useStepSize;
    bool UpdateStepSize (const Vector3f &refPoint);
  };

  struct Patch
  {
    eTerrainSize patchSize;
    size_t       v00;
    size_t       v01;
    size_t       v10;
    size_t       v11;
    Line         lineTop;
    Line         lineBottom;
    Line         lineLeft;
    Line         lineRight;
    uint32_t     *buffer = nullptr;
    size_t       bufferSize;
    bool UpdateIndices (const Vector3f &refPoint, eTerrainSize size);
  };
public:
  GL4TerrainMeshCPU(uint32_t vao,
                    const VertexDeclaration& vd,
                    iVertexBuffer* vb,
                    iIndexBuffer* ib,
                    const BoundingBox& bbox,
                    const std::vector<Patch>& patches,
                    eTerrainSize terrainSize,
                    eTerrainSize patchSize);
  ~GL4TerrainMeshCPU() override = default;


  CE_NODISCARD virtual const BoundingBox& GetBoundingBox() const override;
  CE_NODISCARD virtual const VertexDeclaration& GetVertexDeclaration() const override;
  void SetReferencePoint(const Vector3f& refPoint) override;
  void Render(iDevice * graphics, eRenderPass pass) override;

  void Update();

private:
  uint32_t          m_vao;
  VertexDeclaration m_vd;
  iVertexBuffer* m_vb;
  iIndexBuffer * m_ib;
  BoundingBox           m_bbox;
  std::vector<Patch>    m_patches;
  std::vector<uint32_t> m_indexBuffer;
  eTerrainSize          m_terrainSize;
  eTerrainSize          m_patchSize;
  Vector3f              m_referencePoint;
};

CE_CLASS()
class GL4TerrainMeshGeneratorCPU : public CE_SUPER(iTerrainMeshGenerator)
{
CE_CLASS_GEN_OBJECT;
public:
  GL4TerrainMeshGeneratorCPU();
  ~GL4TerrainMeshGeneratorCPU() override = default;

  void SetSize(eTerrainSize size) override;
  void SetPatchSize(eTerrainSize size) override;
  void SetSize(const Vector3f& min, const Vector3f& max) override;
  void SetNormalizedHeightData(const std::vector<float>& heightData) override;

  iTerrainMesh* Generate() override;
private:
  void GenerateVerticesUVs(std::vector<Vector3f>& vertices, std::vector<Vector2f>& uv);
  void GenerateNormals(std::vector<Vector3f>& vertices, std::vector<Vector3f>& normals);
  void GeneratePatches(std::vector<Vector3f>& vertices, std::vector<GL4TerrainMeshCPU::Patch>& patches);
  static void EvalLine(std::vector<Vector3f>& vertices,
                       size_t v0,
                       size_t v1,
                       size_t stepSize,
                       GL4TerrainMeshCPU::Line& line);

  eTerrainSize       m_size      = eTerrainSize::TS_1025;
  eTerrainSize       m_patchSize = eTerrainSize::TS_65;
  Vector3f           m_min       = Vector3f(-1000.0f, 0.0f, -1000.0f);
  Vector3f           m_max       = Vector3f(1000.0f, 100.0f, 1000.0f);
  std::vector<float> m_heightData;
};

}