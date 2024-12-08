//
// Created by Marcell on 08.12.2024.
//

#include <editors/shadergraph/ShaderGraphEditorDialog.hh>
#include <editors/shadergraph/ShaderGraphNodeItem.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <QGraphicsScene>
#include "ui_ShaderGraphEditorWidget.h"

using namespace cs;

ShaderGraphEditorDialog::ShaderGraphEditorDialog(QWidget *parent)
    : QDialog(parent)
    , m_gui(new Ui::ShaderGraphEditorWidget)
    , m_shaderGraph(new csShaderGraph())
    , m_scene (new QGraphicsScene(this))
{
  m_gui->setupUi(this);
  connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));

  m_gui->graph->setScene(m_scene);

  new ShaderGraphNodeItem(m_shaderGraph, m_scene);

  QList<int> sizes;
  sizes << 100
  << width() - 200
  << 100;
  m_gui->mainSplitter->setSizes(sizes);

}

ShaderGraphEditorDialog::~ShaderGraphEditorDialog()
{
  CS_RELEASE(m_shaderGraph);
  delete m_gui;
}
