#pragma once

namespace spc
{

class iFileClass : public spc::Class
{
public:
 iFileClass();
 static iFileClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

