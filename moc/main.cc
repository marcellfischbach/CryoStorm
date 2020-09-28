

#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>

#include <ast.hh>
#include <cache.hh>
//#include <stackwalker.hh>
#include <generate/header.hh>
#include <generate/ioutput.hh>
#include <generate/master.hh>
#include <generate/source.hh>
#include <parser/sourcefile.hh>
#include <parser/token.hh>
#include <parser/tokenizer.hh>
#include <parser/parser.hh>
#include <parser/parseexception.hh>

//class StackWalkerToConsole : public StackWalker
//{
//protected:
//  virtual void OnOutput(LPCSTR szText) { printf("%s", szText); }
//};


void print_usage(char* name)
{
  printf("usage: %s [options]\n", name);
  printf("-----------------------------------\n");
  printf("  options:\n");
  printf("    --file   <file>        a single file that should be process\n");
  printf("    --source <source>      the cc file when a single file is processed\n");
  printf("    --header <header>      the hh file when a single file is processed\n");
  printf("    --path   <path>        base path that contains the moc file and where to put the files\n");
  printf("    --prefix <prefix>      the inclue prefix where include files are located\n");
  printf("    --export <export>      <export> the name of the export api name\n");
}

class StdOutOutput : public Spice::moc::iOutput
{
public:
  StdOutOutput() { }

  virtual void output(const std::string& output)
  {
    std::cout << output;
  }
};


class FileOutput : public Spice::moc::iOutput
{
public:
  FileOutput(const std::string& filename)
    : m_filename(filename)
  {
  }

  virtual void output(const std::string& output)
  {
    std::filesystem::path p(m_filename);
    std::filesystem::path pp = p.parent_path();
    std::filesystem::create_directories(pp);

    std::ofstream ostream = std::ofstream(m_filename);
    ostream << output;
  }

private:
  std::string m_filename;
};

void put_classes_to_cache(Spice::moc::Cache& cache, const std::string& sourceName, Spice::moc::ASTNode* root)
{
  std::vector<Spice::moc::ClassNode*> classes = Spice::moc::Generator::FindAllMajorClasses(root);
  for (auto cls : classes)
  {
    std::list<Spice::moc::NamespaceNode*> nss = Spice::moc::Generator::GetAllNamespaces(cls);
    std::string nsName = Spice::moc::Generator::GetFullNamespaceName(nss);
    std::cout << "  " << nsName + cls->GetName() << std::endl;

    cache.Put(sourceName, nsName + cls->GetName());
  }
}

void generate(
  Spice::moc::Cache * cache,
  const std::string & input,
  const std::string & outputHeader,
  const std::string & outputSource,
  const std::string & exp)
{

  Spice::moc::SourceFile sourceFile;
  sourceFile.Read(input);
  Spice::moc::Tokenizer tokenizer(sourceFile);

  Spice::moc::Parser parser;
  Spice::moc::ASTNode* ns = nullptr;
  if (cache)
  {
    cache->Clear(input);
    cache->Touch(input);
  }

  try
  {
    ns = parser.Parse(tokenizer);

    if (cache)
    {
      put_classes_to_cache(*cache, input, ns);
    }


    if (!outputHeader.empty())
    {
      FileOutput output(outputHeader);
      Spice::moc::HeaderGenerator hg;
      hg.SetRoot(ns);
      hg.Output(exp, &output);
    }

    if (!outputSource.empty())
    {
      FileOutput output(outputSource);
      Spice::moc::SourceGenerator sg;
      sg.SetRoot(ns);
      sg.Output(exp, &output);
    }
    //    ns->DebugNode(0);
  }
  catch (Spice::moc::ParseException & e)
  {
    std::cout << "Parse Exception: [" << e.GetFile() << "@" << e.GetLine() << "] '" << e.GetMessage() << "' in " << input << std::endl;
  }
  catch (std::exception & ex)
  {
    std::cout << "Unhandled exception '" << ex.what() << "' in " << input << std::endl;
  }
  catch (...)
  {
    if (ns)
    {
      ns->DebugNode(0);
    }
    else
    {
      std::cout << "Unhandled exception in " << input << std::endl;
    }
  }
}




bool compareChar(const char& c1, const char& c2)
{
  if (c1 == c2)
    return true;
  else if (std::toupper(c1) == std::toupper(c2))
    return true;
  return false;
}

/*
 * Case Insensitive String Comparision
 */
bool equalsCI(const std::string & str1, const std::string & str2)
{
  return ((str1.size() == str2.size()) &&
    std::equal(str1.begin(), str1.end(), str2.begin(), &compareChar));
}


void generate_list(const std::string & path, const std::string & exp, const std::string & prefix)
{
  Spice::moc::Cache cache;
  cache.Load(path);

  std::ifstream stream(path + "/.spicemoc");
  std::string filename;
  bool neededRevalidation = false;
  while (std::getline(stream, filename))
  {
    size_t idx = filename.rfind('.');
    if (idx == std::string::npos)
    {
      continue;
    }
    std::string ext = filename.substr(idx);
    if (!(equalsCI(ext, std::string(".h"))
      || equalsCI(ext, std::string(".hh"))
      || equalsCI(ext, std::string(".hpp"))
      || equalsCI(ext, std::string(".hxx"))
      )) {
      continue;
    }

    std::string plainFilename = filename.substr(0, idx);

    if (cache.NeedRevalidation(filename))
    {
      neededRevalidation = true;
      try
      {
        std::cout << "csmoc: " << filename << std::endl;
        generate(
          &cache,
          filename,
          path + "/" + plainFilename + ".refl.hh",
          path + "/" + plainFilename + ".refl.cc",
          exp
        );
      }
      catch (Spice::moc::BaseException & e)
      {
        std::cout << "Exception: [" << e.GetFile() << "@" << e.GetLine() << "] '" << e.GetMessage() << "' @ " << filename << std::endl;
      }
      catch (...)
      {
        std::cout << "Unhandled exception in " << filename << std::endl;
      }
    }
    else
    {
      cache.Touch(filename);
    }
  }

  std::filesystem::path masterPath(path + "/master.refl.cc");
  if (neededRevalidation || cache.HaveUntouched() || !std::filesystem::exists(masterPath))
  {
    Spice::moc::MasterGenerator masterGenerator;
    FileOutput output(path + "/master.refl.cc");
    masterGenerator.Generate(cache, prefix, &output);
  }
  cache.Store(path);

}





int main(int argc, char** argv)
{

  std::filesystem::path cp = std::filesystem::current_path();

  std::string file;
  std::string source;
  std::string header;
  std::string path;
  std::string prefix;
  std::string exp;

  header = "header";
  source = "source";
  exp = "";
  // file = "D:\\DEV\\CobaltSKY\\Engine\\cobalt\\entity\\csdynamiccolliderstate.hh";
  path = "C:\\Users\\MCEL\\CMakeBuilds\\d5ee80a5-d1ee-2937-85d1-86f3b75e756d\\build\\x64-Release\\src\\test";


  for (int i = 0; i < argc; i++)
  {
  printf ("%s ", argv[i]);
  }
  printf ("\n");
  for (int i = 1; i < argc; i++)
  {
    printf ("%s ", argv[i]);
    std::string arg(argv[i]);
    if (arg == "--file")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      file = std::string(argv[++i]);
      // std::cout << " file: '" << file << "'";
    }
    else if (arg == "--header")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      header = std::string(argv[++i]);
      // std::cout << " header: '" << header << "'";
    }
    else if (arg == "--source")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      source = std::string(argv[++i]);
      // std::cout << " source: '" << source << "'";
    }
    else if (arg == "--path")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      path = std::string(argv[++i]);
      // std::cout << " path: '" << path << "'";
    }
    else if (arg == "--prefix")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      prefix = std::string(argv[++i]);
      //std::cout << " prefix: '" << prefix << "'";
    }
    else if (arg == "--export")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      exp = std::string(argv[++i]);
      //std::cout << " exp: '" << exp << "'";
    }
    else
    {
      printf("Invalid arg %s\n", arg.c_str());
      print_usage(argv[0]);
      return -1;
    }
  }

  if (!file.empty() && !source.empty() && !header.empty())
  {
    generate(nullptr, file, header, source, exp);
  }
  else if (!path.empty())
  {

    generate_list(path, exp, prefix);
  }

  return 0;
}
