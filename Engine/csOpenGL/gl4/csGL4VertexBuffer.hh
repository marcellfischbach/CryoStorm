

#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/graphics/iVertexBuffer.hh>


namespace cryo::opengl
{
CS_CLASS()
class CS_OGL_API csGL4VertexBuffer : public CS_SUPER(iVertexBuffer)
{
  CS_CLASS_GEN_OBJECT;
public:
	csGL4VertexBuffer();
	~csGL4VertexBuffer() override;

	void CreateForRendering(Size size, eBufferUsage usage = eBU_Static) override;

	void Bind() override;

	void Unbind() override;

  void Map(void **data, Size &dataSize) override;

  void Unmap() override;

  void Copy(const void* data, Size count, Size targetOffset = 0) override;

private:
	uint32_t m_name;
	Size m_size;
};

}