//
// Created by Marcell on 22.09.2024.
//

#include <sceneview/SceneViewWidget.hh>
#include <QCoreApplication>
#include <csCore/csEngine.hh>
#include <csCore/csViewport.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iFrameRenderer.hh>
#include <csCore/window/iWindow.hh>

using namespace cs;

static bool s_engineInitialized = false;




SceneViewWidget::SceneViewWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_viewport(nullptr)
    , m_window(new SceneViewWidget_WindowPriv(this))
{

}


std::vector<std::string> extract_args()
{
  std::vector<std::string> args;
  QStringList arguments = QCoreApplication::arguments();
  for (QString arg: arguments)
  {
    args.push_back(std::string(arg.toLatin1()));
  }

  args.push_back("--glProfile");
  args.push_back("compat");

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
  if (!m_viewport)
  {
    m_viewport = new csViewport();
    m_viewport->SetWindow(m_window);
    m_viewport->SetDevice(csObjectRegistry::Get<iDevice>());
    m_viewport->SetWorld(new csWorld());
    m_viewport->SetFrameRenderer(csObjectRegistry::Get<iFrameRenderer>());
  }

  m_viewport->ProcessFrame();
}



SceneViewWidget_WindowPriv::SceneViewWidget_WindowPriv(SceneViewWidget *window)
: iWindow()
, m_window(window)
{

}

void SceneViewWidget_WindowPriv::SetTitle(const std::string &title)
{
  m_title = title;
}

const std::string &SceneViewWidget_WindowPriv::GetTitle() const
{
  return m_title;
}

void SceneViewWidget_WindowPriv::SetPosition(int16_t x, int16_t y)
{
  //
}
int16_t SceneViewWidget_WindowPriv::GetPositionX() const
{
  return 0;
}

int16_t SceneViewWidget_WindowPriv::GetPositionY() const
{
  return 0;
}

void SceneViewWidget_WindowPriv::SetResolution(uint16_t width, uint16_t height)
{
  // nope can't set the resoluation
}

int16_t SceneViewWidget_WindowPriv::GetWidth() const
{
  return static_cast<int16_t>(m_window->width());
}

int16_t SceneViewWidget_WindowPriv::GetHeight() const
{
  return static_cast<int16_t>(m_window->height());
}

void SceneViewWidget_WindowPriv::SetWindowIcon(const csResourceLocator &iconName)
{
}

void SceneViewWidget_WindowPriv::Show()
{
}
void SceneViewWidget_WindowPriv::Hide()
{
}

void SceneViewWidget_WindowPriv::Present()
{
}

void SceneViewWidget_WindowPriv::ProcessUpdates()
{
}