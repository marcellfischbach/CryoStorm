#pragma once

namespace spc
{

class iIndexBufferClass : public spc::Class
{
public:
 iIndexBufferClass();
 static iIndexBufferClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

