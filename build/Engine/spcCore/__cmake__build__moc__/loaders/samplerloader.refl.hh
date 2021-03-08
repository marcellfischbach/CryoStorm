#pragma once

namespace spc
{

class SamplerLoaderSpcClass : public spc::Class
{
public:
 SamplerLoaderSpcClass();
 static SamplerLoaderSpcClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

