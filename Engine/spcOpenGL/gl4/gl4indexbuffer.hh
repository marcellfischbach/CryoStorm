

#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/iindexbuffer.hh>


namespace spc
{
SPC_CLASS()
class SPC_OGL_API GL4IndexBuffer : public SPC_SUPER(iIndexBuffer)
{
	SPC_CLASS_GEN_OBJECT;
public:
	GL4IndexBuffer();
	virtual ~GL4IndexBuffer();

	virtual void CreateForRendering(Size size, eBufferUsage usage = eBU_Static);

	virtual void Bind();

	virtual void Unbind();

	virtual void Copy(const void* data, Size count, Size targetOffset = 0);

private:
	UInt32 m_name;

};

}