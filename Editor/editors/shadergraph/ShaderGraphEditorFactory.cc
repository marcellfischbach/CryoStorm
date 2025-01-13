
#include <editors/shadergraph/ShaderGraphEditorFactory.hh>
#include <editors/shadergraph/ShaderGraphEditorWidget.hh>

#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csAssetLocator.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>

using namespace cs;


bool ShaderGraphEditorFactory::CanEdit(const cs::csAssetLocator &locator) const
{
  return locator.GetExtension() == "SHADERGRAPH";
}

void ShaderGraphEditorFactory::Edit(const cs::csAssetLocator &locator, QWidget *parent) const
{
  csShaderGraph* shaderGraph = csAssetManager::Get()->Load<csShaderGraph>(locator);
  if (!shaderGraph)
  {
    return;
  }




  ShaderGraphEditorWidget *dlg = new ShaderGraphEditorWidget (shaderGraph, locator, parent);
  dlg->show();
}
