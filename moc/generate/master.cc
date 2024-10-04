

#include <generate/master.hh>
#include <generate/ioutput.hh>
#include <cache.hh>

namespace cryo::moc
{

MasterGenerator::MasterGenerator()
{

}


void MasterGenerator::Generate(Cache& cache, iOutput* output, const std::string &sourcePath)
{
  std::string headerIncludes;
  std::string sourceIncludes;

  headerIncludes += "#include <stdarg.h>\n";

  std::string register_classes = "";
  register_classes += "static void register_classes()\n";
  register_classes += "{\n";
  register_classes += "  cryo::csClassRegistry *reg = cryo::csClassRegistry::Get();\n";

  std::string unregister_classes = "";
  unregister_classes += "static void unregister_classes()\n";
  unregister_classes += "{\n";
  unregister_classes += "  cryo::csClassRegistry *reg = cryo::csClassRegistry::Get();\n";

  for (auto it : cache.GetFileCache())
  {
    Cache::Data& data = it.second;
    if (data.classes.empty())
    {
      continue;
    }

    size_t idx = data.filename.find_last_of('.');
    std::string filename = data.filename.substr(0, idx);
    headerIncludes += "#include \"" + sourcePath + "/" + data.filename + "\"\n";
    headerIncludes += "#include \"" + filename + ".refl.hh\"\n";
    sourceIncludes += "#include \"" + filename + ".refl.cc\"\n";

    register_classes += "  // " + data.filename + "\n";
    unregister_classes += "  // " + data.filename + "\n";
    for (auto cls : data.classes)
    {
      register_classes += "  reg->Register(" + cls + "Class::Get());\n";
      unregister_classes += "  reg->Unregister(" + cls + "Class::Get());\n";
    }
  }
  register_classes += "}\n\n";
  unregister_classes += "}\n\n";

  std::string source = "\n\n";
  source += headerIncludes;
  source += "\n\n";
  source += sourceIncludes;
  source += "\n\n";
  source += "#include <ceCore/csClassRegistry.hh>\n\n";
  source += register_classes;
  source += unregister_classes;

  if (output)
  {
    output->output(source);
  }

}

}

