//
// Created by Marcell on 28.01.2025.
//

#include <editors/shadergraph/ShaderGraphEditorNodePropertiesWidget.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>

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
  for (auto &input: m_inputWidgets)
  {
    input.input->SetScalar(input.value->value());
  }
  
  if (m_node->IsInstanceOf<csSGResourceNode>())
  {
    csSGResourceNode* resource = m_node->Query<csSGResourceNode>();

    std::array<float, 16> floats = resource->GetDefaultFloats();
    for (size_t i=0; i<16; i++)
    {
      if (m_resourceWidgets.valueFloat[i])
      {
        floats[i] = m_resourceWidgets.valueFloat[i]->value();
      }
    }
    resource->SetDefault(floats);

    std::array<int, 4> ints = resource->GetDefaultInts();
    for (size_t i=0; i<4; i++)
    {
      if (m_resourceWidgets.valueInt[i])
      {
        ints[i] = m_resourceWidgets.valueInt[i]->value();
      }
    }
    resource->SetDefault(ints);
    if (m_resourceWidgets.lineEditName)
    {
      resource->SetDefault(csAssetLocator(m_resourceWidgets.lineEditName->text().toStdString()));
    }
  }
}


void ShaderGraphEditorNodePropertiesWidget::RemoveAll()
{
  for (const auto &item: m_inputWidgets)
  {
    if (item.label)
    {
      m_layout->removeWidget(item.label);
      item.label->deleteLater();
    }
    if (item.value)
    {
      m_layout->removeWidget(item.value);
      Unbind(item.value);
      item.value->deleteLater();
    }
  }
  m_inputWidgets.clear();

  if (m_resourceWidgets.labelName)
  {
    m_layout->removeWidget(m_resourceWidgets.labelName);
    m_resourceWidgets.labelName->deleteLater();
    m_resourceWidgets.labelName = nullptr;
  }

  if (m_resourceWidgets.labelValues)
  {
    m_layout->removeWidget(m_resourceWidgets.labelValues);
    m_resourceWidgets.labelValues->deleteLater();
    m_resourceWidgets.labelValues = nullptr;
  }
  if (m_resourceWidgets.lineEditName)
  {
    m_layout->removeWidget(m_resourceWidgets.lineEditName);
    Unbind(m_resourceWidgets.lineEditName);
    m_resourceWidgets.lineEditName->deleteLater();
    m_resourceWidgets.lineEditName = nullptr;
  }

  for (size_t i = 0; i < 16; i++)
  {
    if (m_resourceWidgets.valueFloat[i])
    {
      m_layout->removeWidget(m_resourceWidgets.valueFloat[i]);
      Unbind(m_resourceWidgets.valueFloat[i]);
      m_resourceWidgets.valueFloat[i]->deleteLater();
      m_resourceWidgets.valueFloat[i] = nullptr;
    }
  }
  for (size_t i = 0; i < 4; i++)
  {
    if (m_resourceWidgets.valueInt[i])
    {
      m_layout->removeWidget(m_resourceWidgets.valueInt[i]);
      Unbind(m_resourceWidgets.valueInt[i]);
      m_resourceWidgets.valueInt[i]->deleteLater();
      m_resourceWidgets.valueInt[i] = nullptr;
    }
  }
  if (m_resourceWidgets.valueLocator)
  {
    m_layout->removeWidget(m_resourceWidgets.valueLocator);
    Unbind(m_resourceWidgets.valueLocator);
    m_resourceWidgets.valueLocator->deleteLater();
    m_resourceWidgets.valueLocator = nullptr;
  }


  m_layout->removeItem(m_spacer);
}

int ShaderGraphEditorNodePropertiesWidget::CreateWidgetsForInputs(int rowStart)
{
  for (int i = 0; i < m_node->GetNumberOfInputs(); ++i)
  {
    csSGNodeInput *input = m_node->GetInput(i);
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