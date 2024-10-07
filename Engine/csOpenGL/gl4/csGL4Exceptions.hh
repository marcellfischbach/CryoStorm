#pragma  once

#include <exception>
#include <string>

namespace cs::opengl
{


class csGL4ShaderCompileException : public std::exception
{
public:
  csGL4ShaderCompileException(const std::string& errorMessage, const std::string& source);

  const std::string& GetSource() const;

  const char* what() const noexcept override;

private:
  std::string m_source;
  std::string m_message;
};

class csGL4ProgramLinkException : public std::exception
{
public:
  csGL4ProgramLinkException(const std::string& errorMessage);
  const char* what() const noexcept override;

private:
    std::string m_message;

};



}