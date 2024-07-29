
#include "gl4meshsorter.hh"
#include "ceCore/graphics/scene/gfxmesh.hh"
#include "ceCore/graphics/material/imaterial.hh"

namespace ce::opengl
{


bool material_shader_compare_less(const GfxMesh *mesh0, const GfxMesh *mesh1, eRenderPass pass)
{
  auto mat0 = mesh0->GetMaterial();
  auto mat1 = mesh1->GetMaterial();
  auto shader0 = mat0->GetShader(pass);
  auto shader1 = mat1->GetShader(pass);

  if (shader0 == shader1)
  {
    return mat0 < mat1;
  }

  return shader0 < shader1;
}

bool material_shader_compare_less_forward(const GfxMesh *mesh0, const GfxMesh *mesh1)
{
  return material_shader_compare_less(mesh0, mesh1, eRenderPass::eRP_Forward);
}

bool material_shader_compare_less_gbuffer(const GfxMesh *mesh0, const GfxMesh *mesh1)
{
  return material_shader_compare_less(mesh0, mesh1, eRenderPass::eRP_GBuffer);
}

}