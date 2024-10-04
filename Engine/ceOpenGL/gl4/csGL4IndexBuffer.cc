
#include <ceOpenGL/gl4/csGL4IndexBuffer.hh>
#include <gl/glew.h>


namespace cryo::opengl
{

csGL4IndexBuffer::csGL4IndexBuffer()
  : iIndexBuffer()
  , m_size(0)
{
  CS_CLASS_GEN_CONSTR;
  glGenBuffers(1, &m_name);
}

csGL4IndexBuffer::~csGL4IndexBuffer()
{
  if (m_name != 0)
  {
    glDeleteBuffers(1, &m_name);
    m_name = 0;
  }
}

void csGL4IndexBuffer::CreateForRendering(Size size, eBufferUsage usage)
{
  GLenum mode;
  switch (usage)
  {
  case eBU_Static: mode = GL_STATIC_DRAW; break;
  case eBU_Stream: mode = GL_STREAM_DRAW; break;
  case eBU_Dynamic: mode = GL_DYNAMIC_DRAW; break;
  }
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, mode);
  m_size = size;
}

void csGL4IndexBuffer::Bind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_name);
}

void csGL4IndexBuffer::Unbind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void csGL4IndexBuffer::Copy(const void* data, Size count, Size targetOffset)
{
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, targetOffset, count, data);
}

void csGL4IndexBuffer::Map(void **data, Size &dataSize)
{
  Bind();
  *data = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
  dataSize = m_size;
}

void csGL4IndexBuffer::Unmap()
{
  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

}