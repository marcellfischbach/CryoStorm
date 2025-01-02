
#include <editors/shadergraph/ShaderGraphEditorFactory.hh>
#include <editors/shadergraph/ShaderGraphEditorWidget.hh>

#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csResourceLocator.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>

using namespace cs;


bool ShaderGraphEditorFactory::CanEdit(const cs::csResourceLocator &locator) const
{
  return locator.GetExtension() == "SHADERGRAPH";
}

void ShaderGraphEditorFactory::Edit(const cs::csResourceLocator &locator, QWidget *parent) const
{
  csShaderGraph* shaderGraph = csAssetManager::Get()->Load<csShaderGraph>(locator);
  if (!shaderGraph)
  {
    return;
  }




  ShaderGraphEditorWidget *dlg = new ShaderGraphEditorWidget (shaderGraph, locator, parent);
  dlg->show();
}
