//
// Created by Marcell on 22.09.2024.
//

#pragma once

#include <QOpenGLWidget>

class SceneViewWidget : public QOpenGLWidget
{
public:
  explicit SceneViewWidget (QWidget* parent);

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

};
