//
// Created by Marcell on 28.01.2025.
//

#include <editors/shadergraph/ShaderGraphEditorNodePropertiesWidget.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpacerItem>


ShaderGraphEditorNodePropertiesWidget::ShaderGraphEditorNodePropertiesWidget(QWidget *parent)
    : QWidget(parent)
{
  m_layout = new QGridLayout(this);
  m_spacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

  m_lineEditId = new QLineEdit("", this);
  m_lineEditId->setEnabled(false);
  m_layout->addWidget(m_lineEditId, 0, 0, 1, 13);


  m_resourceWidgets.labelName = nullptr;
  m_resourceWidgets.labelValues = nullptr;
  m_resourceWidgets.lineEditName = nullptr;
  memset(m_resourceWidgets.valueFloat, 0, sizeof(QDoubleSpinBox *) * 16);
  memset(m_resourceWidgets.valueInt, 0, sizeof(QSpinBox *) * 4);
  m_resourceWidgets.valueLocator = nullptr;

  m_shaderGraphWidget.receiveShadow = nullptr;
  m_shaderGraphWidget.alphaThresholdLabel = nullptr;
  m_shaderGraphWidget.alphaThreshold = nullptr;
  m_shaderGraphWidget.alphaDiscardLabel = nullptr;
  m_shaderGraphWidget.alphaDiscard = nullptr;
  m_shaderGraphWidget.blendingModeLabel = nullptr;
  m_shaderGraphWidget.blendingMode = nullptr;
  m_shaderGraphWidget.lightingModeLabel = nullptr;
  m_shaderGraphWidget.lightingMode = nullptr;
  m_shaderGraphWidget.separator = nullptr;

}

ShaderGraphEditorNodePropertiesWidget::~ShaderGraphEditorNodePropertiesWidget()
{

}


void ShaderGraphEditorNodePropertiesWidget::SetShaderGraph(cs::csShaderGraph *shaderGraph)
{
  m_shaderGraph = shaderGraph;
}


void ShaderGraphEditorNodePropertiesWidget::SetNode(cs::csSGNode *node)
{
  m_node = node;
  RemoveAll();

  int row = 1;
  if (node == m_shaderGraph->Root())
  {
    row += CreateShaderGraphProperties(row);
  }
  row += CreateWidgetsForResource(row);
  row += CreateWidgetsForInputs(row);


  m_layout->addItem(m_spacer, row, 0);

  m_lineEditId->setText(node->GetKey().c_str());
}

void ShaderGraphEditorNodePropertiesWidget::UpdateState()
{
  for (auto &input: m_inputWidgets)
  {
    input.value->setEnabled(!input.input->GetSource());
  }
}


void ShaderGraphEditorNodePropertiesWidget::WriteValuesToNode()
{
  bool resourceNameChanged = false;
  for (auto &input: m_inputWidgets)
  {
    input.input->SetScalar(input.value->value());
  }

  if (m_node->IsInstanceOf<csSGResourceNode>())
  {
    csSGResourceNode *resource = m_node->Query<csSGResourceNode>();

    if (m_resourceWidgets.lineEditName)
    {
      std::string resourceName = m_resourceWidgets.lineEditName->text().toStdString();
      if (resource->GetResourceName() != resourceName)
      {
        resource->SetResourceName(m_resourceWidgets.lineEditName->text().toStdString());
        resourceNameChanged = true;
      }
    }

    std::array<float, 16> floats = resource->GetDefaultFloats();
    for (size_t i = 0; i < 16; i++)
    {
      if (m_resourceWidgets.valueFloat[i])
      {
        floats[i] = m_resourceWidgets.valueFloat[i]->value();
      }
    }
    resource->SetDefault(floats);

    std::array<int, 4> ints = resource->GetDefaultInts();
    for (size_t i = 0; i < 4; i++)
    {
      if (m_resourceWidgets.valueInt[i])
      {
        ints[i] = m_resourceWidgets.valueInt[i]->value();
      }
    }
    resource->SetDefault(ints);
    if (m_resourceWidgets.valueLocator)
    {
      resource->SetDefault(csAssetLocator(m_resourceWidgets.valueLocator->text().toStdString()));
    }
  }
  if (m_node == m_shaderGraph->Root())
  {
    if (m_shaderGraphWidget.receiveShadow)
    {
      m_shaderGraph->SetReceiveShadow(m_shaderGraphWidget.receiveShadow->isChecked());
    }
    if (m_shaderGraphWidget.lightingMode)
    {
      m_shaderGraph->SetLightingMode((csShaderGraph::eLightingMode) m_shaderGraphWidget.lightingMode->currentIndex());
    }
    if (m_shaderGraphWidget.blendingMode)
    {
      m_shaderGraph->SetBlendingMode((csShaderGraph::eBlendingMode) m_shaderGraphWidget.blendingMode->currentIndex());
    }
    if (m_shaderGraphWidget.alphaThreshold && m_shaderGraphWidget.alphaDiscard)
    {
      m_shaderGraph->SetAlphaDiscard((float) m_shaderGraphWidget.alphaThreshold->value(),
                                     (eCompareFunc) m_shaderGraphWidget.alphaDiscard->currentIndex());
    }
    printf("ShaderGraph RS[%d] L[%d] B[%d] A[%d:%f]\n",
           m_shaderGraph->IsReceiveShadow(),
           m_shaderGraph->GetLightingMode(),
           m_shaderGraph->GetBlendingMode(),
           m_shaderGraph->GetAlphaDiscard_Func(),
           m_shaderGraph->GetAlphaDiscard_Threshold()
    );
    fflush(stdout);
  }

  if (resourceNameChanged)
  {
    emit ResourceNameChanged();
  }
}


void ShaderGraphEditorNodePropertiesWidget::RemoveAll()
{
#define REMOVE_AND_DELETE(v) \
  if (v)\
  {                          \
    m_layout->removeWidget(v); \
    v->deleteLater();        \
    v = nullptr;             \
  }

#define REMOVE_DELETE_AND_UNBIND(v) \
  if (v)\
  {                          \
    m_layout->removeWidget(v);      \
    Unbind(v);\
    v->deleteLater();        \
    v = nullptr;             \
  }

  for (auto &item: m_inputWidgets)
  {
    REMOVE_AND_DELETE(item.label);
    REMOVE_DELETE_AND_UNBIND(item.value);
  }
  m_inputWidgets.clear();

  REMOVE_AND_DELETE(m_resourceWidgets.labelName);
  REMOVE_AND_DELETE(m_resourceWidgets.labelValues);
  REMOVE_DELETE_AND_UNBIND(m_resourceWidgets.lineEditName);


  for (size_t i = 0; i < 16; i++)
  {
    REMOVE_DELETE_AND_UNBIND(m_resourceWidgets.valueFloat[i])
  }
  for (size_t i = 0; i < 4; i++)
  {
    REMOVE_DELETE_AND_UNBIND(m_resourceWidgets.valueInt[i])
  }
  REMOVE_DELETE_AND_UNBIND(m_resourceWidgets.valueLocator);


  REMOVE_AND_DELETE(m_shaderGraphWidget.alphaDiscardLabel);
  REMOVE_AND_DELETE(m_shaderGraphWidget.alphaThresholdLabel);
  REMOVE_AND_DELETE(m_shaderGraphWidget.blendingModeLabel);
  REMOVE_AND_DELETE(m_shaderGraphWidget.lightingModeLabel);
  REMOVE_AND_DELETE(m_shaderGraphWidget.separator);
  REMOVE_DELETE_AND_UNBIND(m_shaderGraphWidget.receiveShadow);
  REMOVE_DELETE_AND_UNBIND(m_shaderGraphWidget.alphaDiscard);
  REMOVE_DELETE_AND_UNBIND(m_shaderGraphWidget.alphaThreshold);
  REMOVE_DELETE_AND_UNBIND(m_shaderGraphWidget.blendingMode);
  REMOVE_DELETE_AND_UNBIND(m_shaderGraphWidget.lightingMode);


#undef REMOVE_AND_DELETE
#undef REMOVE_DELETE_AND_UNBIND


  m_layout->removeItem(m_spacer);
}

int ShaderGraphEditorNodePropertiesWidget::CreateShaderGraphProperties(int rowStart)
{
  m_shaderGraphWidget.blendingModeLabel = new QLabel(tr("Blending"), this);
  m_shaderGraphWidget.lightingModeLabel = new QLabel(tr("Lighting"), this);
  m_shaderGraphWidget.alphaDiscardLabel = new QLabel(tr("Alpha Discard"), this);
  m_shaderGraphWidget.alphaThresholdLabel = new QLabel(tr("Alpha threshold"), this);

  m_shaderGraphWidget.separator = new QFrame(this);
  m_shaderGraphWidget.separator->setFrameShape(QFrame::HLine);
  m_shaderGraphWidget.separator->setFrameShadow(QFrame::Sunken);
  m_shaderGraphWidget.separator->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

  m_shaderGraphWidget.blendingModeLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  m_shaderGraphWidget.lightingModeLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  m_shaderGraphWidget.alphaDiscardLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  m_shaderGraphWidget.alphaThresholdLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);


  m_shaderGraphWidget.receiveShadow = new QCheckBox(tr("Receive shadow"), this);
  m_shaderGraphWidget.blendingMode = new QComboBox(this);
  m_shaderGraphWidget.lightingMode = new QComboBox(this);
  m_shaderGraphWidget.alphaDiscard = new QComboBox(this);
  m_shaderGraphWidget.alphaThreshold = new QDoubleSpinBox(this);


  m_shaderGraphWidget.blendingMode->addItem(tr("Off"));
  m_shaderGraphWidget.blendingMode->addItem(tr("Alpha"));
  m_shaderGraphWidget.blendingMode->addItem(tr("Add"));
  m_shaderGraphWidget.blendingMode->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

  m_shaderGraphWidget.lightingMode->addItem(tr("Default"));
  m_shaderGraphWidget.lightingMode->addItem(tr("Attenuated"));
  m_shaderGraphWidget.lightingMode->addItem(tr("Unlit"));
  m_shaderGraphWidget.lightingMode->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

  m_shaderGraphWidget.alphaDiscard->addItem(tr("Less"));
  m_shaderGraphWidget.alphaDiscard->addItem(tr("LessOrEqual"));
  m_shaderGraphWidget.alphaDiscard->addItem(tr("Greater"));
  m_shaderGraphWidget.alphaDiscard->addItem(tr("GreaterOrEqual"));
  m_shaderGraphWidget.alphaDiscard->addItem(tr("Equal"));
  m_shaderGraphWidget.alphaDiscard->addItem(tr("NotEqual"));
  m_shaderGraphWidget.alphaDiscard->addItem(tr("Always"));
  m_shaderGraphWidget.alphaDiscard->addItem(tr("Never"));
  m_shaderGraphWidget.alphaDiscard->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

  m_shaderGraphWidget.alphaThreshold->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_shaderGraphWidget.alphaThreshold->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);


  m_shaderGraphWidget.receiveShadow->setChecked(m_shaderGraph->IsReceiveShadow());
  m_shaderGraphWidget.lightingMode->setCurrentIndex(m_shaderGraph->GetLightingMode());
  m_shaderGraphWidget.blendingMode->setCurrentIndex(m_shaderGraph->GetBlendingMode());
  m_shaderGraphWidget.alphaDiscard->setCurrentIndex(m_shaderGraph->GetAlphaDiscard_Func());
  m_shaderGraphWidget.alphaThreshold->setValue(m_shaderGraph->GetAlphaDiscard_Threshold());

  Bind(m_shaderGraphWidget.receiveShadow);
  Bind(m_shaderGraphWidget.lightingMode);
  Bind(m_shaderGraphWidget.blendingMode);
  Bind(m_shaderGraphWidget.alphaDiscard);
  Bind(m_shaderGraphWidget.alphaThreshold);

  int i = 0;
  m_layout->addWidget(m_shaderGraphWidget.receiveShadow, rowStart + i, 0, 1, 13);
  i++;
  m_layout->addWidget(m_shaderGraphWidget.blendingModeLabel, rowStart + i, 0, 1, 1);
  m_layout->addWidget(m_shaderGraphWidget.blendingMode, rowStart + i, 1, 1, 12);
  i++;
  m_layout->addWidget(m_shaderGraphWidget.lightingModeLabel, rowStart + i, 0, 1, 1);
  m_layout->addWidget(m_shaderGraphWidget.lightingMode, rowStart + i, 1, 1, 12);
  i++;

  m_layout->addWidget(m_shaderGraphWidget.alphaDiscardLabel, rowStart + i, 0, 1, 1);
  m_layout->addWidget(m_shaderGraphWidget.alphaDiscard, rowStart + i, 1, 1, 12);
  i++;

  m_layout->addWidget(m_shaderGraphWidget.alphaThresholdLabel, rowStart + i, 0, 1, 1);
  m_layout->addWidget(m_shaderGraphWidget.alphaThreshold, rowStart + i, 1, 1, 12);
  i++;
  m_layout->addWidget(m_shaderGraphWidget.separator, rowStart + i, 0, 1, 13);
  i++;
  return i;
}

int ShaderGraphEditorNodePropertiesWidget::CreateWidgetsForInputs(int rowStart)
{
  for (int i = 0; i < m_node->GetNumberOfInputs(); ++i)
  {
    csSGNodeInput *input = m_node->GetInput(i);
    if (input->IsNoValue())
    {
      continue;
    }
    QLabel *label = new QLabel(input->GetName().c_str(), this);
    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_layout->addWidget(label, rowStart + i, 0, 1, 1);


    QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
    spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    m_layout->addWidget(spinBox, rowStart + i, 1, 1, 12);

    if (input->GetSource())
    {
      spinBox->setEnabled(false);
    }

    spinBox->setValue(input->GetScalar());
    m_inputWidgets.emplace_back(input, label, spinBox);

    Bind(spinBox);
  }

  return m_node->GetNumberOfInputs();
}

int ShaderGraphEditorNodePropertiesWidget::CreateWidgetsForResource(int rowStart)
{
  if (!m_node->IsInstanceOf<csSGResourceNode>())
  {
    return 0;
  }

  csSGResourceNode *resource = m_node->Query<csSGResourceNode>();

  m_resourceWidgets.labelName = new QLabel("Resource name", this);
  m_resourceWidgets.labelValues = new QLabel("Default", this);
  m_resourceWidgets.lineEditName = new QLineEdit(resource->GetResourceName().c_str(), this);

  m_resourceWidgets.labelName->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  m_resourceWidgets.labelValues->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  m_resourceWidgets.lineEditName->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

  m_layout->addWidget(m_resourceWidgets.labelName, rowStart, 0, 1, 1);
  m_layout->addWidget(m_resourceWidgets.lineEditName, rowStart, 1, 1, 12);
  m_layout->addWidget(m_resourceWidgets.labelValues, rowStart + 1, 0, 1, 1);


  Bind(m_resourceWidgets.lineEditName);


  int numValueRows = 1;
  switch (resource->GetMatType())
  {
    case cs::eMAT_Float:
      CreateFloats(rowStart + 1, 0, 1);
      break;

    case cs::eMAT_Vec2:
      CreateFloats(rowStart + 1, 0, 2);
      break;

    case cs::eMAT_Vec3:
      CreateFloats(rowStart + 1, 0, 3);
      break;

    case cs::eMAT_Vec4:
      CreateFloats(rowStart + 1, 0, 4);
      break;

    case cs::eMAT_Int:
      CreateInts(rowStart + 1, 0, 1);
      break;

    case cs::eMAT_IVec2:
      CreateInts(rowStart + 1, 0, 2);
      break;

    case cs::eMAT_IVec3:
      CreateInts(rowStart + 1, 0, 3);
      break;

    case cs::eMAT_IVec4:
      CreateInts(rowStart + 1, 0, 4);
      break;

    case cs::eMAT_Matrix3:
      CreateFloats(rowStart + 1, 0, 3);
      CreateFloats(rowStart + 2, 3, 3);
      CreateFloats(rowStart + 3, 6, 3);
      numValueRows = 3;
      break;
    case cs::eMAT_Matrix4:
      CreateFloats(rowStart + 1, 0, 3);
      CreateFloats(rowStart + 2, 3, 3);
      CreateFloats(rowStart + 3, 6, 3);
      numValueRows = 4;
      break;

    case cs::eMAT_Texture:
      CreateLocator(rowStart + 1);
      break;
    default:
      break;

  }


  return numValueRows + 1;
}


void ShaderGraphEditorNodePropertiesWidget::CreateFloats(size_t row, size_t idx, size_t count)
{
  size_t columnCount = 12 / count;

  csSGResourceNode *resource = m_node->Query<csSGResourceNode>();
  const std::array<float, 16> &defaultFloats = resource->GetDefaultFloats();

  for (size_t i = 0; i < count; i++)
  {
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
    spinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    m_layout->addWidget(spinBox, row, i * columnCount + 1, 1, columnCount);
    m_resourceWidgets.valueFloat[idx + i] = spinBox;

    spinBox->setValue(defaultFloats[idx + i]);
    Bind(spinBox);
  }
}

void ShaderGraphEditorNodePropertiesWidget::CreateInts(size_t row, size_t idx, size_t count)
{
  size_t columnCount = 12 / count;

  csSGResourceNode *resource = m_node->Query<csSGResourceNode>();
  const std::array<int, 4> &defaultInts = resource->GetDefaultInts();

  for (size_t i = 0; i < count; i++)
  {
    QSpinBox *spinBox = new QSpinBox(this);
    spinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    m_layout->addWidget(spinBox, row, i * columnCount + 1, 1, columnCount);
    m_resourceWidgets.valueInt[idx + i] = spinBox;
    spinBox->setValue(defaultInts[idx + i]);
    Bind(spinBox);
  }
}

void ShaderGraphEditorNodePropertiesWidget::CreateLocator(size_t row)
{
  csSGResourceNode *resource = m_node->Query<csSGResourceNode>();

  QLineEdit *lineEdit = new QLineEdit(this);
  lineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
  m_layout->addWidget(lineEdit, row, 1, 1, 12);

  m_resourceWidgets.valueLocator = lineEdit;
  lineEdit->setText(resource->GetDefaultLocator().Encoded().c_str());

  Bind(lineEdit);
}


void ShaderGraphEditorNodePropertiesWidget::Bind(QCheckBox *checkBox)
{
  connect(checkBox, &QCheckBox::checkStateChanged, this, &ShaderGraphEditorNodePropertiesWidget::OnCheckStateChanged);
}

void ShaderGraphEditorNodePropertiesWidget::Unbind(QCheckBox *checkBox)
{
  disconnect(checkBox,
             &QCheckBox::checkStateChanged,
             this,
             &ShaderGraphEditorNodePropertiesWidget::OnCheckStateChanged);
}

void ShaderGraphEditorNodePropertiesWidget::Bind(QComboBox *comboBox)
{
  connect(comboBox,
          &QComboBox::currentIndexChanged,
          this,
          &ShaderGraphEditorNodePropertiesWidget::OnComboBoxChanged);
}

void ShaderGraphEditorNodePropertiesWidget::Unbind(QComboBox *comboBox)
{
  disconnect(comboBox,
             &QComboBox::currentIndexChanged,
             this,
             &ShaderGraphEditorNodePropertiesWidget::OnComboBoxChanged);
}

void ShaderGraphEditorNodePropertiesWidget::Bind(QLineEdit *lineEdit)
{
  connect(lineEdit, &QLineEdit::textEdited, this, &ShaderGraphEditorNodePropertiesWidget::OnLineEditTextChanged);
}

void ShaderGraphEditorNodePropertiesWidget::Unbind(QLineEdit *lineEdit)
{
  disconnect(lineEdit, &QLineEdit::textEdited, this, &ShaderGraphEditorNodePropertiesWidget::OnLineEditTextChanged);
}

void ShaderGraphEditorNodePropertiesWidget::Bind(QSpinBox *spinBox)
{
  connect(spinBox, &QSpinBox::valueChanged, this, &ShaderGraphEditorNodePropertiesWidget::OnSpinBoxValueChanged);
}

void ShaderGraphEditorNodePropertiesWidget::Unbind(QSpinBox *spinBox)
{
  disconnect(spinBox, &QSpinBox::valueChanged, this, &ShaderGraphEditorNodePropertiesWidget::OnSpinBoxValueChanged);
}

void ShaderGraphEditorNodePropertiesWidget::Bind(QDoubleSpinBox *spinBox)
{
  connect(spinBox,
          &QDoubleSpinBox::valueChanged,
          this,
          &ShaderGraphEditorNodePropertiesWidget::OnDoubleSpinBoxValueChanged);

}

void ShaderGraphEditorNodePropertiesWidget::Unbind(QDoubleSpinBox *spinBox)
{
  disconnect(spinBox,
             &QDoubleSpinBox::valueChanged,
             this,
             &ShaderGraphEditorNodePropertiesWidget::OnDoubleSpinBoxValueChanged);

}

void ShaderGraphEditorNodePropertiesWidget::OnCheckStateChanged(Qt::CheckState state)
{
  WriteValuesToNode();
}

void ShaderGraphEditorNodePropertiesWidget::OnComboBoxChanged(int index)
{
  WriteValuesToNode();
}

void ShaderGraphEditorNodePropertiesWidget::OnLineEditTextChanged(const QString &text)
{
  WriteValuesToNode();
}

void ShaderGraphEditorNodePropertiesWidget::OnSpinBoxValueChanged(int value)
{
  WriteValuesToNode();
}

void ShaderGraphEditorNodePropertiesWidget::OnDoubleSpinBoxValueChanged(double value)
{
  WriteValuesToNode();
}