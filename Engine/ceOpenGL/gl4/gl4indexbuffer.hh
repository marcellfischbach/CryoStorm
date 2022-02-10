

#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/graphics/iindexbuffer.hh>

namespace ce::opengl
{
CE_CLASS()
class CE_OGL_API GL4IndexBuffer : public CE_SUPER(iIndexBuffer)
{
	CE_CLASS_GEN_OBJECT;
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