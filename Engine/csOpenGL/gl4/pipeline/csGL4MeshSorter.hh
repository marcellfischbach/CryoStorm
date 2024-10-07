#pragma  once

#include <csCore/graphics/eRenderPass.hh>

namespace cs
{

class csGfxMesh;
namespace opengl
{

extern bool material_shader_compare_less(const csGfxMesh *mesh0, const csGfxMesh *mesh1, eRenderPass pass);
extern bool material_shader_compare_less_forward(const csGfxMesh *mesh0, const csGfxMesh *mesh1);
extern bool material_shader_compare_less_gbuffer(const csGfxMesh *mesh0, const csGfxMesh *mesh1);

}

}