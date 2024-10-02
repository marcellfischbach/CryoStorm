//
// Created by Marcell on 22.09.2024.
//

#include <sceneview/sceneviewwidget.hh>

SceneViewWidget::SceneViewWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

}


void SceneViewWidget::initializeGL()
{

}

void SceneViewWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
}

void SceneViewWidget::paintGL()
{
  glClearColor(0.5f, 0.0, 0.0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

