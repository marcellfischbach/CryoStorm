
#include "csGL4MeshSorter.hh"
#include "ceCore/graphics/scene/csGfxMesh.hh"
#include "ceCore/graphics/material/iMaterial.hh"

namespace cryo::opengl
{


bool material_shader_compare_less(const csGfxMesh *mesh0, const csGfxMesh *mesh1, eRenderPass pass)
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

bool material_shader_compare_less_forward(const csGfxMesh *mesh0, const csGfxMesh *mesh1)
{
  return material_shader_compare_less(mesh0, mesh1, eRenderPass::eRP_Forward);
}

bool material_shader_compare_less_gbuffer(const csGfxMesh *mesh0, const csGfxMesh *mesh1)
{
  return material_shader_compare_less(mesh0, mesh1, eRenderPass::eRP_GBuffer);
}

}