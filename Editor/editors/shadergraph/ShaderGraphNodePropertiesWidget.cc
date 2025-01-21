//
// Created by Marcell on 21.12.2024.
//

#include <editors/shadergraph/ShaderGraphNodePropertiesWidget.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>
#include <csCore/graphics/shadergraph/csSGNodes.hh>
#include <ui_ShaderGraphNodeInputWidget.h>
#include <ui_ShaderGraphResourceNodeWidget.h>


ShaderGraphNodeInputWidget::ShaderGraphNodeInputWidget(QWidget* parent)
  : QWidget(parent)
  , m_gui(new Ui::ShaderGraphNodeInputWidget())
  , m_input(nullptr)
{
  m_gui->setupUi(this);

  connect(m_gui->valueF1X, SIGNAL(valueChanged(double)), this, SLOT(SyncInput(double)));
}

ShaderGraphNodeInputWidget::~ShaderGraphNodeInputWidget()
{
  delete m_gui;
}

void ShaderGraphNodeInputWidget::SetInput(cs::csSGNodeInput* input)
{
  m_input = input;
  UpdateState();
}

void ShaderGraphNodeInputWidget::UpdateState()
{
  if (m_input)
  {
    m_gui->typeName->setText(QString(m_input->GetName().c_str()));
    if (m_input->GetSource())
    {
      m_gui->type->setEnabled(false);
    }
    else
    {
      m_gui->type->setEnabled(true);
      m_gui->type->setCurrentIndex(0);
      m_gui->valueF1X->setValue(m_input->GetScalar());
    }
  }
}

void ShaderGraphNodeInputWidget::SyncInput(double)
{
  printf("Sync\n");
  if (m_input)
  {
    if (!m_input->GetSource())
    {
      m_input->SetScalar((float)m_gui->valueF1X->value());
    }
  }
}

ShaderGraphResourceNodeWidget::ShaderGraphResourceNodeWidget(QWidget* parent)
  : QWidget(parent)
  , m_gui(new Ui::ShaderGraphResourceNodeWidget())
  , m_resource(nullptr)
{
  m_gui->setupUi(this);

  connect(m_gui->matFloat0, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matVec2f0, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matVec2f1, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matVec3f0, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matVec3f1, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matVec3f2, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matVec4f0, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matVec4f1, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matVec4f2, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matVec4f3, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);

  connect(m_gui->matMat300, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat301, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat302, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat310, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat311, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat312, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat320, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat321, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat322, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);

  connect(m_gui->matMat400, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat401, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat402, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat403, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat410, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat411, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat412, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat413, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat420, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat421, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat422, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat423, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat430, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat431, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat432, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);
  connect(m_gui->matMat433, &QDoubleSpinBox::valueChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox);

  connect(m_gui->resourceName, &QLineEdit::textChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesLineEdit);
  connect(m_gui->matTextureLocator, &QLineEdit::textChanged, this, &ShaderGraphResourceNodeWidget::SyncValuesLineEdit);


}

ShaderGraphResourceNodeWidget::~ShaderGraphResourceNodeWidget()
{
  delete m_gui;
}

void ShaderGraphResourceNodeWidget::SetResource(cs::csSGResourceNode* resource)
{
  m_resource = resource;
  UpdateValues();
}

void ShaderGraphResourceNodeWidget::UpdateValues()
{
  if (!m_resource)
  {
    return;
  }

  m_changeGuard = true;
  m_gui->resourceName->setText(m_resource->GetResourceName().c_str());

  m_gui->mat->setCurrentIndex(m_resource->GetMatType());

  const std::array<float, 16> &floats = m_resource->GetDefaultFloats();
  const std::array<int, 4>& ints = m_resource->GetDefaultInts();



  switch (m_resource->GetMatType())
  {
  case cs::eMAT_Float:
    m_gui->matFloat0->setValue(floats[0]);
    break;
  case cs::eMAT_Vec2:
    m_gui->matVec2f0->setValue(floats[0]);
    m_gui->matVec2f1->setValue(floats[1]);
    break;
  case cs::eMAT_Vec3:
    m_gui->matVec3f0->setValue(floats[0]);
    m_gui->matVec3f1->setValue(floats[1]);
    m_gui->matVec3f2->setValue(floats[2]);
    break;
  case cs::eMAT_Vec4:
    m_gui->matVec4f0->setValue(floats[0]);
    m_gui->matVec4f1->setValue(floats[1]);
    m_gui->matVec4f2->setValue(floats[2]);
    m_gui->matVec4f3->setValue(floats[3]);
    break;
  case cs::eMAT_Int:
  case cs::eMAT_IVec2:
  case cs::eMAT_IVec3:
  case cs::eMAT_IVec4:
    break;
  case cs::eMAT_Matrix3:
    m_gui->matMat300->setValue(floats[0]);
    m_gui->matMat301->setValue(floats[1]);
    m_gui->matMat302->setValue(floats[2]);
    m_gui->matMat310->setValue(floats[3]);
    m_gui->matMat311->setValue(floats[4]);
    m_gui->matMat312->setValue(floats[5]);
    m_gui->matMat320->setValue(floats[6]);
    m_gui->matMat321->setValue(floats[7]);
    m_gui->matMat322->setValue(floats[8]);
    break;
  case cs::eMAT_Matrix4:
    m_gui->matMat400->setValue(floats[0]);
    m_gui->matMat401->setValue(floats[1]);
    m_gui->matMat402->setValue(floats[2]);
    m_gui->matMat403->setValue(floats[3]);
    m_gui->matMat410->setValue(floats[4]);
    m_gui->matMat411->setValue(floats[5]);
    m_gui->matMat412->setValue(floats[6]);
    m_gui->matMat413->setValue(floats[7]);
    m_gui->matMat420->setValue(floats[8]);
    m_gui->matMat421->setValue(floats[9]);
    m_gui->matMat422->setValue(floats[10]);
    m_gui->matMat423->setValue(floats[11]);
    m_gui->matMat430->setValue(floats[12]);
    m_gui->matMat431->setValue(floats[13]);
    m_gui->matMat432->setValue(floats[14]);
    m_gui->matMat433->setValue(floats[15]);
    break;
  case cs::eMAT_Texture:
    m_gui->matTextureLocator->setText(m_resource->GetDefaultLocator().Encoded().c_str());
    break;

  }
  m_changeGuard = false;

}


void ShaderGraphResourceNodeWidget::SyncValuesSpinBox(int)
{
  SyncValues();
}

void ShaderGraphResourceNodeWidget::SyncValuesDoubleSpinBox(double)
{
  SyncValues();
}

void ShaderGraphResourceNodeWidget::SyncValuesLineEdit(const QString &)
{
  SyncValues();
}

void ShaderGraphResourceNodeWidget::SyncValues()
{
  if (!m_resource || m_changeGuard)
  {
    return;
  }

  std::array<float, 16> floats;
  std::array<int, 4> ints;
  cs::csAssetLocator locator("");
  switch (m_resource->GetMatType())
  {
  case cs::eMAT_Float:
    floats[0] = m_gui->matFloat0->value();
    break;
  case cs::eMAT_Vec2:
    floats[0] = m_gui->matVec2f0->value();
    floats[1] = m_gui->matVec2f1->value();
    break;
  case cs::eMAT_Vec3:
    floats[0] = m_gui->matVec3f0->value();
    floats[1] = m_gui->matVec3f1->value();
    floats[2] = m_gui->matVec3f2->value();
    break;
  case cs::eMAT_Vec4:
    floats[0] = m_gui->matVec4f0->value();
    floats[1] = m_gui->matVec4f1->value();
    floats[2] = m_gui->matVec4f2->value();
    floats[3] = m_gui->matVec4f3->value();
    break;
  case cs::eMAT_Int:
  case cs::eMAT_IVec2:
  case cs::eMAT_IVec3:
  case cs::eMAT_IVec4:
    break;
  case cs::eMAT_Matrix3:
    floats[0] = m_gui->matMat300->value();
    floats[1] = m_gui->matMat301->value();
    floats[2] = m_gui->matMat302->value();
    floats[3] = m_gui->matMat310->value();
    floats[4] = m_gui->matMat311->value();
    floats[5] = m_gui->matMat312->value();
    floats[6] = m_gui->matMat320->value();
    floats[7] = m_gui->matMat321->value();
    floats[8] = m_gui->matMat322->value();
    break;
  case cs::eMAT_Matrix4:
    floats[0] = m_gui->matMat400->value();
    floats[1] = m_gui->matMat401->value();
    floats[2] = m_gui->matMat402->value();
    floats[3] = m_gui->matMat403->value();
    floats[4] = m_gui->matMat410->value();
    floats[5] = m_gui->matMat411->value();
    floats[6] = m_gui->matMat412->value();
    floats[7] = m_gui->matMat413->value();
    floats[8] = m_gui->matMat420->value();
    floats[9] = m_gui->matMat421->value();
    floats[10] = m_gui->matMat422->value();
    floats[11] = m_gui->matMat423->value();
    floats[12] = m_gui->matMat430->value();
    floats[13] = m_gui->matMat431->value();
    floats[14] = m_gui->matMat432->value();
    floats[15] = m_gui->matMat433->value();
    break;
  case cs::eMAT_Texture:
    locator = cs::csAssetLocator(m_gui->matTextureLocator->text().toStdString());
    break;
  }

  m_resource->SetResourceName(m_gui->resourceName->text().toStdString());
  m_resource->SetDefault(floats);
  m_resource->SetDefault(ints);
  m_resource->SetDefault(locator);

}


ShaderGraphNodePropertiesWidget::ShaderGraphNodePropertiesWidget(QWidget* parent)
  : QWidget(parent)
  , m_node(nullptr)
  , m_vbox(new QVBoxLayout(this))
  , m_spacer(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding))
  , m_resourceWidget(nullptr)
{
  QHBoxLayout* nameBox = new QHBoxLayout(this);
  QLabel* label = new QLabel("Name", this);
  label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
  nameBox->addWidget(label);

  m_nameLineEdit = new QLineEdit(this);
  m_nameLineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  nameBox->addWidget(m_nameLineEdit);

  m_vbox->addItem(nameBox);

  connect(m_nameLineEdit, &QLineEdit::textChanged, this, &ShaderGraphNodePropertiesWidget::NodeNameChanged);
}

ShaderGraphNodePropertiesWidget::~ShaderGraphNodePropertiesWidget()
{
  RemoveAll();
}

void ShaderGraphNodePropertiesWidget::SetNode(cs::csSGNode* node)
{
  m_node = nullptr;
  RemoveAll();


  m_nameLineEdit->setText(node->GetKey().c_str());
  if (node->IsInstanceOf<cs::csSGResourceNode>())
  {
    m_resourceWidget = new ShaderGraphResourceNodeWidget(this);
    m_resourceWidget->SetResource(node->Query<cs::csSGResourceNode>());
    m_resourceWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    m_vbox->addWidget(m_resourceWidget);
  }

  for (int i = 0; i < node->GetNumberOfInputs(); ++i)
  {
    cs::csSGNodeInput* sgInput = node->GetInput(i);
    ShaderGraphNodeInputWidget* input = new ShaderGraphNodeInputWidget(this);
    input->SetInput(sgInput);
    input->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    m_vbox->addWidget(input);
    m_inputs.push_back(input);
  }


  m_vbox->addSpacerItem(m_spacer);
  m_node = node;
}

void ShaderGraphNodePropertiesWidget::NodeNameChanged(const QString& name)
{
  if (m_node)
  {
    printf ("Update node name: %s\n", name.toStdString().c_str());
    fflush(stdout);
    m_node->SetKey(name.toStdString());
  }
  else
  {
    printf ("Update node name: %s - but node not set\n", name.toStdString().c_str());
    fflush(stdout);
  }
}

void ShaderGraphNodePropertiesWidget::UpdateState()
{
  if (m_resourceWidget)
  {
    m_resourceWidget->UpdateValues();
  }
  for (auto& input : m_inputs)
  {
    input->UpdateState();
  }
}

void ShaderGraphNodePropertiesWidget::RemoveAll()
{
  if (m_resourceWidget)
  {
    m_vbox->removeWidget(m_resourceWidget);
    delete m_resourceWidget;
    m_resourceWidget = nullptr;
  }
  for (auto& input : m_inputs)
  {
    m_vbox->removeWidget(input);
    delete input;
  }
  m_inputs.clear();

  m_vbox->removeItem(m_spacer);
}