//
// Created by mcel on 30.04.2024.
//

#include <csLauncher/window/csSDLWindow.hh>
#include <csCore/csSettings.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/graphics/csImage.hh>
#include <csCore/entity/csEntity.hh>



namespace cs::launcher
{



void csSDLWindow::do_something_raw (csEntity* entity)
{
  if (entity)
  {
    printf("Do something: \"%s\"\n", entity->GetName().c_str());
  }
  else
  {
    printf ("Do someting: Entity is null\n");
  }
}

void csSDLWindow::do_something_ref (csRef<csEntity> &entity)
{
  if (entity)
  {
    printf("Do something: \"%s\"\n", entity->GetName().c_str());
  }
  else
  {
    printf ("Do someting: Entity is null\n");
  }
}

void csSDLWindow::do_something_value (csRef<csEntity> entity)
{
  if (entity)
  {
    printf("Do something: \"%s\"\n", entity->GetName().c_str());
  }
  else
  {
    printf ("Do someting: Entity is null\n");
  }
}

csSDLWindow::csSDLWindow()
{
  csRef<csEntity> entity = new csEntity("Name");
  do_something_raw(entity.raw());
  do_something_ref(entity);
  do_something_value(entity);
  fflush(stdout);
}

csSDLWindow::~csSDLWindow()
{
  if (m_window)
  {
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
  }
}

bool csSDLWindow::Initialize(bool compat)
{
  const cs::csSettingsFile &settings = cs::csSettings::Get().Display();


  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
  if (compat)
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
  }
  else
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  }
  uint32_t flags = SDL_WINDOW_OPENGL;
  // flags |= SDL_WINDOW_BORDERLESS;
  std::string title = settings.GetText("title");
  cs::csVector2i res = settings.GetVector2i("resolution");
  cs::csVector2i pos = settings.GetVector2i("pos");

  std::string viewMode = settings.GetText("viewmode", "windowed");
  if (viewMode == "fullscreen")
  {
    flags |= SDL_WINDOW_FULLSCREEN;
  }
  else if (viewMode == "desktop")
  {
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  }
  else if (viewMode == "borderless")
  {
    flags |= SDL_WINDOW_BORDERLESS;
  }
  else if (viewMode == "windowed")
  {

  }
  else if (viewMode != "windowed")
  {
    printf("Illegal viewmode: '%s'. Defaulting to 'windowed'\n", viewMode.c_str());
    return false;
  }

  m_posX = pos.x;
  m_posY = pos.y;
  m_width = res.x;
  m_height = res.y;
  m_title = title;

  bool vsync = settings.GetBool("vsync");
  m_window = SDL_CreateWindow(title.c_str(),
                              pos.x, pos.y,
                              res.x, res.y, flags
  );
  m_mouse.SetWindow(m_window);
  //  wnd = SDL_CreateWindow("Spice", 0, 0, 1920, 1080, flags);
  m_glContext = SDL_GL_CreateContext(m_window);
  SDL_GL_SetSwapInterval(vsync ? 1 : 0);


  if (m_visible)
  {
    Show();
  }

  return true;
}

void csSDLWindow::SetTitle(const std::string &title)
{
  m_title = title;
  if (m_window)
  {
    SDL_SetWindowTitle(m_window, title.c_str());
  }
}

const std::string &csSDLWindow::GetTitle() const
{
  return m_title;
}

void csSDLWindow::SetPosition(int16_t x, int16_t y)
{
  m_posX = x;
  m_posY = y;
  if (m_window)
  {
    SDL_SetWindowPosition(m_window, x, y);
  }
}

int16_t csSDLWindow::GetPositionX() const
{
  return m_posX;
}

int16_t csSDLWindow::GetPositionY() const
{
  return m_posY;
}

void csSDLWindow::SetResolution(uint16_t width, uint16_t height)
{
  m_width = width;
  m_height = height;
  if (m_window)
  {
    SDL_SetWindowSize(m_window, width, height);
  }
}

int16_t csSDLWindow::GetWidth() const
{
  return m_width;
}


int16_t csSDLWindow::GetHeight() const
{
  return m_height;
}

void csSDLWindow::SetWindowIcon(const cs::csAssetLocator &locator)
{
  if (m_window)
  {

    auto image = cs::csAssetManager::Get()->Load<cs::csImage>("/icons/cryo-stasis_64.png");
    if (!image)
    {
      return;
    }

    SDL_Surface *surf = SDL_CreateRGBSurface(0,
                                             image->GetWidth(),
                                             image->GetHeight(),
                                             32,
                                             0x000000ff,
                                             0x0000ff00,
                                             0x00ff0000,
                                             0xff000000);
    SDL_LockSurface(surf);
    SDL_memcpy(surf->pixels, image->GetData(), image->GetWidth() * image->GetHeight() * 4);
    SDL_UnlockSurface(surf);
    SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_BLEND);

    SDL_SetWindowIcon(m_window, surf);
  }
}

void csSDLWindow::Show()
{
  m_visible = true;
  if (m_window)
  {
    SDL_ShowWindow(m_window);
    SDL_GL_MakeCurrent(m_window, m_glContext);
  }
}


void csSDLWindow::Hide()
{
  m_visible = false;
  if (m_window)
  {
    SDL_HideWindow(m_window);
  }
}

void csSDLWindow::Present()
{
  if (m_window)
  {
    SDL_GL_SwapWindow(m_window);
  }
}

void csSDLWindow::ProcessUpdates()
{
  if (m_window)
  {
    m_keyboard.Update();
    m_mouse.Update();
    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
      switch (evt.type)
      {
        case SDL_KEYDOWN:
          m_keyboard.Update(evt.key.keysym.scancode, true);
          break;
        case SDL_KEYUP:
          m_keyboard.Update(evt.key.keysym.scancode, false);
          break;
        case SDL_MOUSEBUTTONDOWN:
          m_mouse.Update(evt.button.button, true);
          break;
        case SDL_MOUSEBUTTONUP:
          m_mouse.Update(evt.button.button, false);
          break;
        case SDL_MOUSEWHEEL:
          m_mouse.Update(evt.wheel.y, evt.wheel.x);
          break;
        case SDL_MOUSEMOTION:
          m_mouse.Update(evt.motion.x, evt.motion.y, evt.motion.xrel, evt.motion.yrel);
          break;
      }
    }
  }
}


csSDLMouse *csSDLWindow::GetMouse()
{
  return &m_mouse;
}

csSDLKeyboard *csSDLWindow::GetKeyboard()
{
  return &m_keyboard;
}

} 