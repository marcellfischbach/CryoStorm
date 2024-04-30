#pragma once

namespace ce
{

class Engine;
struct iModule
{

  virtual bool Register(int argc, char** argv, Engine* engine) = 0;
  virtual bool Initialize(int argc, char** argv, Engine* engine) = 0;

};


}