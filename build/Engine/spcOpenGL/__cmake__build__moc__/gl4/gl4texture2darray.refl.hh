#pragma once

namespace spc::opengl
{

class GL4Texture2DArrayClass : public spc::Class
{
public:
 GL4Texture2DArrayClass();
 static GL4Texture2DArrayClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

