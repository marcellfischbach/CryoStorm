
#include <csOpenGL/gl4/shading/csGL4ShaderSourceFragment.hh>

namespace cryo::opengl
{

csGL4ShaderSourceFragment::csGL4ShaderSourceFragment()
{

}

void csGL4ShaderSourceFragment::SetLines(const std::vector<std::string>&lines)
{
  m_lines = lines;
}

const std::vector<std::string>& csGL4ShaderSourceFragment::GetLines() const
{
  return m_lines;
}


}