#include <ceOpenGL/gl4/shading/GL4ShaderAttributeNull.hh>


namespace cryo::opengl
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



void GL4ShaderAttributeNull::Bind(const csVector2f& v)
{
}



void GL4ShaderAttributeNull::Bind(const csVector3f& v)
{
}



void GL4ShaderAttributeNull::Bind(const csVector4f& v)
{
}



void GL4ShaderAttributeNull::Bind(const csColor4f& v)
{
}



void GL4ShaderAttributeNull::Bind(const csMatrix3f& v)
{
}




void GL4ShaderAttributeNull::Bind(const csMatrix4f& v)
{
}


}