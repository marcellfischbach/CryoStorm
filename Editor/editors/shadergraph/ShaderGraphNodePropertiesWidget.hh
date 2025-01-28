//
// Created by Marcell on 21.12.2024.
//

#pragma once

#include <QWidget>
#include <csCore/csRef.hh>

namespace Ui
{
class ShaderGraphNodeInputWidget;
class ShaderGraphResourceNodeWidget;
}

namespace cs
{
class csSGNode;
class csSGNodeInput;
class csSGResourceNode;
class csShaderGraph;
}

class QLabel;
class QLineEdit;
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

  cs::csRef<cs::csSGNodeInput> m_input;
};

class ShaderGraphResourceNodeWidget : public QWidget
{
  Q_OBJECT;
public:
  ShaderGraphResourceNodeWidget(QWidget* parent = nullptr);
  ~ShaderGraphResourceNodeWidget() override;

  void SetResource(cs::csSGResourceNode* resource);
  void UpdateValues();
  void SyncValues();

private slots:
  void SyncValuesSpinBox(int);
  void SyncValuesDoubleSpinBox(double);
  void SyncValuesLineEdit(const QString&);

private:
  Ui::ShaderGraphResourceNodeWidget* m_gui;
  cs::csRef<cs::csSGResourceNode> m_resource;
  bool m_changeGuard;
};

class ShaderGraphNodePropertiesWidget : public QWidget
{
  Q_OBJECT;
public:
  ShaderGraphNodePropertiesWidget(QWidget* parent = nullptr);
  ~ShaderGraphNodePropertiesWidget() override;

  void SetShaderGraph (cs::csShaderGraph* shaderGraph);
  void SetNode (cs::csSGNode* node);

  void UpdateState ();

private:
  void RemoveAll ();

private slots:
  void NodeNameChanged(const QString& name);

private:

  cs::csRef<cs::csShaderGraph> m_shaderGraph;
  cs::csRef<cs::csSGNode> m_node;
  QVBoxLayout* m_vbox;

  QLineEdit* m_nameLineEdit;
  ShaderGraphResourceNodeWidget* m_resourceWidget;
  std::vector<ShaderGraphNodeInputWidget*> m_inputs;
  QSpacerItem *m_spacer;



};
