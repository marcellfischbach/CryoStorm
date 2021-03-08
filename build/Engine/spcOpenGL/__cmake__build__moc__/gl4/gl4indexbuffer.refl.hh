#pragma once

namespace spc::opengl
{

class GL4IndexBufferClass : public spc::Class
{
public:
 GL4IndexBufferClass();
 static GL4IndexBufferClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

