

#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/graphics/iIndexBuffer.hh>

namespace cs::opengl
{
CS_CLASS()
class CS_OGL_API csGL4IndexBuffer : public CS_SUPER(iIndexBuffer)
{
  CS_CLASS_GEN_OBJECT;
public:
	csGL4IndexBuffer();
	~csGL4IndexBuffer() override;

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