
#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/color4f.hh>
#include <spcCore/math/vector.hh>
#include <spcCore/math/matrix.hh>
#include <spcCore/graphics/itexture.hh>
#include <string>


namespace spc
{

SPC_CLASS()
struct SPC_CORE_API iMaterial : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  virtual ~iMaterial()
  {

  }

  virtual bool Bind(iDevice * device, eRenderPass pass) = 0;

  virtual UInt16 IndexOf(const std::string & attributeName) = 0;

  virtual void Set(UInt16 idx, float value) = 0;
  virtual void Set(UInt16 idx, const Vector2f & v) = 0;
  virtual void Set(UInt16 idx, const Vector3f & v) = 0;
  virtual void Set(UInt16 idx, const Vector4f & v) = 0;
  virtual void Set(UInt16 idx, const Color4f & v) = 0;
  virtual void Set(UInt16 idx, int value) = 0;
  virtual void Set(UInt16 idx, const Matrix3f & m) = 0;
  virtual void Set(UInt16 idx, const Matrix4f & m) = 0;
  virtual void Set(UInt16 idx, iTexture * texture) = 0;
};

}