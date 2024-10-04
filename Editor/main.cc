
#include <QMainWindow>
#include <QApplication>
#include <mainwindow.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/csVFSConfigReader.hh>


int main(int argc, char** argv)
{
  QApplication::setStyle ("Fusion");
  QApplication app(argc, argv);


  std::vector<std::string> args;
  for (int i=0; i<argc; i++)
  {
    args.emplace_back(argv[i]);
  }


  std::string dataPath("../");
  std::string configFilename = "vfs.config";
  for (size_t i = 0, in = args.size(); i < in; i++)
  {
    const std::string &arg(args[i]);
    if (arg == std::string("--data") && i + 1 < in)
    {
      dataPath = std::string(args[++i]);
    }
    else if (arg == std::string("--config") && i + 1 < in)
    {
      configFilename = std::string(args[++i]);
    }
  }


  printf("Starting with data-path: '%s'\n", dataPath.c_str()); fflush(stdout);
  cryo::csVFS::Get()->SetRootPath(dataPath);
  cryo::csVFSConfigReader::Read(dataPath, configFilename);

  MainWindow wnd;
  wnd.resize(1024, 768);
  wnd.move(100, 100);
  wnd.setVisible(true);

  return app.exec();
}


