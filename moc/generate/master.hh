

#pragma once

#include <string>

namespace cs::moc
{

class Cache;
struct iOutput;
class MasterGenerator
{
public:

  MasterGenerator();

  void Generate(Cache &cache, const std::string &prefix, iOutput *output);
};

}

