
#include <spcOpenGL/gl4/gl4exceptions.hh>

namespace spc
{

GL4ShaderCompileException::GL4ShaderCompileException(const std::string &message, const std::string &source)
  : std::exception(message.c_str())
  , m_source(source)
{

}


const std::string& GL4ShaderCompileException::GetSource() const
{
  return m_source;
}

GL4ProgramLinkException::GL4ProgramLinkException(const std::string& message)
  : std::exception(message.c_str())
{

}



}