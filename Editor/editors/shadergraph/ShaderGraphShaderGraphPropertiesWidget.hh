//
// Created by Marcell on 21.12.2024.
//

#pragma once

#include <QWidget>
#include <csCore/csRef.hh>

namespace Ui
{
class ShaderGraphShaderGraphPropertiesWidget;
}

namespace cs
{
class csShaderGraph;
}

class ShaderGraphShaderGraphPropertiesWidget : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphShaderGraphPropertiesWidget(QWidget *parent = nullptr);
  ~ShaderGraphShaderGraphPropertiesWidget() override;

  void SetShaderGraph(cs::csShaderGraph* shaderGraph);
  void UpdateState();

private slots:
  void on_receiveShadow_stateChanged(int);
  void on_alphaDiscard_currentIndexChanged (int);
  void on_alphaThreshold_valueChanged (double);
  void on_blending_currentIndexChanged (int);
  void on_lighting_currentIndexChanged (int);
  void on_queue_currentIndexChanged (int);

private:
  void UpdateValues ();
  void UpdateAlphaThreshold ();
  Ui::ShaderGraphShaderGraphPropertiesWidget *m_gui;

  cs::csRef<cs::csShaderGraph> m_shaderGraph;

};
