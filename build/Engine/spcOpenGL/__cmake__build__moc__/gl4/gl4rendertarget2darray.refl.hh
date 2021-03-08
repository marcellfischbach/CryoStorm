#pragma once

namespace spc::opengl
{

class GL4RenderTarget2DArrayClass : public spc::Class
{
public:
 GL4RenderTarget2DArrayClass();
 static GL4RenderTarget2DArrayClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

