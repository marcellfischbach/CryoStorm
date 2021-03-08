#pragma once

namespace spc::opengl
{

class GL4SamplerClass : public spc::Class
{
public:
 GL4SamplerClass();
 static GL4SamplerClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

