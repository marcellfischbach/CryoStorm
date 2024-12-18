#include <csOpenGL/gl4/shading/csGL4ShaderAttribute.hh>
#include <csOpenGL/csGLError.hh>

namespace cs::opengl
{

csGL4ShaderAttribute::csGL4ShaderAttribute(GLint location, const std::string& name)
  : iShaderAttribute()
  , m_location(location)
  , m_arrayIndex(0)
  , m_absLocation(location)
  , m_name(name)
{

}

const std::string& csGL4ShaderAttribute::GetName() const
{
  return m_name;
}

bool csGL4ShaderAttribute::IsValid() const
{
  return true;
}

void csGL4ShaderAttribute::SetArrayIndex(Size idx)
{
  m_arrayIndex = (GLint)idx;
  m_absLocation = m_location + m_arrayIndex;
}



void csGL4ShaderAttribute::Bind(float x)
{
  CS_GL_ERROR();
  glUniform1f(m_absLocation, x);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(float x, float y)
{
  CS_GL_ERROR();
  glUniform2f(m_absLocation, x, y);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(float x, float y, float z)
{
  CS_GL_ERROR();
  glUniform3f(m_absLocation, x, y, z);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(float x, float y, float z, float w)
{
  CS_GL_ERROR();
  glUniform4f(m_absLocation, x, y, z, w);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(int x)
{
  CS_GL_ERROR();
  glUniform1i(m_absLocation, x);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(int x, int y)
{
  CS_GL_ERROR();
  glUniform2i(m_absLocation, x, y);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(int x, int y, int z)
{
  CS_GL_ERROR();
  glUniform3i(m_absLocation, x, y, z);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(int x, int y, int z, int w)
{
  CS_GL_ERROR();
  glUniform4i(m_absLocation, x, y, z, w);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(const csVector2f& v)
{
  CS_GL_ERROR();
  glUniform2fv(m_absLocation, 1, &v.x);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(const csVector3f& v)
{
  CS_GL_ERROR();
  glUniform3fv(m_absLocation, 1, &v.x);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(const csVector4f& v)
{
  CS_GL_ERROR();
  glUniform4fv(m_absLocation, 1, &v.x);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(const csColor4f& v)
{
  CS_GL_ERROR();
  glUniform4fv(m_absLocation, 1, &v.r);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(const csMatrix3f& v)
{
  CS_GL_ERROR();
  glUniformMatrix3fv(m_absLocation, 1, false, &v.m00);
  CS_GL_ERROR();
}

void csGL4ShaderAttribute::Bind(const csMatrix3f* v, Size count)
{
  CS_GL_ERROR();
  glUniformMatrix3fv(m_absLocation, (GLsizei)count, false, &v->m00);
  CS_GL_ERROR();
}



void csGL4ShaderAttribute::Bind(const csMatrix4f& v)
{
  CS_GL_ERROR();
  glUniformMatrix4fv(m_absLocation, 1, false, &v.m00);
  CS_GL_ERROR();
}

void csGL4ShaderAttribute::Bind(const csMatrix4f* v, Size count)
{
  CS_GL_ERROR();
  glUniformMatrix4fv(m_absLocation, (GLsizei)count, false, &v->m00);
  CS_GL_ERROR();
}

void csGL4ShaderAttribute::Bind(eTextureUnit unit)
{
  CS_GL_ERROR();
  glUniform1i(m_absLocation, unit);
  CS_GL_ERROR();
}


}