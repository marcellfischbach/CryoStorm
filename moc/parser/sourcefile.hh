

#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace cs::moc
{

struct SourceLine {
  std::string line;
  uint32_t no;
};

class SourceFile;
class SourceFileIterator
{

public:
  SourceFileIterator(const SourceFile& sourceFile);

  bool HasMore() const;
  char Next();

  uint32_t GetLineNo () const;
  uint32_t GetColumNo () const;
private:
  const SourceFile& m_sourceFile;

  size_t m_lineNo;
  SourceLine m_line;
  size_t m_idx;
};



class SourceFile
{
public:

  SourceFile();

  SourceFileIterator GetIterator()const;


  void Read(const std::string& fileName);

  const std::vector<SourceLine>& GetLines() const;

private:
  enum State
  {
    eS_Text,
    eS_BlockComment,
  };

  State PutLine(State state, int lineNo, const std::string& line);




  std::vector<SourceLine> m_lines;
};


}
