//
// Created by Marcell on 22.09.2024.
//

#pragma once

#include <QOpenGLWidget>
#include <csCore/window/iWindow.hh>
#include <csCore/csRef.hh>

namespace cs
{
class csViewport;
class csWorld;
}

class QTimer;
class SceneViewWidget;

CS_CLASS()
class SceneViewWidget_WindowPriv : public cs::iWindow
{
CS_CLASS_GEN_OBJECT;
public:
  SceneViewWidget_WindowPriv(SceneViewWidget *window);
  ~SceneViewWidget_WindowPriv() override = default;

  void SetTitle(const std::string &title) override;
  const std::string &GetTitle() const override;

  void SetPosition(int16_t x, int16_t y) override;
  int16_t GetPositionX() const override;
  int16_t GetPositionY() const override;

  void SetResolution(uint16_t width, uint16_t height) override;
  int16_t GetWidth() const override;
  int16_t GetHeight() const override;


  void SetWindowIcon(const cs::csAssetLocator &iconName) override;

  void Show() override;
  void Hide() override;

  void Present() override;

  void ProcessUpdates() override;

private:
  std::string     m_title;
  SceneViewWidget *m_window;


};


class SceneViewWidget : public QOpenGLWidget
{
Q_OBJECT
public:
  explicit SceneViewWidget(QWidget *parent);
  ~SceneViewWidget() override;

  cs::csWorld *GetWorld();
  const cs::csWorld *GetWorld() const;

signals:
  void initialize(cs::csWorld *world);


protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

private:
  bool m_engineInitialized = false;
  cs::csRef<cs::csViewport> m_viewport;
  cs::csRef<cs::csWorld> m_world;

  class WindowPriv;
  cs::csRef<SceneViewWidget_WindowPriv> m_window;
  QTimer *m_timer;
};

