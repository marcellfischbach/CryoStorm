
#include <ceOpenGL/gl4/shading/gl4shadersourcefragment.hh>

namespace cryo::opengl
{

GL4ShaderSourceFragment::GL4ShaderSourceFragment()
{

}

void GL4ShaderSourceFragment::SetLines(const std::vector<std::string>&lines)
{
  m_lines = lines;
}

const std::vector<std::string>& GL4ShaderSourceFragment::GetLines() const
{
  return m_lines;
}


}