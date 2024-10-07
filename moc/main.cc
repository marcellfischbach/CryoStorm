

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
#include <generate/javaconverter.hh>

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
  printf("    --sourcepath <path>    the base path where the source code is located\n");
  printf("    --header <header>      the hh file when a single file is processed\n");
  printf("    --path   <path>        base path that contains the moc file and where to put the files\n");
  printf("    --prefix <prefix>      the include prefix where include files are located\n");
  printf("    --javaConverter <path> path, where *-moc.xml files are located");
}

class StdOutOutput : public cryo::moc::iOutput
{
public:
  StdOutOutput() { }

  virtual void output(const std::string& output)
  {
    std::cout << output;
  }
};


class FileOutput : public cryo::moc::iOutput
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

void put_classes_to_cache(cryo::moc::Cache& cache, const std::string& sourceName, cryo::moc::ASTNode* root)
{
  std::vector<cryo::moc::ClassNode*> classes = cryo::moc::Generator::FindAllMajorClasses(root);
  for (auto cls : classes)
  {
    std::list<cryo::moc::NamespaceNode*> nss = cryo::moc::Generator::GetAllNamespaces(cls);
    std::string nsName = cryo::moc::Generator::GetFullNamespaceName(nss);
    std::cout << "  " << nsName + cls->GetName() << std::endl;

    cache.Put(sourceName, nsName + cls->GetName());
  }
}

void generate(
  cryo::moc::Cache* cache,
  const std::string& input,
  const std::string& outputHeader,
  const std::string& outputSource)
{

  cryo::moc::SourceFile sourceFile;
  sourceFile.Read(input);
  cryo::moc::Tokenizer tokenizer(sourceFile);

  cryo::moc::Parser parser;
  cryo::moc::ASTNode* ns = nullptr;
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
      cryo::moc::HeaderGenerator hg;
      hg.SetRoot(ns);
      hg.Output(&output);
    }

    if (!outputSource.empty())
    {
      FileOutput output(outputSource);
      cryo::moc::SourceGenerator sg;
      sg.SetRoot(ns);
      sg.Output(&output);
    }
    //    ns->DebugNode(0);
  }
  catch (cryo::moc::ParseException& e)
  {
    std::cout << "Parse Exception: [" << e.GetFile() << "@" << e.GetLine() << "] '" << e.GetMessage()
              << "' in " << input << "@" << e.getSourceLine() << ":" << e.getSourceColumn() << std::endl;
  }
  catch (std::exception& ex)
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
bool equalsCI(const std::string& str1, const std::string& str2)
{
  return ((str1.size() == str2.size()) &&
    std::equal(str1.begin(), str1.end(), str2.begin(), &compareChar));
}

std::vector<std::string> read_all_filenames(const std::string& spicemoc)
{
  std::vector<std::string> all_filenames;
  std::ifstream stream(spicemoc);
  std::string filename;
  while (std::getline(stream, filename))
  {
    all_filenames.push_back(filename);
  }
  return all_filenames;
}

void scan_directory(const std::filesystem::path& root, const std::filesystem::path& path, std::vector<std::string>& result)
{
//  std::cout << "scan_directory: " << path.string() << std::endl;
  for (std::filesystem::path child : std::filesystem::directory_iterator(path))
  {
    if (std::filesystem::is_directory(child))
    {
      scan_directory(root, child, result);
    }
    else
    {
      std::string ext = child.extension().string();
      if (equalsCI(ext, ".h") ||
        equalsCI(ext, ".hh") ||
        equalsCI(ext, ".hpp") ||
        equalsCI(ext, ".hxx") ||
        equalsCI(ext, ".c") ||
        equalsCI(ext, ".cc") ||
        equalsCI(ext, ".cpp") ||
        equalsCI(ext, ".cxx")
        ) {
        result.push_back(child.lexically_relative(root).string());
      }
    }
  }
  //std::cout << "scan_directory: " << path.string() << " -- done " << std::endl;
}

std::vector<std::string> scan_directory()
{
  std::vector<std::string> result;
  std::filesystem::path path = std::filesystem::current_path();
  scan_directory(path, path, result);
  return result;
}


void generate_list(const std::string& path, const std::string &sourcePath)
{
  cryo::moc::Cache cache;
  cache.Load(path);

  //std::vector<std::string> all_filenames = read_all_filenames(path + "/.spicemoc");
  std::vector<std::string> scanned_filenames = scan_directory();
  //std::ifstream stream(path + "/.spicemoc");
  //std::string filename;
  bool neededRevalidation = false;
  for (std::string filename : scanned_filenames)
  {
    //std::cout << "  " << filename << std::endl;
    //    while (std::getline(stream, filename))
    //  {
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
        generate(
          &cache,
          filename,
          path + "/" + plainFilename + ".refl.hh",
          path + "/" + plainFilename + ".refl.cc"
        );
      }
      catch (cryo::moc::BaseException& e)
      {
        std::cout << "Exception: [" << e.GetFile() << "@" << e.GetLine() << "] '" << e.GetMessage() << "' @ " << filename << std::endl;
      }
      catch (...)
      {
        std::cout << "Unhandled exception in " << filename << std::endl;
      }
    }
    cache.Touch(filename);
  }
  size_t removed_entries = cache.RemoveAllUntouched();

  std::filesystem::path masterPath(path + "/master.refl.cc");
  if (neededRevalidation || removed_entries != 0 || !std::filesystem::exists(masterPath))
  {
    cryo::moc::MasterGenerator masterGenerator;
    FileOutput output(path + "/master.refl.cc");
    masterGenerator.Generate(cache, &output, sourcePath);
  }
  cache.Store(path);

}





int main(int argc, char** argv)
{

  std::filesystem::path cp = std::filesystem::current_path();

  std::string file;
  std::string source;
  std::string header;
  std::string sourcePath;
  std::string path;
  std::string javaConverter;

  header = "header";
  source = "source";
  // file = "D:\\DEV\\CobaltSKY\\Engine\\cobalt\\entity\\csdynamiccolliderstate.hh";
  path = "C:\\Users\\MCEL\\CMakeBuilds\\d5ee80a5-d1ee-2937-85d1-86f3b75e756d\\build\\x64-Release\\src\\test";


  if (argc == 1)
  {
    print_usage(argv[0]);
    return 0;
  }

  for (int i = 1; i < argc; i++)
  {
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
    else if (arg == "--sourcepath")
    {
        if (i + 1 >= argc)
        {
            print_usage(argv[0]);
            return -1;
        }
        sourcePath = std::string(argv[++i]);
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
    else if (arg == "--javaConverter")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      javaConverter = std::string(argv[++i]);
      // std::cout << " path: '" << path << "'";
    }
    else
    {
      printf("Invalid arg %s\n", arg.c_str());
      print_usage(argv[0]);
      return -1;
    }
  }

  if (!javaConverter.empty())
  {
    JavaConverters::Get()->ReadConverters (javaConverter);
  }

  if (!file.empty() && !source.empty() && !header.empty())
  {
    generate(nullptr, file, header, source);
  }
  else if (!path.empty())
  {

    generate_list(path, sourcePath);
  }

  return 0;
}
