//
// Created by Marcell on 22.09.2024.
//
#include <GL/glew.h>

#include <sceneview/SceneViewWidget.hh>
#include <QCoreApplication>
#include <QTimer>
#include <csCore/csEngine.hh>
#include <csCore/csViewport.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iFrameRenderer.hh>
#include <csCore/window/iWindow.hh>
#include <csOpenGL/gl4/csGL4RenderTarget2D.hh>


using namespace cs;

static bool s_engineInitialized = false;




SceneViewWidget::SceneViewWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_viewport(nullptr)
    , m_window(new SceneViewWidget_WindowPriv(this))
    , m_world(nullptr)
{

  m_timer = new QTimer(this);
  connect (m_timer, SIGNAL(timeout()), this, SLOT(update()));
  m_timer->setInterval(10);
  m_timer->start();

  setUpdateBehavior(QOpenGLWidget::PartialUpdate);
}

SceneViewWidget::~SceneViewWidget()
{
  delete m_window;
  m_window = nullptr;
}

csWorld* SceneViewWidget::GetWorld()
{
  return m_world;
}


const csWorld* SceneViewWidget::GetWorld() const
{
  return m_world;
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
  csObjectRegistry::Get<iDevice>()->CheckError();

}

void SceneViewWidget::resizeGL(int w, int h)
{
  csObjectRegistry::Get<iDevice>()->CheckError();
  glViewport(0, 0, w, h);
  csObjectRegistry::Get<iDevice>()->CheckError();
}

void SceneViewWidget::paintGL()
{
  // drain the error state.... it looks like Qt leaves the openGL context in an error state
  glGetError();

  if (!m_world)
  {
    m_world = new csWorld();
    emit initialize(m_world);
  }
  GLuint fb = defaultFramebufferObject();
  opengl::csGL4RenderTarget2D rt (fb, QWidget::width(), QWidget::height());


  if (!m_viewport)
  {
    m_viewport = new csViewport();
    m_viewport->SetWindow(m_window);
    m_viewport->SetDevice(csObjectRegistry::Get<iDevice>());
    m_viewport->SetWorld(m_world);
    m_viewport->SetFrameRenderer(csObjectRegistry::Get<iFrameRenderer>());
  }

  m_viewport->ProcessFrame(&rt);
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