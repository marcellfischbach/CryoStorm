
#include <csEditor/editors/shadergraph/ShaderGraphEditorFactory.hh>
#include <csEditor/editors/shadergraph/ShaderGraphEditorWidget.hh>

#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csAssetLocator.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <QGridLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>

using namespace cs;


bool ShaderGraphEditorFactory::CanEdit(const cs::csAssetLocator &locator) const
{
  return locator.GetExtension() == "SHADERGRAPH";
}


EditorWidget * ShaderGraphEditorFactory::Create(const cs::csAssetLocator &locator) const
{
  auto shaderGraph = csAssetManager::Get()->Load<csShaderGraph>(locator);
  if (!shaderGraph)
  {
    return nullptr;
  }


  return new ShaderGraphEditorWidget(shaderGraph.Data(), locator);
}
