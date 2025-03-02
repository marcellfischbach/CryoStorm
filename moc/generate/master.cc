

#include <generate/master.hh>
#include <generate/ioutput.hh>
#include <cache.hh>

namespace cs::moc
{

MasterGenerator::MasterGenerator()
{

}


void MasterGenerator::Generate(Cache& cache, iOutput* output, const std::string &sourcePath)
{
  std::string source;
  source += "#include <csCore/csClassRegistry.hh>\n\n";
  source += "#include <stdarg.h>\n";
  source += "#ifdef CS_WIN32\n";
  source += "#include <Windows.h>\n";

  source += R"(
static HMODULE GetCurrentModuleHandle()
{
  HMODULE hModule = NULL;
  GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetCurrentModuleHandle, &hModule);
  return hModule;
}

static std::string CreateClassResolver(const std::string& name)
{
  std::string res;
  res.reserve(name.length() + 15);
  for (auto ch : name)
  {
    if (ch == ':')
    {
      ch = '_';
    }
    res += ch;
  }

  res += "_GetStaticClass";
  return res;
}

static const cs::csClass* GetClassInstance(HMODULE hModule, const std::string& name)
{
  FARPROC proc = GetProcAddress(hModule, CreateClassResolver(name).c_str());
  if (proc == nullptr)
  {
    return nullptr;
  }
  typedef const cs::csClass* (*GetClass)();  return reinterpret_cast<GetClass>(proc)();
}
)";
  /*
  source += "static HMODULE GetCurrentModuleHandle()\n";
  source += "{\n";
  source += "  HMODULE hModule = NULL;\n";
  source += "  GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetCurrentModuleHandle, &hModule);\n";
  source += "  return hModule;\n";
  source += "}\n\n";

  source += "static const cs::csClass* GetClassInstance(HMODULE hModule, const std::string& name)\n";
  source += "{\n";
  source += "  FARPROC proc = GetProcAddress(hModule, name.c_str());\n";
  source += "  if (proc == nullptr)\n";
  source += "  {\n";
  source += "    return nullptr;\n";
  source += "  }\n";
  source += "  typedef const cs::csClass* (*GetClass)();";
  source += "  return reinterpret_cast<GetClass>(proc)();\n";
  source += "}\n\n";
  */
  source += "#endif\n\n";

  std::string register_classes = "";
  register_classes += "static void register_classes()\n";
  register_classes += "{\n";
  register_classes += "  cs::csClassRegistry *reg = cs::csClassRegistry::Get();\n";
  register_classes += "#ifdef CS_WIN32\n";
  register_classes += "  HMODULE hModule = GetCurrentModuleHandle();\n";
  register_classes += "#endif\n";

  std::string unregister_classes = "";
  unregister_classes += "static void unregister_classes()\n";
  unregister_classes += "{\n";
  unregister_classes += "  cs::csClassRegistry *reg = cs::csClassRegistry::Get();\n";
  unregister_classes += "#ifdef CS_WIN32\n";
  unregister_classes += "  HMODULE hModule = GetCurrentModuleHandle();\n";
  unregister_classes += "#endif\n";
  for (auto it : cache.GetFileCache())
  {
    Cache::Data& data = it.second;
    if (data.classes.empty())
    {
      continue;
    }

    register_classes += "  // " + data.filename + "\n";
    unregister_classes += "  // " + data.filename + "\n";
    for (auto cls : data.classes)
    {
      register_classes += "  reg->Register(GetClassInstance(hModule, \"" + cls + "\"));\n";
      unregister_classes += "  reg->Unregister(GetClassInstance(hModule, \"" + cls + "\"));\n";
    }
  }
  register_classes += "}\n\n";
  unregister_classes += "}\n\n";

  //source += headerIncludes;
  //source += "\n\n";
  //source += sourceIncludes;
  //source += "\n\n";
  source += register_classes;
  source += unregister_classes;

  if (output)
  {
    output->output(source);
  }

}

}

