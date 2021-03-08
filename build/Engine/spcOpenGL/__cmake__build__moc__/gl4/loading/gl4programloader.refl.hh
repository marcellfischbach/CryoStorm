#pragma once

namespace spc::opengl
{

class GL4ProgramLoaderClass : public spc::Class
{
public:
 GL4ProgramLoaderClass();
 static GL4ProgramLoaderClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

