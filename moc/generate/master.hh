

#pragma once

#include <string>

namespace spc::moc
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

