//
// Created by Marcell on 21.12.2024.
//

#include <editors/shadergraph/ShaderGraphShaderGraphPropertiesWidget.hh>
#include <editors/shadergraph/ShaderGraphNodePropertiesWidget.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include "ui_ShaderGraphShaderGraphPropertiesWidget.h"

using namespace cs;

ShaderGraphShaderGraphPropertiesWidget::ShaderGraphShaderGraphPropertiesWidget(QWidget *parent)
    : QWidget(parent)
    , m_gui(new Ui::ShaderGraphShaderGraphPropertiesWidget)
    , m_shaderGraph(nullptr)
{
  m_gui->setupUi(this);
}

ShaderGraphShaderGraphPropertiesWidget::~ShaderGraphShaderGraphPropertiesWidget()
{
  delete m_gui;
  CS_RELEASE(m_shaderGraph);
}


void ShaderGraphShaderGraphPropertiesWidget::SetShaderGraph(cs::csShaderGraph *shaderGraph)
{
  CS_SET(m_shaderGraph, shaderGraph);
  m_gui->nodeProperties->SetNode(shaderGraph);
  if (shaderGraph)
  {
    m_gui->receiveShadow->setChecked(shaderGraph->IsReceiveShadow());
    m_gui->alphaDiscard->setCurrentIndex(static_cast<int>(shaderGraph->GetAlphaDiscard_Func()));
    m_gui->alphaThreshold->setValue(shaderGraph->GetAlphaDiscard_Threshold());
    m_gui->blending->setCurrentIndex(static_cast<int>(shaderGraph->GetBlendingMode()));
    m_gui->lighting->setCurrentIndex(static_cast<int>(shaderGraph->GetLightingMode()));
    m_gui->queue->setCurrentIndex(static_cast<int>(shaderGraph->GetQueue()));
  }
  UpdateAlphaThreshold();
}

void ShaderGraphShaderGraphPropertiesWidget::UpdateState()
{
  m_gui->nodeProperties->UpdateState();
}

void ShaderGraphShaderGraphPropertiesWidget::UpdateAlphaThreshold()
{
  m_gui->alphaThreshold->setEnabled(
      m_shaderGraph->GetAlphaDiscard_Func() != eCompareFunc::eCF_Never
      && m_shaderGraph->GetAlphaDiscard_Func() != eCompareFunc::eCF_Always
  );
}

void ShaderGraphShaderGraphPropertiesWidget::on_receiveShadow_stateChanged(int)
{
  UpdateValues();
}

void ShaderGraphShaderGraphPropertiesWidget::on_alphaDiscard_currentIndexChanged(int idx)
{
  UpdateValues();
}

void ShaderGraphShaderGraphPropertiesWidget::on_alphaThreshold_valueChanged(double value)
{
  UpdateValues();
}

void ShaderGraphShaderGraphPropertiesWidget::on_blending_currentIndexChanged(int)
{
  UpdateValues();
}

void ShaderGraphShaderGraphPropertiesWidget::on_lighting_currentIndexChanged(int)
{
  UpdateValues();
}

void ShaderGraphShaderGraphPropertiesWidget::on_queue_currentIndexChanged(int)
{
  UpdateValues();
}


void ShaderGraphShaderGraphPropertiesWidget::UpdateValues()
{
  m_shaderGraph->SetReceiveShadow(m_gui->receiveShadow->isChecked());
  m_shaderGraph->SetAlphaDiscard((float) m_gui->alphaThreshold->value(),
                                 static_cast<eCompareFunc>(m_gui->alphaDiscard->currentIndex()));
  m_shaderGraph->SetBlendingMode(static_cast<csShaderGraph::eBlendingMode>(m_gui->blending->currentIndex()));
  m_shaderGraph->SetLightingMode(static_cast<csShaderGraph::eLightingMode>(m_gui->lighting->currentIndex()));
  m_shaderGraph->SetQueue(static_cast<eRenderQueue>(m_gui->queue->currentIndex()));

  UpdateAlphaThreshold();
}