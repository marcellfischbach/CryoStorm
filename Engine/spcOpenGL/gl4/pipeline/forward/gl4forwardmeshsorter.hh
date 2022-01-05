#pragma  once

#include <spcCore/graphics/erenderpass.hh>

namespace spc
{

class GfxMesh;
namespace opengl
{

extern bool material_shader_compare_less(const GfxMesh *mesh0, const GfxMesh *mesh1, eRenderPass pass);
extern bool material_shader_compare_less_forward(const GfxMesh *mesh0, const GfxMesh *mesh1);

}

}