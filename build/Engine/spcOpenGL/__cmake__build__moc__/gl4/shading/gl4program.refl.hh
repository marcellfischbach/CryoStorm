#pragma once

namespace spc::opengl
{

class GL4ProgramClass : public spc::Class
{
public:
 GL4ProgramClass();
 static GL4ProgramClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

