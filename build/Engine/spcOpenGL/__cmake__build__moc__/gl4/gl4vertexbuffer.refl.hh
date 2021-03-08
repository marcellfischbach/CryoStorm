#pragma once

namespace spc::opengl
{

class GL4VertexBufferClass : public spc::Class
{
public:
 GL4VertexBufferClass();
 static GL4VertexBufferClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

