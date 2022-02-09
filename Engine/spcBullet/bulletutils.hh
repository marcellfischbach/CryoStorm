#pragma 

#include <spcCore/defs.hh>
#include <spcCore/math/matrix4f.hh>
#include <LinearMath/btTransform.h>

namespace spc::bullet
{


SPC_FORCEINLINE void btTransformToMatrix4f(const btTransform& tr, Matrix4f& matrix)
{
  tr.getOpenGLMatrix(&matrix.m00);
}

SPC_FORCEINLINE void Matrix4fTobtTransform(const Matrix4f& matrix, btTransform& tr)
{
  tr.setFromOpenGLMatrix(&matrix.m00);

}



}