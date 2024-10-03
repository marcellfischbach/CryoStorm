
#pragma once

#include <generate/generator.hh>

namespace cryo::moc
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
