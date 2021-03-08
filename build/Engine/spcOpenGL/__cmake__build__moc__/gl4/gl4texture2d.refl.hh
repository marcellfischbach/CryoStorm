#pragma once

namespace spc::opengl
{

class GL4Texture2DClass : public spc::Class
{
public:
 GL4Texture2DClass();
 static GL4Texture2DClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

