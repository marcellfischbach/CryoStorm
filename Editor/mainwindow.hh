//
// Created by MCEL on 22.02.2021.
//
#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

namespace cs
{
class csWorld;
}

class AssetBrowserDockWidget;
class SceneViewWidget;
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

  void showEvent (QShowEvent* event) override;


public slots:
  void on_actionCreateShaderGraph_triggered(bool checked);

private slots:
  void initializeScene (cs::csWorld* world);
private:
  Ui::MainWindow* ui;

  AssetBrowserDockWidget *m_assetBrowser;
  SceneViewWidget *m_sceneViewWidget;
};


