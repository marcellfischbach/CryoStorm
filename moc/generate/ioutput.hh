

#pragma once 

namespace spc::moc
{


struct iOutput
{
  virtual ~iOutput() { }

  virtual void output(const std::string& output) = 0;

};

}
