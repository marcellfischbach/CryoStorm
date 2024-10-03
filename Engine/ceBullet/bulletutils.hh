#pragma 

#include <ceCore/defs.hh>
#include <ceCore/math/matrix4f.hh>
#include <LinearMath/btTransform.h>

namespace ce::bullet
{


CS_FORCEINLINE void btTransformToMatrix4f(const btTransform& tr, Matrix4f& matrix)
{
  tr.getOpenGLMatrix(&matrix.m00);
}

CS_FORCEINLINE void Matrix4fTobtTransform(const Matrix4f& matrix, btTransform& tr)
{
  tr.setFromOpenGLMatrix(&matrix.m00);

}



}