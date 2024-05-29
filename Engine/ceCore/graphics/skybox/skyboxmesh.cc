//
// Created by mcel on 28.05.2024.
//

#include <ceCore/graphics/skybox/skyboxmesh.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/itexturecube.hh>
#include <ceCore/graphics/irendermesh.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/resource/assetmanager.hh>

namespace ce
{

void SkyboxMesh::Render(iDevice *device, float size, iTextureCube *texture, iTexture2D *depth)
{
  if (!texture)
  {
    return;
  }

  iRenderMesh *renderMesh = RenderMesh(device);
  iShader     *shader     = Shader(device);
  if (renderMesh && shader)
  {
    device->SetShader(shader);
    device->ResetTextures();

    if (m_attrRenderPlane)
    {
      m_attrRenderPlane->Bind(size);
    }

    if (m_attrSkybox && texture)
    {
      eTextureUnit unit = device->BindTexture(texture);
      m_attrSkybox->Bind(unit);
    }
    if (m_attrDepth && depth)
    {
      eTextureUnit unit = device->BindTexture(depth);
      m_attrDepth->Bind(unit);
    }
    device->Render(m_renderMesh, eRenderPass::eRP_Forward);
  }
}

iRenderMesh *SkyboxMesh::RenderMesh(ce::iDevice *device)
{
  if (!m_renderMesh)
  {
    iRenderMeshGenerator *generator = ObjectRegistry::Get<iRenderMeshGeneratorFactory>()->Create();

    std::vector<Vector4f> vertices;
    std::vector<Vector3f> uvs;
    std::vector<uint32_t> indices;

    vertices.push_back(Vector4f(-1.0f, -1.0f, -1.0f, 1.0f));
    vertices.push_back(Vector4f(-1.0f, -1.0f, 1.0f, 1.0f));
    vertices.push_back(Vector4f(-1.0f, 1.0f, -1.0f, 1.0f));
    vertices.push_back(Vector4f(-1.0f, 1.0f, 1.0f, 1.0f));
    vertices.push_back(Vector4f(1.0f, -1.0f, -1.0f, 1.0f));
    vertices.push_back(Vector4f(1.0f, -1.0f, 1.0f, 1.0f));
    vertices.push_back(Vector4f(1.0f, 1.0f, -1.0f, 1.0f));
    vertices.push_back(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

    uvs.push_back(Vector3f(-1.0f, -1.0f, -1.0f));
    uvs.push_back(Vector3f(-1.0f, -1.0f, 1.0f));
    uvs.push_back(Vector3f(-1.0f, 1.0f, -1.0f));
    uvs.push_back(Vector3f(-1.0f, 1.0f, 1.0f));
    uvs.push_back(Vector3f(1.0f, -1.0f, -1.0f));
    uvs.push_back(Vector3f(1.0f, -1.0f, 1.0f));
    uvs.push_back(Vector3f(1.0f, 1.0f, -1.0f));
    uvs.push_back(Vector3f(1.0f, 1.0f, 1.0f));

    // front
    indices.push_back(0);
    indices.push_back(6);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(6);

    // back
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(7);
    indices.push_back(1);
    indices.push_back(7);
    indices.push_back(5);

    // left
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(1);

    // right
    indices.push_back(5);
    indices.push_back(7);
    indices.push_back(6);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(4);

    // bottom
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(0);
    indices.push_back(5);
    indices.push_back(4);

    // top
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(6);
    indices.push_back(3);
    indices.push_back(6);
    indices.push_back(7);
    generator->SetVertices(vertices);
    generator->SetUV0(uvs);
    generator->SetIndices(indices);
    m_renderMesh = generator->Generate();
  }
  return m_renderMesh;
}

iShader *SkyboxMesh::Shader(ce::iDevice *device)
{
  if (!m_shader)
  {
    m_shader = AssetManager::Get()->Get<iShader>("${shaders}/skybox/skybox_on_screen.shader");
    if (!m_shader)
    {
      return nullptr;
    }
  }

  m_attrRenderPlane = m_shader->GetShaderAttribute("RenderPlane");
  m_attrSkybox      = m_shader->GetShaderAttribute("Skybox");
  m_attrDepth       = m_shader->GetShaderAttribute("Depth");
  return m_shader;
}

} // ce