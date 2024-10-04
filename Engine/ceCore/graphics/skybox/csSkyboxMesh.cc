//
// Created by mcel on 28.05.2024.
//

#include <ceCore/graphics/skybox/csSkyboxMesh.hh>
#include <ceCore/graphics/csCamera.hh>
#include <ceCore/graphics/csProjector.hh>
#include <ceCore/graphics/iDevice.hh>
#include <ceCore/graphics/iTextureCube.hh>
#include <ceCore/graphics/iRenderMesh.hh>
#include <ceCore/graphics/shading/iShader.hh>
#include <ceCore/graphics/shading/iShaderAttribute.hh>
#include <ceCore/csObjectRegistry.hh>
#include <ceCore/resource/csAssetManager.hh>

namespace cryo
{

void csSkyboxMesh::Render(iDevice *device, float size, iTextureCube *texture, iTexture2D *depth)
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

void csSkyboxMesh::RenderDeferred(cryo::iDevice *device, float size, cryo::iTextureCube *texture, cryo::iTexture2D *depth)
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

void csSkyboxMesh::RenderForward(cryo::iDevice *device, float size, cryo::iTextureCube *texture)
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


iRenderMesh *csSkyboxMesh::RenderMesh(cryo::iDevice *device)
{
  if (!m_renderMesh)
  {
    iRenderMeshGenerator *generator = csObjectRegistry::Get<iRenderMeshGeneratorFactory>()->Create();

    std::vector<csVector4f> vertices;
    std::vector<csVector3f> uvs;
    std::vector<uint32_t>   indices;

    vertices.push_back(csVector4f(-1.0f, -1.0f, -1.0f, 1.0f));
    vertices.push_back(csVector4f(-1.0f, -1.0f, 1.0f, 1.0f));
    vertices.push_back(csVector4f(-1.0f, 1.0f, -1.0f, 1.0f));
    vertices.push_back(csVector4f(-1.0f, 1.0f, 1.0f, 1.0f));
    vertices.push_back(csVector4f(1.0f, -1.0f, -1.0f, 1.0f));
    vertices.push_back(csVector4f(1.0f, -1.0f, 1.0f, 1.0f));
    vertices.push_back(csVector4f(1.0f, 1.0f, -1.0f, 1.0f));
    vertices.push_back(csVector4f(1.0f, 1.0f, 1.0f, 1.0f));

    uvs.push_back(csVector3f(-1.0f, -1.0f, -1.0f));
    uvs.push_back(csVector3f(-1.0f, -1.0f, 1.0f));
    uvs.push_back(csVector3f(-1.0f, 1.0f, -1.0f));
    uvs.push_back(csVector3f(-1.0f, 1.0f, 1.0f));
    uvs.push_back(csVector3f(1.0f, -1.0f, -1.0f));
    uvs.push_back(csVector3f(1.0f, -1.0f, 1.0f));
    uvs.push_back(csVector3f(1.0f, 1.0f, -1.0f));
    uvs.push_back(csVector3f(1.0f, 1.0f, 1.0f));

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

iShader *csSkyboxMesh::ShaderDeferred(iDevice *device)
{
  if (!m_shaderDeferred)
  {
    m_shaderDeferred = csAssetManager::Get()->Get<iShader>("${shaders}/skybox/skybox_on_screen_deferred.shader");
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


iShader *csSkyboxMesh::ShaderForward(iDevice *device)
{
  if (!m_shaderForward)
  {
    m_shaderForward = csAssetManager::Get()->Get<iShader>("${shaders}/skybox/skybox_on_screen_forward.shader");
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