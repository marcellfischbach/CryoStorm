#include <spcOpenGL/gl4/shading/gl4shaderattribute.hh>


namespace spc::opengl
{

GL4ShaderAttribute::GL4ShaderAttribute(GLint location, const std::string& name)
  : iShaderAttribute()
  , m_location(location)
  , m_arrayIndex(0)
  , m_absLocation(location)
  , m_name(name)
{

}

const std::string& GL4ShaderAttribute::GetName() const 
{
  return m_name;
}

bool GL4ShaderAttribute::IsValid() const 
{
  return true;
}

void GL4ShaderAttribute::SetArrayIndex(UInt16 idx) 
{
  m_arrayIndex = idx;
  m_absLocation = m_location + m_arrayIndex;
}



void GL4ShaderAttribute::Bind(float x) 
{
  glUniform1f(m_absLocation, x);
}



void GL4ShaderAttribute::Bind(float x, float y) 
{
  glUniform2f(m_absLocation, x, y);
}



void GL4ShaderAttribute::Bind(float x, float y, float z) 
{
  glUniform3f(m_absLocation, x, y, z);
}



void GL4ShaderAttribute::Bind(float x, float y, float z, float w) 
{
  glUniform4f(m_absLocation, x, y, z, w);
}



void GL4ShaderAttribute::Bind(int x) 
{
  glUniform1i(m_absLocation, x);
}



void GL4ShaderAttribute::Bind(int x, int y) 
{
  glUniform2i(m_absLocation, x, y);
}



void GL4ShaderAttribute::Bind(int x, int y, int z) 
{
  glUniform3i(m_absLocation, x, y, z);
}



void GL4ShaderAttribute::Bind(int x, int y, int z, int w) 
{
  glUniform4i(m_absLocation, x, y, z, w);
}



void GL4ShaderAttribute::Bind(const Vector2f& v) 
{
  glUniform2fv(m_absLocation, 1, &v.x);
}



void GL4ShaderAttribute::Bind(const Vector3f& v) 
{
  glUniform3fv(m_absLocation, 1, &v.x);
}



void GL4ShaderAttribute::Bind(const Vector4f& v) 
{
  glUniform4fv(m_absLocation, 1, &v.x);
}



void GL4ShaderAttribute::Bind(const Color4f& v) 
{
  glUniform4fv(m_absLocation, 1, &v.r);
}



void GL4ShaderAttribute::Bind(const Matrix3f& v) 
{
  glUniformMatrix3fv(m_absLocation, 1, false, &v.m00);
}




void GL4ShaderAttribute::Bind(const Matrix4f& v) 
{
  glUniformMatrix4fv(m_absLocation, 1, false, &v.m00);
}


}