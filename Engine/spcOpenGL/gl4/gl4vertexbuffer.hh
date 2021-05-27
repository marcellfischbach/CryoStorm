

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
	~GL4VertexBuffer() override;

	void CreateForRendering(Size size, eBufferUsage usage = eBU_Static) override;

	void Bind() override;

	void Unbind() override;

  void Map(void **data, Size &dataSize) override;

  void Unmap() override;

  void Copy(const void* data, Size count, Size targetOffset = 0) override;

private:
	UInt32 m_name;
	Size m_size;
};

}