

#pragma once 

namespace Spice::moc
{


struct iOutput
{
  virtual ~iOutput() { }

  virtual void output(const std::string& output) = 0;

};

}
