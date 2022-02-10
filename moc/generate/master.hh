

#pragma once

#include <string>

namespace ce::moc
{

class Cache;
struct iOutput;
class MasterGenerator
{
public:

  MasterGenerator();

  void Generate(Cache &cache,  iOutput *output);
};

}

