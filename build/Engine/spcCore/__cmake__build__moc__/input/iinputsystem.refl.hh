#pragma once

namespace spc
{

class iInputSystemClass : public spc::Class
{
public:
 iInputSystemClass();
 static iInputSystemClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

