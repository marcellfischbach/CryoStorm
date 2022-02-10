
#pragma once

#include <ceCore/class.hh>
#include <string>
#include <vector>

namespace ce::opengl
{


CE_CLASS()
class GL4ShaderSourceFragment :  public CE_SUPER(iObject)
{
  CE_CLASS_GEN_OBJECT;
public:
  GL4ShaderSourceFragment();
  ~GL4ShaderSourceFragment() override = default;

  void SetLines(const std::vector<std::string> &lines);
  CE_NODISCARD const std::vector<std::string>& GetLines() const;

private:
  std::vector<std::string> m_lines;
};



}