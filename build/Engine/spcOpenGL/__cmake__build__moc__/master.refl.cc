

#include <stdarg.h>
#include "gl4\gl4device.hh"
#include "gl4\gl4device.refl.hh"
#include "gl4\gl4directionallight.hh"
#include "gl4\gl4directionallight.refl.hh"
#include "gl4\gl4indexbuffer.hh"
#include "gl4\gl4indexbuffer.refl.hh"
#include "gl4\gl4pointlight.hh"
#include "gl4\gl4pointlight.refl.hh"
#include "gl4\gl4rendermesh.hh"
#include "gl4\gl4rendermesh.refl.hh"
#include "gl4\gl4rendertarget2d.hh"
#include "gl4\gl4rendertarget2d.refl.hh"
#include "gl4\gl4rendertarget2darray.hh"
#include "gl4\gl4rendertarget2darray.refl.hh"
#include "gl4\gl4rendertargetcube.hh"
#include "gl4\gl4rendertargetcube.refl.hh"
#include "gl4\gl4sampler.hh"
#include "gl4\gl4sampler.refl.hh"
#include "gl4\gl4texture2d.hh"
#include "gl4\gl4texture2d.refl.hh"
#include "gl4\gl4texture2darray.hh"
#include "gl4\gl4texture2darray.refl.hh"
#include "gl4\gl4texturecube.hh"
#include "gl4\gl4texturecube.refl.hh"
#include "gl4\gl4vertexbuffer.hh"
#include "gl4\gl4vertexbuffer.refl.hh"
#include "gl4\loading\gl4programloader.hh"
#include "gl4\loading\gl4programloader.refl.hh"
#include "gl4\loading\gl4shaderloader.hh"
#include "gl4\loading\gl4shaderloader.refl.hh"
#include "gl4\pipeline\forward\gl4forwardpipeline.hh"
#include "gl4\pipeline\forward\gl4forwardpipeline.refl.hh"
#include "gl4\shading\gl4program.hh"
#include "gl4\shading\gl4program.refl.hh"
#include "gl4\shading\gl4shader.hh"
#include "gl4\shading\gl4shader.refl.hh"
#include "gl4\shading\gl4shadersourcefragment.hh"
#include "gl4\shading\gl4shadersourcefragment.refl.hh"


#include "gl4\gl4device.refl.cc"
#include "gl4\gl4directionallight.refl.cc"
#include "gl4\gl4indexbuffer.refl.cc"
#include "gl4\gl4pointlight.refl.cc"
#include "gl4\gl4rendermesh.refl.cc"
#include "gl4\gl4rendertarget2d.refl.cc"
#include "gl4\gl4rendertarget2darray.refl.cc"
#include "gl4\gl4rendertargetcube.refl.cc"
#include "gl4\gl4sampler.refl.cc"
#include "gl4\gl4texture2d.refl.cc"
#include "gl4\gl4texture2darray.refl.cc"
#include "gl4\gl4texturecube.refl.cc"
#include "gl4\gl4vertexbuffer.refl.cc"
#include "gl4\loading\gl4programloader.refl.cc"
#include "gl4\loading\gl4shaderloader.refl.cc"
#include "gl4\pipeline\forward\gl4forwardpipeline.refl.cc"
#include "gl4\shading\gl4program.refl.cc"
#include "gl4\shading\gl4shader.refl.cc"
#include "gl4\shading\gl4shadersourcefragment.refl.cc"


#include <spcCore/classregistry.hh>

static void register_classes()
{
  spc::ClassRegistry *reg = spc::ClassRegistry::Get();
  // gl4\gl4device.hh
  reg->Register(spc::opengl::GL4DeviceClass::Get());
  // gl4\gl4directionallight.hh
  reg->Register(spc::opengl::GL4DirectionalLightClass::Get());
  // gl4\gl4indexbuffer.hh
  reg->Register(spc::opengl::GL4IndexBufferClass::Get());
  // gl4\gl4pointlight.hh
  reg->Register(spc::opengl::GL4PointLightClass::Get());
  // gl4\gl4rendermesh.hh
  reg->Register(spc::opengl::GL4RenderMeshClass::Get());
  reg->Register(spc::opengl::GL4RenderMeshGeneratorClass::Get());
  reg->Register(spc::opengl::GL4RenderMeshGeneratorFactoryClass::Get());
  // gl4\gl4rendertarget2d.hh
  reg->Register(spc::opengl::GL4RenderTarget2DClass::Get());
  // gl4\gl4rendertarget2darray.hh
  reg->Register(spc::opengl::GL4RenderTarget2DArrayClass::Get());
  // gl4\gl4rendertargetcube.hh
  reg->Register(spc::opengl::GL4RenderTargetCubeClass::Get());
  // gl4\gl4sampler.hh
  reg->Register(spc::opengl::GL4SamplerClass::Get());
  // gl4\gl4texture2d.hh
  reg->Register(spc::opengl::GL4Texture2DClass::Get());
  // gl4\gl4texture2darray.hh
  reg->Register(spc::opengl::GL4Texture2DArrayClass::Get());
  // gl4\gl4texturecube.hh
  reg->Register(spc::opengl::GL4TextureCubeClass::Get());
  // gl4\gl4vertexbuffer.hh
  reg->Register(spc::opengl::GL4VertexBufferClass::Get());
  // gl4\loading\gl4programloader.hh
  reg->Register(spc::opengl::GL4ProgramLoaderClass::Get());
  // gl4\loading\gl4shaderloader.hh
  reg->Register(spc::opengl::GL4ShaderLoaderSpcClass::Get());
  reg->Register(spc::opengl::GL4ShaderLoaderClass::Get());
  // gl4\pipeline\forward\gl4forwardpipeline.hh
  reg->Register(spc::opengl::GL4ForwardPipelineClass::Get());
  // gl4\shading\gl4program.hh
  reg->Register(spc::opengl::GL4ProgramClass::Get());
  // gl4\shading\gl4shader.hh
  reg->Register(spc::opengl::GL4ShaderClass::Get());
  // gl4\shading\gl4shadersourcefragment.hh
  reg->Register(spc::opengl::GL4ShaderSourceFragmentClass::Get());
}

static void unregister_classes()
{
  spc::ClassRegistry *reg = spc::ClassRegistry::Get();
  // gl4\gl4device.hh
  reg->Unregister(spc::opengl::GL4DeviceClass::Get());
  // gl4\gl4directionallight.hh
  reg->Unregister(spc::opengl::GL4DirectionalLightClass::Get());
  // gl4\gl4indexbuffer.hh
  reg->Unregister(spc::opengl::GL4IndexBufferClass::Get());
  // gl4\gl4pointlight.hh
  reg->Unregister(spc::opengl::GL4PointLightClass::Get());
  // gl4\gl4rendermesh.hh
  reg->Unregister(spc::opengl::GL4RenderMeshClass::Get());
  reg->Unregister(spc::opengl::GL4RenderMeshGeneratorClass::Get());
  reg->Unregister(spc::opengl::GL4RenderMeshGeneratorFactoryClass::Get());
  // gl4\gl4rendertarget2d.hh
  reg->Unregister(spc::opengl::GL4RenderTarget2DClass::Get());
  // gl4\gl4rendertarget2darray.hh
  reg->Unregister(spc::opengl::GL4RenderTarget2DArrayClass::Get());
  // gl4\gl4rendertargetcube.hh
  reg->Unregister(spc::opengl::GL4RenderTargetCubeClass::Get());
  // gl4\gl4sampler.hh
  reg->Unregister(spc::opengl::GL4SamplerClass::Get());
  // gl4\gl4texture2d.hh
  reg->Unregister(spc::opengl::GL4Texture2DClass::Get());
  // gl4\gl4texture2darray.hh
  reg->Unregister(spc::opengl::GL4Texture2DArrayClass::Get());
  // gl4\gl4texturecube.hh
  reg->Unregister(spc::opengl::GL4TextureCubeClass::Get());
  // gl4\gl4vertexbuffer.hh
  reg->Unregister(spc::opengl::GL4VertexBufferClass::Get());
  // gl4\loading\gl4programloader.hh
  reg->Unregister(spc::opengl::GL4ProgramLoaderClass::Get());
  // gl4\loading\gl4shaderloader.hh
  reg->Unregister(spc::opengl::GL4ShaderLoaderSpcClass::Get());
  reg->Unregister(spc::opengl::GL4ShaderLoaderClass::Get());
  // gl4\pipeline\forward\gl4forwardpipeline.hh
  reg->Unregister(spc::opengl::GL4ForwardPipelineClass::Get());
  // gl4\shading\gl4program.hh
  reg->Unregister(spc::opengl::GL4ProgramClass::Get());
  // gl4\shading\gl4shader.hh
  reg->Unregister(spc::opengl::GL4ShaderClass::Get());
  // gl4\shading\gl4shadersourcefragment.hh
  reg->Unregister(spc::opengl::GL4ShaderSourceFragmentClass::Get());
}

