
#include <ceOpenGL/gl4/gl4exceptions.hh>

namespace ce::opengl
{

GL4ShaderCompileException::GL4ShaderCompileException(const std::string &message, const std::string &source)
  : std::exception()
  , m_source(source)
  , m_message(message)
{

}


const char* GL4ShaderCompileException::what() const noexcept
{
    return m_message.c_str();
}


const std::string& GL4ShaderCompileException::GetSource() const
{
  return m_source;
}

GL4ProgramLinkException::GL4ProgramLinkException(const std::string& message)
  : std::exception()
        , m_message(message)
{

}


const char* GL4ProgramLinkException::what() const noexcept
{
    return m_message.c_str();
}



}