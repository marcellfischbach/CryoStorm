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


class AssetBrowserDockWidget;
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

private:
  Ui::MainWindow* ui;

  AssetBrowserDockWidget *m_assetBrowser;
};


