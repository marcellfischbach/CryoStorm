#pragma  once

#include <exception>
#include <string>

namespace ce::opengl
{


class GL4ShaderCompileException : public std::exception
{
public:
  GL4ShaderCompileException(const std::string& errorMessage, const std::string& source);

  const std::string& GetSource() const;

  const char* what() const noexcept override;

private:
  std::string m_source;
  std::string m_message;
};

class GL4ProgramLinkException : public std::exception
{
public:
  GL4ProgramLinkException(const std::string& errorMessage);
  const char* what() const noexcept override;

private:
    std::string m_message;

};



}