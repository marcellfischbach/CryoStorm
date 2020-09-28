
#include <parser/sourcefile.hh>

#include <iostream>
#include <fstream>
#include <iomanip>



namespace cs::moc
{


SourceFile::SourceFile()
{

}


void SourceFile::Read(const std::string& fileName)
{
  std::ifstream stream(fileName);
  std::string line;
  int l = 1;
  State state = eS_Text;
  std::string fullLine = "";
  while (std::getline(stream, line))
  {
    if (!line.empty())
    {
      if (line[line.length() - 1] == '\\')
      {
        fullLine += line.substr(0, line.length() - 1);;
        continue;
      }
      else
      {
        fullLine += line;
      }
    }
    state = PutLine(state, l++, fullLine);
    fullLine = "";
  }

  /*
  for (size_t i = 0, in = m_lines.size(); i < in; ++i)
  {
    std::cout << std::setw(4) << std::setfill(' ') << (i + 1) << " " << m_lines[i] << std::endl;
  }
  */
}


SourceFile::State SourceFile::PutLine(State state, int lineNo, const std::string& line)
{
  std::string result = "";
  std::string tmp = line + " ";

  bool blockComment = state == eS_BlockComment;
  bool inString = false;
  for (size_t i = 0, in = line.size(); i < in; ++i)
  {
    const char& ch = tmp[i];
    const char& chpp = tmp[i + 1];

    if (blockComment)
    {
      if (ch == '*' && chpp == '/')
      {
        i++;
        blockComment = false;
        continue;
      }
    }
    else
    {
      if (ch == '"')
      {
        inString = !inString;
      }

      if (ch == '/' && !inString)
      {
        if (chpp == '/')
        {
          break;
        }
        else if (chpp == '*')
        {

          blockComment = true;
          continue;
        }
      }
      if (ch == '#')
      {
        break;
      }

      result += ch;
      if (ch == '\\')
      {
        result += chpp;
        i++;
      }
    }
  }

  if (!result.empty())
  {
    m_lines.push_back(result);
  }
  

  return blockComment ? eS_BlockComment : eS_Text;
}


const std::vector<std::string>& SourceFile::GetLines() const
{
  return m_lines;
}

SourceFileIterator SourceFile::GetIterator() const
{
  return SourceFileIterator(*this);
}



SourceFileIterator::SourceFileIterator(const SourceFile& sourceFile)
  : m_sourceFile(sourceFile)
  , m_lineNo(0)
  , m_idx(0)
{
  if (sourceFile.GetLines().empty())
  {
    m_line = "";
  }
  else
  {
    m_line = sourceFile.GetLines()[m_lineNo];
  }
}


bool SourceFileIterator::HasMore() const
{
  if (m_idx  < m_line.size())
  {
    return true;
  }

  const std::vector<std::string>& lines = m_sourceFile.GetLines();
  for (size_t i = m_lineNo+1, in = lines.size(); i < in; ++i)
  {
    if (!lines[i].empty())
    {
      return true;
    }
  }
  return false;
}

char SourceFileIterator::Next() 
{
  if (m_idx >= m_line.size())
  {
    m_idx = 0;
    m_line = "";
    const std::vector<std::string>& lines = m_sourceFile.GetLines();
    for (size_t i = m_lineNo + 1, in = lines.size(); i < in; ++i)
    {
      if (!lines[i].empty())
      {
        m_lineNo = i;
        m_line = lines[i];
        break;
      }
    }
  }

  if (m_idx < m_line.size())
  {
    return m_line[m_idx++];
  }
  return ' ';
}


}