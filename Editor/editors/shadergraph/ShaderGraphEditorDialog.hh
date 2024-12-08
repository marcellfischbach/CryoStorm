//
// Created by Marcell on 08.12.2024.
//

#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui
{
class ShaderGraphEditorWidget;
}
QT_END_NAMESPACE

class QGraphicsScene;

namespace cs
{
class csShaderGraph;
}

class ShaderGraphEditorDialog : public QDialog
{
Q_OBJECT

public:
  explicit ShaderGraphEditorDialog(QWidget *parent = nullptr);
  ~ShaderGraphEditorDialog() override;


private:
  Ui::ShaderGraphEditorWidget *m_gui;

  QGraphicsScene *m_scene;

  cs::csShaderGraph *m_shaderGraph;
};
