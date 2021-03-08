#pragma once

namespace spc::opengl
{

class GL4RenderTargetCubeClass : public spc::Class
{
public:
 GL4RenderTargetCubeClass();
 static GL4RenderTargetCubeClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

