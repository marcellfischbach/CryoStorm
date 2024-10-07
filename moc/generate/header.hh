
#pragma once

#include <generate/generator.hh>

namespace cs::moc
{

struct iOutput;
class ClassNode;
class HeaderGenerator : public Generator
{
public:
  HeaderGenerator();

  void Output(iOutput *output);

private:
  std::string OutputClass(ClassNode* classNode);
};
}
