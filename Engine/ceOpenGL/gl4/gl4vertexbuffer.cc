
#include <ceOpenGL/gl4/gl4vertexbuffer.hh>
#include <gl/glew.h>


namespace cryo::opengl
{

GL4VertexBuffer::GL4VertexBuffer()
  : iVertexBuffer()
  , m_size(0)
{
  CS_CLASS_GEN_CONSTR;
  glGenBuffers(1, &m_name);
}

GL4VertexBuffer::~GL4VertexBuffer()
{
  if (m_name != 0)
  {
    glDeleteBuffers(1, &m_name);
    m_name = 0;
  }
}

void GL4VertexBuffer::CreateForRendering(Size size, eBufferUsage usage)
{
  GLenum mode;
  switch (usage)
  {
  case eBU_Static: mode = GL_STATIC_DRAW; break;
  case eBU_Stream: mode = GL_STREAM_DRAW; break;
  case eBU_Dynamic: mode = GL_DYNAMIC_DRAW; break;
  }
  glBufferData(GL_ARRAY_BUFFER, size, 0, mode);
  m_size = size;
}

void GL4VertexBuffer::Bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, m_name);
}

void GL4VertexBuffer::Unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GL4VertexBuffer::Copy(const void* data, Size count, Size targetOffset)
{
  glBufferSubData(GL_ARRAY_BUFFER, targetOffset, count, data);
}

void GL4VertexBuffer::Map(void **data, Size &dataSize)
{
  Bind();
  *data = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
  dataSize = m_size;
}

void GL4VertexBuffer::Unmap()
{
  glUnmapBuffer(GL_ARRAY_BUFFER);
}

}