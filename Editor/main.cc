
#include <QMainWindow>
#include <QApplication>
#include <QSurfaceFormat>
#include <mainwindow.hh>
#include <default_registries.hh>
#include <csCore/resource/csAssetLocator.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/csVFSConfigReader.hh>
#include <Project.hh>
#include <master.refl.cc>


void print_usage()
{
  printf ("Usage: EdLauncher <project-path>\n");
  printf ("---------\n");
  printf ("  <project-path>   folder of the cryo-project.xml file\n");
}

std::string extract_path (const std::string &projectDesc)
{
  std::string path = projectDesc;
  size_t idx = path.rfind("/cryo-project.xml");
  if (idx == std::string::npos)
  {
    idx = path.rfind("\\cryo-project.xml");
    if (idx == std::string::npos)
    {
      return path;
    }
  }

  path = path.substr(0, idx);
//  if (path.ends_with("/cryo-project.xml") || path.ends_with("\\cryo-project.xml"))
//  {
//      path = path.substr(0, path.size() - 10);
//  }

  while (path.ends_with("/") || path.ends_with("\\"))
  {
    path = path.substr(0, path.size()-1);
  }

  return path;
}

int main(int argc, char** argv)
{
  register_classes();


  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

  QSurfaceFormat format;
  format.setRedBufferSize(8);
  format.setGreenBufferSize(8);
  format.setBlueBufferSize(8);
  format.setAlphaBufferSize(8);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  format.setSwapInterval(0);
  format.setMajorVersion(4);
  format.setMinorVersion(4);
  format.setProfile(QSurfaceFormat::CompatibilityProfile);
  QSurfaceFormat::setDefaultFormat(format);

  QApplication::setStyle ("Fusion");
  QApplication app(argc, argv);


  std::vector<std::string> args;
  for (int i=0; i<argc; i++)
  {
    args.emplace_back(argv[i]);
  }

  if (args.size() < 2)
  {
    print_usage();
    return -1;
  }

  std::string projectPath = extract_path(args[1]);
  Project::Get()->Open(projectPath);
//  cs::csVFS *vfs = cs::csVFS::Get();
//  std::string dataPath("../");
//  std::string configFilename = "vfs.config";
//  for (size_t i = 0, in = args.size(); i < in; i++)
//  {
//    const std::string &arg(args[i]);
//    if (arg == std::string("--data") && i + 1 < in)
//    {
//      dataPath = std::string(args[++i]);
//    }
//    else if (arg == std::string("--config") && i + 1 < in)
//    {
//      configFilename = std::string(args[++i]);
//    }
//  }
//
//
//  printf("Starting with data-path: '%s'\n", dataPath.c_str()); fflush(stdout);
//  cs::csVFS::Get()->SetRootPath(dataPath);
//  cs::csVFSConfigReader::Read(dataPath, configFilename);

  MainWindow wnd;
  wnd.resize(1024, 768);
  wnd.move(100, 100);
  wnd.setVisible(true);


  register_defaults();

  return QApplication::exec();
}


