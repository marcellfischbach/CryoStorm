
#pragma once

#include <exception>
#include <string>

namespace ce::moc
{


class BaseException : public std::exception
{
public:
  BaseException(const std::string &file, unsigned line, const std::string &message, uint32_t sourceLine,
                uint32_t sourceColumn)
      : std::exception()
      , m_file(file)
      , m_line(line)
      , m_message(message)
      , m_sourceLine(sourceLine)
      , m_sourceColumn(sourceColumn)
  {

  }

  const char *GetMessage() const
  {
    return m_message.c_str();
  }

  const char *GetFile() const
  {
    return m_file.c_str();
  }

  unsigned GetLine() const
  {
    return m_line;
  }

  uint32_t getSourceLine() const
  {
    return m_sourceLine;
  }
  uint32_t getSourceColumn() const
  {
    return m_sourceColumn;
  }

private:
  std::string m_file;
  unsigned m_line;
  std::string m_message;

  uint32_t m_sourceLine;
  uint32_t m_sourceColumn;
};

class ParseException : public BaseException
{
public:
  ParseException(const std::string &file, unsigned line, const std::string &message, uint32_t sourceLine,
                 uint32_t sourceColumn)
      : BaseException(file, line, message, sourceLine, sourceColumn)
  {

  }
};
}
