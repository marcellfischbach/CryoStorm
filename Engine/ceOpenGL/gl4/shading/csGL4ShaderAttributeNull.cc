#include <ceOpenGL/gl4/shading/csGL4ShaderAttributeNull.hh>


namespace cryo::opengl
{

csGL4ShaderAttributeNull::csGL4ShaderAttributeNull(const std::string& name)
  : iShaderAttribute()
  , m_name(name)
{

}

const std::string& csGL4ShaderAttributeNull::GetName() const
{
  return m_name;
}

bool csGL4ShaderAttributeNull::IsValid() const
{
  return true;
}

void csGL4ShaderAttributeNull::SetArrayIndex(Size idx)
{
}



void csGL4ShaderAttributeNull::Bind(float x)
{
}



void csGL4ShaderAttributeNull::Bind(float x, float y)
{
}



void csGL4ShaderAttributeNull::Bind(float x, float y, float z)
{
}



void csGL4ShaderAttributeNull::Bind(float x, float y, float z, float w)
{
}



void csGL4ShaderAttributeNull::Bind(int x)
{
}



void csGL4ShaderAttributeNull::Bind(int x, int y)
{
}



void csGL4ShaderAttributeNull::Bind(int x, int y, int z)
{
}



void csGL4ShaderAttributeNull::Bind(int x, int y, int z, int w)
{
}



void csGL4ShaderAttributeNull::Bind(const csVector2f& v)
{
}



void csGL4ShaderAttributeNull::Bind(const csVector3f& v)
{
}



void csGL4ShaderAttributeNull::Bind(const csVector4f& v)
{
}



void csGL4ShaderAttributeNull::Bind(const csColor4f& v)
{
}



void csGL4ShaderAttributeNull::Bind(const csMatrix3f& v)
{
}




void csGL4ShaderAttributeNull::Bind(const csMatrix4f& v)
{
}


}