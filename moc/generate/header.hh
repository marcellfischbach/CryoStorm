
#pragma once

#include <generate/generator.hh>

namespace Spice::moc
{

struct iOutput;
class ClassNode;
class HeaderGenerator : public Generator
{
public:
  HeaderGenerator();

  void Output(const std::string& exp, iOutput *output);

private:
  std::string OutputClass(ClassNode* classNode, const std::string & exp);
};
}
