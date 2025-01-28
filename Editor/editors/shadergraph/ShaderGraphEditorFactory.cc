
#include <editors/shadergraph/ShaderGraphEditorFactory.hh>
#include <editors/shadergraph/ShaderGraphEditorWidget.hh>

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

MySpinBox::MySpinBox(QWidget *parent, QString styleSheet)
    : QDoubleSpinBox(parent)
    , m_styleSheet(styleSheet)
{
  setStyleSheet(styleSheet);
}

void MySpinBox::focusInEvent(QFocusEvent *evt)
{
  QDoubleSpinBox::focusInEvent(evt);
  setStyleSheet("");
  emit focusGained();
}


void MySpinBox::focusOutEvent(QFocusEvent *evt)
{
  QDoubleSpinBox::focusOutEvent(evt);
  setStyleSheet(m_styleSheet);
  emit focusLost();
}



void ShaderGraphEditorFactory::Edit(const cs::csAssetLocator &locator, QWidget *parent) const
{
  auto shaderGraph = csAssetManager::Get()->Load<csShaderGraph>(locator);
  if (!shaderGraph)
  {
    return;
  }


  ShaderGraphEditorWidget *dlg = new ShaderGraphEditorWidget (shaderGraph.Data(), locator, parent);
 dlg->show();
}
