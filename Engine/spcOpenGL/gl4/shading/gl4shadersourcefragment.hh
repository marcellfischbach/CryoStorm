
#pragma once

#include <spcCore/class.hh>
#include <string>
#include <vector>

namespace spc::opengl
{


SPC_CLASS()
class GL4ShaderSourceFragment :  public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4ShaderSourceFragment();
  ~GL4ShaderSourceFragment() override = default;

  void SetLines(const std::vector<std::string> &lines);
  SPC_NODISCARD const std::vector<std::string>& GetLines() const;

private:
  std::vector<std::string> m_lines;
};



}