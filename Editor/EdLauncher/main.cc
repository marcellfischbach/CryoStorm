
#include <QMainWindow>
#include <QApplication>
#include "mainwindow.hh"


int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  MainWindow wnd;
  wnd.resize(1024, 768);
  wnd.move(100, 100);
  wnd.setVisible(true);

  return app.exec();
}


