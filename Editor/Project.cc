//
// Created by MCEL on 29.11.2024.
//

#include <Project.hh>
#include <csXml/csXml.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/csFileSystemArchive.hh>


using namespace cs;

Project *Project::Get()
{
  static Project project;
  return &project;
}

bool read_vfs_alias(const xml::csElement *aliasElement)
{
  if (!aliasElement->HasAttribute("name"))
  {
    fprintf(stderr, "<vfs><aliases><alias> has no name.\n");
    return false;
  }

  const std::string &name = aliasElement->GetAttribute("name");
  const std::string &replacement = aliasElement->GetContent();


  printf("Add alias ${%s} -> '%s'\n", name.c_str(), replacement.c_str());
  csVFS::Get()->InsertAlias(name, replacement);
  return true;
}

bool read_vfs_aliases(const xml::csElement *vfsElement)
{
  const xml::csElement *aliasesElement = vfsElement->FindElement("aliases");
  if (!aliasesElement)
  {
    // aliases are optional
    return true;
  }

  for (int i = 0; i < aliasesElement->GetNumberOfChildren(); ++i)
  {
    const xml::csElement *aliasElement = aliasesElement->GetChild(i)->AsElement();
    if (!aliasElement || aliasElement->GetTagName() != "alias")
    {
      continue;
    }

    if (!read_vfs_alias(aliasElement))
    {
      return false;
    }
  }

  return true;
}

bool read_vfs_archive(const xml::csElement *archiveElement)
{

  int prio = 0;
  if (archiveElement->HasAttribute("prio"))
  {
    prio = atoi(archiveElement->GetAttribute("prio").c_str());
  }

  if (!archiveElement->HasAttribute("name"))
  {
    fprintf(stderr, "No <vfs><archives><archive> has not name attribute.\n");
    return false;
  }

  std::string name = archiveElement->GetAttribute("name");
  std::string path = archiveElement->GetContent();

  printf("Add archive '%s' [%d] -> '%s'\n", name.c_str(), prio, path.c_str());

  auto fsArchive = new csFileSystemArchive(name, path, prio);
  csVFS::Get()->AddArchive(fsArchive);
  return true;
}

bool read_vfs_archives(const xml::csElement *vfsElement)
{
  const xml::csElement *archivesElement = vfsElement->FindElement("archives");
  if (!archivesElement)
  {
    fprintf(stderr, "No <vfs><archives> specified.\n");
    return false;
  }

  for (int i = 0; i < archivesElement->GetNumberOfChildren(); ++i)
  {
    const xml::csElement *childElement = archivesElement->GetChild(i)->AsElement();
    if (!childElement || childElement->GetTagName() != "archive")
    {
      continue;
    }

    if (!read_vfs_archive(childElement))
    {
      return false;
    }
  }
  return true;
}

bool is_absolute_path (const std::string &rootPath)
{
#ifdef WIN32
  if (rootPath.size() > 2 && rootPath[1] == ':' && (rootPath[2] == '\\' || rootPath[2] == '/'))
  {
    return true;
  }
#else
  if (!rootPath.empty() && rootPath[0] == '/')
  {
    return true;
  }
#endif
  return false;
}

void set_root_path(const std::string &rootPath, const std::string &projectFolder)
{
  if (is_absolute_path(rootPath))
  {
    csVFS::Get()->SetRootPath(rootPath);
  }
  csVFS::Get()->SetRootPath(projectFolder + "/" + rootPath);
}

bool read_vfs(const xml::csElement *rootElement, const std::string &projectFolder)
{
  const xml::csElement *vfsElement = rootElement->FindElement("vfs");
  if (!vfsElement)
  {
    fprintf(stderr, "No <vfs> specified.\n");
    return false;
  }

  const xml::csElement *root = vfsElement->FindElement("root");
  if (!root)
  {
    fprintf(stderr, "No <vfs><root> specified.\n");
    return false;
  }

  const std::string &rootPath = root->GetContent();
  printf("Open project. Set root: '%s'\n", rootPath.c_str());
  set_root_path(rootPath, projectFolder);

  if (!read_vfs_archives(vfsElement))
  {
    return false;
  }
  if (!read_vfs_aliases(vfsElement))
  {
    return false;
  }

  return true;
}


bool open_doc(xml::csDocument *doc, const std::string &projectFolder)
{
  if (!doc)
  {
    return false;
  }

  const xml::csElement *rootElement = doc->GetRoot();
  if (rootElement->GetTagName() != std::string("CryoProject"))
  {
    fprintf(stderr, "Malformed project: Expected tag <CryoProject> but got <%s>\n", rootElement->GetTagName().c_str());
    return false;
  }

  if (!read_vfs(rootElement, projectFolder))
  {
    return false;
  }
  return true;
}

bool Project::Open(const std::string &projectFolder)
{
  xml::csDocument *doc = xml::csParser::ParseFilename(projectFolder + "/cryo-project.xml");
  if (!open_doc(doc, projectFolder))
  {
    return false;
  }


  return true;
}