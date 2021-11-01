

#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/iindexbuffer.hh>

namespace spc::opengl
{
SPC_CLASS()
class SPC_OGL_API GL4IndexBuffer : public SPC_SUPER(iIndexBuffer)
{
	SPC_CLASS_GEN_OBJECT;
public:
	GL4IndexBuffer();
	~GL4IndexBuffer() override;

	void CreateForRendering(Size size, eBufferUsage usage = eBU_Static) override;

	void Bind() override;

	void Unbind() override;

  void Map(void **data, Size &dataSize) override;

  void Unmap() override;

	void Copy(const void* data, Size count, Size targetOffset = 0);

private:
	uint32_t m_name;
	Size m_size;

};

}