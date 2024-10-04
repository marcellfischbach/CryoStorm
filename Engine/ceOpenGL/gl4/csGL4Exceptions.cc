
#include <ceOpenGL/gl4/csGL4Exceptions.hh>

namespace cryo::opengl
{

csGL4ShaderCompileException::csGL4ShaderCompileException(const std::string &message, const std::string &source)
  : std::exception()
  , m_source(source)
  , m_message(message)
{

}


const char* csGL4ShaderCompileException::what() const noexcept
{
    return m_message.c_str();
}


const std::string& csGL4ShaderCompileException::GetSource() const
{
  return m_source;
}

csGL4ProgramLinkException::csGL4ProgramLinkException(const std::string& message)
  : std::exception()
        , m_message(message)
{

}


const char* csGL4ProgramLinkException::what() const noexcept
{
    return m_message.c_str();
}



}