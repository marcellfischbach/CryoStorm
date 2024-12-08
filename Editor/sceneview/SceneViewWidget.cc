//
// Created by Marcell on 22.09.2024.
//

#include <sceneview/SceneViewWidget.hh>
#include <QCoreApplication>
#include <csCore/csEngine.hh>

using namespace cs;

static bool s_engineInitialized = false;

SceneViewWidget::SceneViewWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

}


std::vector<std::string> extract_args ()
{
  std::vector<std::string> args;
  QStringList arguments = QCoreApplication::arguments();
  for(QString arg : arguments)
  {
    args.push_back(std::string(arg.toLatin1()));
  }
  return args;
}

void SceneViewWidget::initializeGL()
{
  if (s_engineInitialized)
  {
    return;
  }


  s_engineInitialized = true;

  csModuleConfig config;
  config.LoadModuleConfig();

  csEngine *engine = csEngine::Get();
  if (!engine->InitializeEngine(extract_args(), config))
  {
    fprintf(stderr, "Unable to initialize engine\n");
  };
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

