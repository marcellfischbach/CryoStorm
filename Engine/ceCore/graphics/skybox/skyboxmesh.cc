//
// Created by mcel on 28.05.2024.
//

#include <ceCore/graphics/skybox/skyboxmesh.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/itexturecube.hh>
#include <ceCore/graphics/irendermesh.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/resource/assetmanager.hh>

namespace ce
{

void SkyboxMesh::Render(ce::iDevice *device, iTextureCube *texture)
{
  if (!texture)
  {
    return;
  }

  iRenderMesh *renderMesh = RenderMesh(device);
  iShader *shader = Shader(device);
  if (renderMesh && shader)
  {
    device->SetShader(shader);
    device->ResetTextures();
//    eTextureUnit unit = device->BindTexture(texture);
//    m_attrSkybox->Bind(unit);
    renderMesh->Render(device, eRenderPass::eRP_Forward);
  }
}

iRenderMesh* SkyboxMesh::RenderMesh(ce::iDevice *device)
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

iShader* SkyboxMesh::Shader(ce::iDevice *device)
{
  if (!m_shader)
  {
    m_shader = AssetManager::Get()->Get<iShader>("${shaders}/skybox/skybox_on_screen.shader");
    if (!m_shader)
    {
      return nullptr;
    }
  }

//  if (!m_attrSkybox)
//  {
//    m_attrSkybox = m_shader->GetShaderAttribute("Skybox");
//    if (!m_attrSkybox)
//    {
//      return nullptr;
//    }
//  }
  return m_shader;
}

} // ce