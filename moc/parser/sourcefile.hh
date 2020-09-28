

#pragma once

#include <string>
#include <vector>

namespace cs::moc
{

class SourceFile;
class SourceFileIterator
{

public:
  SourceFileIterator(const SourceFile& sourceFile);

  bool HasMore() const;
  char Next();

private:
  const SourceFile& m_sourceFile;

  size_t m_lineNo;
  std::string m_line;
  size_t m_idx;
};



class SourceFile
{
public:
  SourceFile();

  SourceFileIterator GetIterator()const;


  void Read(const std::string& fileName);

  const std::vector<std::string>& GetLines() const;

private:
  enum State
  {
    eS_Text,
    eS_BlockComment,
  };

  State PutLine(State state, int lineNo, const std::string& line);



  std::vector<std::string> m_lines;
};


}