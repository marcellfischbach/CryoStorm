#pragma 

#include <csCore/csDefs.hh>
#include <csCore/math/csMatrix4f.hh>
#include <LinearMath/btTransform.h>

namespace cryo::bullet
{


CS_FORCEINLINE void btTransformToMatrix4f(const btTransform& tr, csMatrix4f& matrix)
{
  tr.getOpenGLMatrix(&matrix.m00);
}

CS_FORCEINLINE void Matrix4fTobtTransform(const csMatrix4f& matrix, btTransform& tr)
{
  tr.setFromOpenGLMatrix(&matrix.m00);

}



}