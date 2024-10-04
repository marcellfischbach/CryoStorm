#include <csOpenGL/gl4/shading/csGL4ShaderAttribute.hh>
#include <csOpenGL/csGLError.hh>

namespace cryo::opengl
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
  glUniform1f(m_absLocation, x);
}



void csGL4ShaderAttribute::Bind(float x, float y)
{
  glUniform2f(m_absLocation, x, y);
}



void csGL4ShaderAttribute::Bind(float x, float y, float z)
{
  glUniform3f(m_absLocation, x, y, z);
}



void csGL4ShaderAttribute::Bind(float x, float y, float z, float w)
{
  glUniform4f(m_absLocation, x, y, z, w);
}



void csGL4ShaderAttribute::Bind(int x)
{
  glUniform1i(m_absLocation, x);
}



void csGL4ShaderAttribute::Bind(int x, int y)
{
  glUniform2i(m_absLocation, x, y);
}



void csGL4ShaderAttribute::Bind(int x, int y, int z)
{
  glUniform3i(m_absLocation, x, y, z);
}



void csGL4ShaderAttribute::Bind(int x, int y, int z, int w)
{
  glUniform4i(m_absLocation, x, y, z, w);
}



void csGL4ShaderAttribute::Bind(const csVector2f& v)
{
  glUniform2fv(m_absLocation, 1, &v.x);
}



void csGL4ShaderAttribute::Bind(const csVector3f& v)
{
  glUniform3fv(m_absLocation, 1, &v.x);
}



void csGL4ShaderAttribute::Bind(const csVector4f& v)
{
  glUniform4fv(m_absLocation, 1, &v.x);
}



void csGL4ShaderAttribute::Bind(const csColor4f& v)
{
  glUniform4fv(m_absLocation, 1, &v.r);
}



void csGL4ShaderAttribute::Bind(const csMatrix3f& v)
{
  glUniformMatrix3fv(m_absLocation, 1, false, &v.m00);
}

void csGL4ShaderAttribute::Bind(const csMatrix3f* v, Size count)
{
  glUniformMatrix3fv(m_absLocation, (GLsizei)count, false, &v->m00);
}



void csGL4ShaderAttribute::Bind(const csMatrix4f& v)
{
  glUniformMatrix4fv(m_absLocation, 1, false, &v.m00);
}

void csGL4ShaderAttribute::Bind(const csMatrix4f* v, Size count)
{
  glUniformMatrix4fv(m_absLocation, (GLsizei)count, false, &v->m00);
}

}