#pragma once

namespace spc::opengl
{

class GL4RenderTarget2DClass : public spc::Class
{
public:
 GL4RenderTarget2DClass();
 static GL4RenderTarget2DClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

