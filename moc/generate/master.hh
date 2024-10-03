

#pragma once

#include <string>

namespace cryo::moc
{

class Cache;
struct iOutput;
class MasterGenerator
{
public:

  MasterGenerator();

  void Generate(Cache &cache,  iOutput *output, const std::string &sourcePath);
};

}

