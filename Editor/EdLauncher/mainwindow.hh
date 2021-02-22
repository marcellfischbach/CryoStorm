//
// Created by MCEL on 22.02.2021.
//

#ifndef SPICEENGINE_MAINWINDOW_HH
#define SPICEENGINE_MAINWINDOW_HH

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

private:
  Ui::MainWindow* ui;
};

#endif //SPICEENGINE_MAINWINDOW_HH
