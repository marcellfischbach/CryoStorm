#pragma  once

#include <exception>
#include <string>

namespace spc::opengl
{


class GL4ShaderCompileException : public std::exception
{
public:
  GL4ShaderCompileException(const std::string& errorMessage, const std::string& source);

  const std::string& GetSource() const;

private:
  std::string m_source;
};

class GL4ProgramLinkException : public std::exception
{
public:
  GL4ProgramLinkException(const std::string& errorMessage);


};



}