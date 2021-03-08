#pragma once

namespace spc
{

class iWindowClass : public spc::Class
{
public:
 iWindowClass();
 static iWindowClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

