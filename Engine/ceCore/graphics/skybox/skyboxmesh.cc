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

namespace cryo
{

void SkyboxMesh::Render(iDevice *device, float size, iTextureCube *texture, iTexture2D *depth)
{
  if (!texture)
  {
    return;
  }

  if (depth)
  {
    RenderDeferred(device, size, texture, depth);
  }
  else
  {
    RenderForward(device, size, texture);
  }
}

void SkyboxMesh::RenderDeferred(cryo::iDevice *device, float size, cryo::iTextureCube *texture, cryo::iTexture2D *depth)
{
  iRenderMesh *renderMesh = RenderMesh(device);
  iShader     *shader     = ShaderDeferred(device);
  if (renderMesh && shader)
  {
    device->SetShader(shader);
    device->ResetTextures();

    if (m_attrDeferredRenderPlane)
    {
      m_attrDeferredRenderPlane->Bind(size);
    }

    if (m_attrDeferredSkybox && texture)
    {
      eTextureUnit unit = device->BindTexture(texture);
      m_attrDeferredSkybox->Bind(unit);
    }
    if (m_attrDeferredDepth)
    {
      eTextureUnit unit = device->BindTexture(depth);
      m_attrDeferredDepth->Bind(unit);
    }
    device->Render(m_renderMesh, eRenderPass::eRP_Forward);
  }
}

void SkyboxMesh::RenderForward(cryo::iDevice *device, float size, cryo::iTextureCube *texture)
{
  iRenderMesh *renderMesh = RenderMesh(device);
  iShader     *shader     = ShaderForward(device);
  if (renderMesh && shader)
  {
    device->SetShader(shader);
    device->ResetTextures();

    if (m_attrForwardRenderPlane)
    {
      m_attrForwardRenderPlane->Bind(size);
    }

    if (m_attrForwardSkybox && texture)
    {
      eTextureUnit unit = device->BindTexture(texture);
      m_attrForwardSkybox->Bind(unit);
    }
    device->Render(m_renderMesh, eRenderPass::eRP_Forward);
  }
}


iRenderMesh *SkyboxMesh::RenderMesh(cryo::iDevice *device)
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

iShader *SkyboxMesh::ShaderDeferred(iDevice *device)
{
  if (!m_shaderDeferred)
  {
    m_shaderDeferred = AssetManager::Get()->Get<iShader>("${shaders}/skybox/skybox_on_screen_deferred.shader");
    if (!m_shaderDeferred)
    {
      return nullptr;
    }
  }

  m_attrDeferredRenderPlane = m_shaderDeferred->GetShaderAttribute("RenderPlane");
  m_attrDeferredSkybox = m_shaderDeferred->GetShaderAttribute("Skybox");
  m_attrDeferredDepth = m_shaderDeferred->GetShaderAttribute("Depth");
  return m_shaderDeferred;
}


iShader *SkyboxMesh::ShaderForward(iDevice *device)
{
  if (!m_shaderForward)
  {
    m_shaderForward = AssetManager::Get()->Get<iShader>("${shaders}/skybox/skybox_on_screen_forward.shader");
    if (!m_shaderForward)
    {
      return nullptr;
    }
  }

  m_attrForwardRenderPlane = m_shaderForward->GetShaderAttribute("RenderPlane");
  m_attrForwardSkybox = m_shaderForward->GetShaderAttribute("Skybox");
  return m_shaderForward;
}

} // ce