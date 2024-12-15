//
// Created by Marcell on 08.12.2024.
//

#pragma once

#include <QDialog>
#include <vector>
#include <set>

QT_BEGIN_NAMESPACE
namespace Ui
{
class ShaderGraphEditorWidget;
}
QT_END_NAMESPACE

class QGraphicsScene;
class QGraphicsPathItem;
namespace cs
{
class csClass;
class csShaderGraph;
class csSGNode;
class csSGNodeInput;
class csSGNodeIO;
class csSGNodeOutput;
}

class ShaderGraphNodeItem;
class ShaderGraphNodePalletTreeModel;
class ShaderGraphEditorWidget : public QDialog
{
Q_OBJECT

public:
  explicit ShaderGraphEditorWidget(QWidget *parent = nullptr);
  ~ShaderGraphEditorWidget() override;



private:


  Ui::ShaderGraphEditorWidget *m_gui;

  cs::csShaderGraph *m_shaderGraph;
  ShaderGraphNodePalletTreeModel *m_palletModel;


};
