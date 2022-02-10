#include <ceOpenGL/gl4/shading/GL4ShaderAttributeNull.hh>


namespace ce::opengl
{

GL4ShaderAttributeNull::GL4ShaderAttributeNull( const std::string& name)
  : iShaderAttribute()
  , m_name(name)
{

}

const std::string& GL4ShaderAttributeNull::GetName() const 
{
  return m_name;
}

bool GL4ShaderAttributeNull::IsValid() const 
{
  return true;
}

void GL4ShaderAttributeNull::SetArrayIndex(Size idx)
{
}



void GL4ShaderAttributeNull::Bind(float x) 
{
}



void GL4ShaderAttributeNull::Bind(float x, float y) 
{
}



void GL4ShaderAttributeNull::Bind(float x, float y, float z) 
{
}



void GL4ShaderAttributeNull::Bind(float x, float y, float z, float w) 
{
}



void GL4ShaderAttributeNull::Bind(int x) 
{
}



void GL4ShaderAttributeNull::Bind(int x, int y) 
{
}



void GL4ShaderAttributeNull::Bind(int x, int y, int z) 
{
}



void GL4ShaderAttributeNull::Bind(int x, int y, int z, int w) 
{
}



void GL4ShaderAttributeNull::Bind(const Vector2f& v) 
{
}



void GL4ShaderAttributeNull::Bind(const Vector3f& v) 
{
}



void GL4ShaderAttributeNull::Bind(const Vector4f& v) 
{
}



void GL4ShaderAttributeNull::Bind(const Color4f& v) 
{
}



void GL4ShaderAttributeNull::Bind(const Matrix3f& v) 
{
}




void GL4ShaderAttributeNull::Bind(const Matrix4f& v) 
{
}


}