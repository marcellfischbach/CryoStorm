
#pragma once

#include <ceCore/csClass.hh>
#include <string>
#include <vector>

namespace cryo::opengl
{


CS_CLASS()
class csGL4ShaderSourceFragment : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  csGL4ShaderSourceFragment();
  ~csGL4ShaderSourceFragment() override = default;

  void SetLines(const std::vector<std::string> &lines);
  CS_NODISCARD const std::vector<std::string>& GetLines() const;

private:
  std::vector<std::string> m_lines;
};



}