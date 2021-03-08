#pragma once

namespace spc::opengl
{

class GL4RenderMeshClass : public spc::Class
{
public:
 GL4RenderMeshClass();
 static GL4RenderMeshClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

#pragma once

namespace spc::opengl
{

class GL4RenderMeshGeneratorClass : public spc::Class
{
public:
 GL4RenderMeshGeneratorClass();
 static GL4RenderMeshGeneratorClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

#pragma once

namespace spc::opengl
{

class GL4RenderMeshGeneratorFactoryClass : public spc::Class
{
public:
 GL4RenderMeshGeneratorFactoryClass();
 static GL4RenderMeshGeneratorFactoryClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

