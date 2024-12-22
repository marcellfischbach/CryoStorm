//
// Created by Marcell on 21.12.2024.
//

#include <editors/shadergraph/ShaderGraphNodePropertiesWidget.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>
#include <ui_ShaderGraphNodeInputWidget.h>


ShaderGraphNodeInputWidget::ShaderGraphNodeInputWidget(QWidget *parent)
    : QWidget(parent)
    , m_gui(new Ui::ShaderGraphNodeInputWidget ())
    , m_input(nullptr)
{
  m_gui->setupUi(this);

  connect (m_gui->valueF1X, SIGNAL(valueChanged(double)), this, SLOT(SyncInput(double)));
}

ShaderGraphNodeInputWidget::~ShaderGraphNodeInputWidget ()
{
  CS_RELEASE(m_input);
}

void ShaderGraphNodeInputWidget::SetInput(cs::csSGNodeInput *input)
{
  CS_SET(m_input, input);
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
  printf ("Sync\n");
  if (m_input)
  {
    if (!m_input->GetSource())
    {
      m_input->SetScalar((float)m_gui->valueF1X->value());
    }
  }
}


ShaderGraphNodePropertiesWidget::ShaderGraphNodePropertiesWidget(QWidget *parent)
    : QWidget(parent)
    , m_node(nullptr)
    , m_vbox(new QVBoxLayout(this))
    , m_spacer(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding))
{
}

ShaderGraphNodePropertiesWidget::~ShaderGraphNodePropertiesWidget()
{
  CS_RELEASE(m_node);
}

void ShaderGraphNodePropertiesWidget::SetNode(cs::csSGNode *node)
{
  RemoveAll();

  for (int i = 0; i < node->GetNumberOfInputs(); ++i)
  {
    cs::csSGNodeInput *sgInput = node->GetInput(i);
    ShaderGraphNodeInputWidget* input = new ShaderGraphNodeInputWidget(this);
    input->SetInput(sgInput);
    input->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    m_vbox->addWidget(input);
    m_inputs.push_back(input);
  }


  m_vbox->addSpacerItem(m_spacer);
  CS_SET(m_node, node);
}

void ShaderGraphNodePropertiesWidget::UpdateState ()
{
  for (auto &input: m_inputs)
  {
    input->UpdateState();
  }
}

void ShaderGraphNodePropertiesWidget::RemoveAll()
{
  for (auto &input: m_inputs)
  {
    m_vbox->removeWidget(input);
    delete input;
  }
  m_inputs.clear();

  m_vbox->removeItem(m_spacer);
}