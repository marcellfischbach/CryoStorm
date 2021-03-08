#pragma once

namespace spc
{

class SamplersClass : public spc::Class
{
public:
 SamplersClass();
 static SamplersClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

