//
// Created by Marcell on 21.12.2024.
//

#pragma once

#include <QWidget>

namespace Ui
{
class ShaderGraphNodeInputWidget;
}

namespace cs
{
class csSGNode;
class csSGNodeInput;
}

class QLabel;
class QSerparator;
class QDoubleSpinBox;
class QVBoxLayout;
class QSpacerItem;

class ShaderGraphNodeInputWidget : public QWidget
{
  Q_OBJECT;
public:
  ShaderGraphNodeInputWidget(QWidget *parent = nullptr);
  ~ShaderGraphNodeInputWidget() override;

  void SetInput (cs::csSGNodeInput *input);
  void UpdateState ();

private slots:
  void SyncInput(double);
private:

  Ui::ShaderGraphNodeInputWidget *m_gui;

  cs::csSGNodeInput *m_input;
};

class ShaderGraphNodePropertiesWidget : public QWidget
{
  Q_OBJECT;
public:
  ShaderGraphNodePropertiesWidget(QWidget* parent = nullptr);
  ~ShaderGraphNodePropertiesWidget() override;

  void SetNode (cs::csSGNode* node);

  void UpdateState ();

private:
  void RemoveAll ();


  cs::csSGNode* m_node;
  QVBoxLayout* m_vbox;

  std::vector<ShaderGraphNodeInputWidget*> m_inputs;
  QSpacerItem *m_spacer;



};
