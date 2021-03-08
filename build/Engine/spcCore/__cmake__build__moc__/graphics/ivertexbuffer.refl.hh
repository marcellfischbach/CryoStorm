#pragma once

namespace spc
{

class iVertexBufferClass : public spc::Class
{
public:
 iVertexBufferClass();
 static iVertexBufferClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

