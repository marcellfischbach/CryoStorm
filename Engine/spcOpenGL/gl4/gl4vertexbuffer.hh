

#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/ivertexbuffer.hh>


namespace spc::opengl
{
SPC_CLASS()
class SPC_OGL_API GL4VertexBuffer : public SPC_SUPER(iVertexBuffer)
{
	SPC_CLASS_GEN_OBJECT;
public:
	GL4VertexBuffer();
	virtual ~GL4VertexBuffer();

	virtual void CreateForRendering(Size size, eBufferUsage usage = eBU_Static);

	virtual void Bind();

	virtual void Unbind();

	virtual void Copy(const void* data, Size count, Size targetOffset = 0);

private:
	UInt32 m_name;
};

}