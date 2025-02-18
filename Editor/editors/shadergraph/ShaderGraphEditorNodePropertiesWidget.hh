//
// Created by Marcell on 28.01.2025.
//

#pragma once

#include <QWidget>
#include <csCore/csRef.hh>
#include <vector>
#include <QCheckBox>

namespace cs
{
class csShaderGraph;
class csSGNode;
class csSGNodeInput;
}

using namespace cs;

class QComboBox;
class QDoubleSpinBox;
class QFrame;
class QGridLayout;
class QLabel;
class QLineEdit;
class QSpacerItem;
class QSpinBox;

class ShaderGraphEditorNodePropertiesWidget : public QWidget
{
Q_OBJECT
public:
  ShaderGraphEditorNodePropertiesWidget(QWidget *parent = nullptr);
  ~ShaderGraphEditorNodePropertiesWidget() override;

  void SetShaderGraph(csShaderGraph *shaderGraph);
  void SetNode(csSGNode *node);

  void UpdateState();

signals:
  void ResourceNameChanged ();

private:
  void RemoveAll();
  int CreateWidgetsForResource(int rowStart);
  int CreateWidgetsForInputs(int rowStart);
  int CreateShaderGraphProperties (int rowStart);

  void CreateFloats (size_t row, size_t idx, size_t count);
  void CreateInts (size_t row, size_t idx, size_t count);
  void CreateLocator (size_t row);

  void Bind (QCheckBox *checkBox);
  void Bind (QComboBox *comboBox);
  void Bind (QDoubleSpinBox *spineBox);
  void Bind (QSpinBox *spineBox);
  void Bind (QLineEdit *lineEdit);
  void Unbind (QCheckBox *checkBox);
  void Unbind (QComboBox *comboBox);
  void Unbind (QDoubleSpinBox *spineBox);
  void Unbind (QSpinBox *spineBox);
  void Unbind (QLineEdit *lineEdit);

  void WriteValuesToNode ();



private slots:
  void OnCheckStateChanged (Qt::CheckState state);
  void OnComboBoxChanged (int index);
  void OnLineEditTextChanged (const QString &text);
  void OnSpinBoxValueChanged (int value);
  void OnDoubleSpinBoxValueChanged (double value);

private:
  QGridLayout *m_layout;
  QSpacerItem *m_spacer;

  struct InputWidgets
  {
    csSGNodeInput *input;
    QLabel *label;
    QDoubleSpinBox *value;
  };


  struct ResourceWidgets
  {
    QLabel* labelName;
    QLabel* labelValues;

    QLineEdit *lineEditName;
    QDoubleSpinBox *valueFloat[16];
    QSpinBox *valueInt[4];
    QLineEdit *valueLocator;

  };

  struct ShaderGraphWidgets
  {
    QCheckBox* receiveShadow;

    QLabel* alphaDiscardLabel;
    QComboBox* alphaDiscard;
    QLabel* alphaThresholdLabel;
    QDoubleSpinBox *alphaThreshold;

    QLabel *blendingModeLabel;
    QComboBox* blendingMode;

    QLabel *lightingModeLabel;
    QComboBox* lightingMode;

    QFrame* separator;

  };

  QLineEdit *m_lineEditId;
  csRef<csShaderGraph> m_shaderGraph;
  csRef<csSGNode> m_node;


  ResourceWidgets m_resourceWidgets;
  ShaderGraphWidgets m_shaderGraphWidget;

  std::vector<InputWidgets> m_inputWidgets;

};
