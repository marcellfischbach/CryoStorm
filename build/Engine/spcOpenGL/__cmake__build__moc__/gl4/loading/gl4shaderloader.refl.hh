#pragma once

namespace spc::opengl
{

class GL4ShaderLoaderSpcClass : public spc::Class
{
public:
 GL4ShaderLoaderSpcClass();
 static GL4ShaderLoaderSpcClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

#pragma once

namespace spc::opengl
{

class GL4ShaderLoaderClass : public spc::Class
{
public:
 GL4ShaderLoaderClass();
 static GL4ShaderLoaderClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

