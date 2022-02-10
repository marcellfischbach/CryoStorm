
#pragma once

#include <exception>
#include <string>

namespace ce::moc
{


class BaseException : public std::exception
{
public:
  BaseException(const std::string& file, unsigned line, const std::string &message)
    : std::exception()
    , m_file(file)
    , m_line(line)
    , m_message(message)
  {

  }

  const char* GetMessage() const
  {
    return m_message.c_str();
  }

  const char* GetFile() const
  {
    return m_file.c_str();
  }

  unsigned GetLine() const
  {
    return m_line;
  }

private:
  std::string m_file;
  unsigned m_line;
  std::string m_message;
};

class ParseException : public BaseException
{
public:
  ParseException(const std::string& file, unsigned line, const std::string& message = "")
    : BaseException (file, line, message)
  {

  }
};
}
