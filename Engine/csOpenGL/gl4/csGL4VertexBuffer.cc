
#include <csOpenGL/gl4/csGL4VertexBuffer.hh>
#include <csOpenGL/csGLError.hh>
#include <gl/glew.h>


namespace cs::opengl
{

csGL4VertexBuffer::csGL4VertexBuffer()
  : iVertexBuffer()
  , m_size(0)
{

  CS_GL_ERROR()
  glGenBuffers(1, &m_name);
  CS_GL_ERROR()
}

csGL4VertexBuffer::~csGL4VertexBuffer()
{
  if (m_name != 0)
  {
    CS_GL_ERROR()
    glDeleteBuffers(1, &m_name);
    CS_GL_ERROR()
    m_name = 0;
  }
}

void csGL4VertexBuffer::CreateForRendering(Size size, eBufferUsage usage)
{
  GLenum mode;
  switch (usage)
  {
  case eBU_Static: mode = GL_STATIC_DRAW; break;
  case eBU_Stream: mode = GL_STREAM_DRAW; break;
  case eBU_Dynamic: mode = GL_DYNAMIC_DRAW; break;
  }
  CS_GL_ERROR()
  glBufferData(GL_ARRAY_BUFFER, size, 0, mode);
  CS_GL_ERROR()
  m_size = size;
}

void csGL4VertexBuffer::Bind()
{
  CS_GL_ERROR()
  glBindBuffer(GL_ARRAY_BUFFER, m_name);
  CS_GL_ERROR()
}

void csGL4VertexBuffer::Unbind()
{
  CS_GL_ERROR()
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  CS_GL_ERROR()
}

void csGL4VertexBuffer::Copy(const void* data, Size count, Size targetOffset)
{
  CS_GL_ERROR()
  glBufferSubData(GL_ARRAY_BUFFER, targetOffset, count, data);
  CS_GL_ERROR()
}

void csGL4VertexBuffer::Map(void **data, Size &dataSize)
{
  Bind();
  CS_GL_ERROR()
  *data = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
  CS_GL_ERROR()
  dataSize = m_size;
}

void csGL4VertexBuffer::Unmap()
{
  CS_GL_ERROR()
  glUnmapBuffer(GL_ARRAY_BUFFER);
  CS_GL_ERROR()
}

}