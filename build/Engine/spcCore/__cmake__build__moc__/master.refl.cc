

#include <stdarg.h>
#include "class.hh"
#include "class.refl.hh"
#include "entity\entity.hh"
#include "entity\entity.refl.hh"
#include "entity\entitystate.hh"
#include "entity\entitystate.refl.hh"
#include "entity\lightstate.hh"
#include "entity\lightstate.refl.hh"
#include "entity\spatialstate.hh"
#include "entity\spatialstate.refl.hh"
#include "entity\staticmeshstate.hh"
#include "entity\staticmeshstate.refl.hh"
#include "entity\world.hh"
#include "entity\world.refl.hh"
#include "graphics\camera.hh"
#include "graphics\camera.refl.hh"
#include "graphics\idevice.hh"
#include "graphics\idevice.refl.hh"
#include "graphics\idirectionallight.hh"
#include "graphics\idirectionallight.refl.hh"
#include "graphics\iindexbuffer.hh"
#include "graphics\iindexbuffer.refl.hh"
#include "graphics\ilight.hh"
#include "graphics\ilight.refl.hh"
#include "graphics\image.hh"
#include "graphics\image.refl.hh"
#include "graphics\ipointlight.hh"
#include "graphics\ipointlight.refl.hh"
#include "graphics\irendermesh.hh"
#include "graphics\irendermesh.refl.hh"
#include "graphics\irenderpipeline.hh"
#include "graphics\irenderpipeline.refl.hh"
#include "graphics\irendertarget.hh"
#include "graphics\irendertarget.refl.hh"
#include "graphics\irendertarget2d.hh"
#include "graphics\irendertarget2d.refl.hh"
#include "graphics\irendertarget2darray.hh"
#include "graphics\irendertarget2darray.refl.hh"
#include "graphics\irendertargetcube.hh"
#include "graphics\irendertargetcube.refl.hh"
#include "graphics\isampler.hh"
#include "graphics\isampler.refl.hh"
#include "graphics\itexture.hh"
#include "graphics\itexture.refl.hh"
#include "graphics\itexture2d.hh"
#include "graphics\itexture2d.refl.hh"
#include "graphics\itexture2darray.hh"
#include "graphics\itexture2darray.refl.hh"
#include "graphics\itexturecube.hh"
#include "graphics\itexturecube.refl.hh"
#include "graphics\ivertexbuffer.hh"
#include "graphics\ivertexbuffer.refl.hh"
#include "graphics\material\imaterial.hh"
#include "graphics\material\imaterial.refl.hh"
#include "graphics\material\material.hh"
#include "graphics\material\material.refl.hh"
#include "graphics\material\materialinstance.hh"
#include "graphics\material\materialinstance.refl.hh"
#include "graphics\mesh.hh"
#include "graphics\mesh.refl.hh"
#include "graphics\samplers.hh"
#include "graphics\samplers.refl.hh"
#include "graphics\scene\gfxlight.hh"
#include "graphics\scene\gfxlight.refl.hh"
#include "graphics\scene\gfxmesh.hh"
#include "graphics\scene\gfxmesh.refl.hh"
#include "graphics\scene\gfxscene.hh"
#include "graphics\scene\gfxscene.refl.hh"
#include "graphics\shading\ishader.hh"
#include "graphics\shading\ishader.refl.hh"
#include "input\iinputsystem.hh"
#include "input\iinputsystem.refl.hh"
#include "loaders\samplerloader.hh"
#include "loaders\samplerloader.refl.hh"
#include "resource\filesystemfile.hh"
#include "resource\filesystemfile.refl.hh"
#include "resource\iassetloader.hh"
#include "resource\iassetloader.refl.hh"
#include "resource\ifile.hh"
#include "resource\ifile.refl.hh"
#include "window\iwindow.hh"
#include "window\iwindow.refl.hh"


#include "class.refl.cc"
#include "entity\entity.refl.cc"
#include "entity\entitystate.refl.cc"
#include "entity\lightstate.refl.cc"
#include "entity\spatialstate.refl.cc"
#include "entity\staticmeshstate.refl.cc"
#include "entity\world.refl.cc"
#include "graphics\camera.refl.cc"
#include "graphics\idevice.refl.cc"
#include "graphics\idirectionallight.refl.cc"
#include "graphics\iindexbuffer.refl.cc"
#include "graphics\ilight.refl.cc"
#include "graphics\image.refl.cc"
#include "graphics\ipointlight.refl.cc"
#include "graphics\irendermesh.refl.cc"
#include "graphics\irenderpipeline.refl.cc"
#include "graphics\irendertarget.refl.cc"
#include "graphics\irendertarget2d.refl.cc"
#include "graphics\irendertarget2darray.refl.cc"
#include "graphics\irendertargetcube.refl.cc"
#include "graphics\isampler.refl.cc"
#include "graphics\itexture.refl.cc"
#include "graphics\itexture2d.refl.cc"
#include "graphics\itexture2darray.refl.cc"
#include "graphics\itexturecube.refl.cc"
#include "graphics\ivertexbuffer.refl.cc"
#include "graphics\material\imaterial.refl.cc"
#include "graphics\material\material.refl.cc"
#include "graphics\material\materialinstance.refl.cc"
#include "graphics\mesh.refl.cc"
#include "graphics\samplers.refl.cc"
#include "graphics\scene\gfxlight.refl.cc"
#include "graphics\scene\gfxmesh.refl.cc"
#include "graphics\scene\gfxscene.refl.cc"
#include "graphics\shading\ishader.refl.cc"
#include "input\iinputsystem.refl.cc"
#include "loaders\samplerloader.refl.cc"
#include "resource\filesystemfile.refl.cc"
#include "resource\iassetloader.refl.cc"
#include "resource\ifile.refl.cc"
#include "window\iwindow.refl.cc"


#include <spcCore/classregistry.hh>

static void register_classes()
{
  spc::ClassRegistry *reg = spc::ClassRegistry::Get();
  // class.hh
  reg->Register(spc::ObjectClass::Get());
  // entity\entity.hh
  reg->Register(spc::EntityClass::Get());
  // entity\entitystate.hh
  reg->Register(spc::EntityStateClass::Get());
  // entity\lightstate.hh
  reg->Register(spc::LightStateClass::Get());
  // entity\spatialstate.hh
  reg->Register(spc::SpatialStateClass::Get());
  // entity\staticmeshstate.hh
  reg->Register(spc::StaticMeshStateClass::Get());
  // entity\world.hh
  reg->Register(spc::WorldClass::Get());
  // graphics\camera.hh
  reg->Register(spc::CameraClass::Get());
  // graphics\idevice.hh
  reg->Register(spc::iDeviceClass::Get());
  // graphics\idirectionallight.hh
  reg->Register(spc::iDirectionalLightClass::Get());
  // graphics\iindexbuffer.hh
  reg->Register(spc::iIndexBufferClass::Get());
  // graphics\ilight.hh
  reg->Register(spc::iLightClass::Get());
  // graphics\image.hh
  reg->Register(spc::ImageClass::Get());
  // graphics\ipointlight.hh
  reg->Register(spc::iPointLightClass::Get());
  // graphics\irendermesh.hh
  reg->Register(spc::iRenderMeshClass::Get());
  reg->Register(spc::iRenderMeshGeneratorClass::Get());
  reg->Register(spc::iRenderMeshGeneratorFactoryClass::Get());
  // graphics\irenderpipeline.hh
  reg->Register(spc::iRenderPipelineClass::Get());
  // graphics\irendertarget.hh
  reg->Register(spc::iRenderTargetClass::Get());
  // graphics\irendertarget2d.hh
  reg->Register(spc::iRenderTarget2DClass::Get());
  // graphics\irendertarget2darray.hh
  reg->Register(spc::iRenderTarget2DArrayClass::Get());
  // graphics\irendertargetcube.hh
  reg->Register(spc::iRenderTargetCubeClass::Get());
  // graphics\isampler.hh
  reg->Register(spc::iSamplerClass::Get());
  // graphics\itexture.hh
  reg->Register(spc::iTextureClass::Get());
  // graphics\itexture2d.hh
  reg->Register(spc::iTexture2DClass::Get());
  // graphics\itexture2darray.hh
  reg->Register(spc::iTexture2DArrayClass::Get());
  // graphics\itexturecube.hh
  reg->Register(spc::iTextureCubeClass::Get());
  // graphics\ivertexbuffer.hh
  reg->Register(spc::iVertexBufferClass::Get());
  // graphics\material\imaterial.hh
  reg->Register(spc::iMaterialClass::Get());
  // graphics\material\material.hh
  reg->Register(spc::MaterialClass::Get());
  // graphics\material\materialinstance.hh
  reg->Register(spc::MaterialInstanceClass::Get());
  // graphics\mesh.hh
  reg->Register(spc::MeshClass::Get());
  // graphics\samplers.hh
  reg->Register(spc::SamplersClass::Get());
  // graphics\scene\gfxlight.hh
  reg->Register(spc::GfxLightClass::Get());
  // graphics\scene\gfxmesh.hh
  reg->Register(spc::GfxMeshClass::Get());
  // graphics\scene\gfxscene.hh
  reg->Register(spc::GfxSceneClass::Get());
  // graphics\shading\ishader.hh
  reg->Register(spc::iShaderClass::Get());
  // input\iinputsystem.hh
  reg->Register(spc::iInputSystemClass::Get());
  // loaders\samplerloader.hh
  reg->Register(spc::SamplerLoaderSpcClass::Get());
  // resource\filesystemfile.hh
  reg->Register(spc::FileSystemFileClass::Get());
  // resource\iassetloader.hh
  reg->Register(spc::iAssetLoaderClass::Get());
  reg->Register(spc::iAssetLoaderSpcClass::Get());
  // resource\ifile.hh
  reg->Register(spc::iFileClass::Get());
  // window\iwindow.hh
  reg->Register(spc::iWindowClass::Get());
}

static void unregister_classes()
{
  spc::ClassRegistry *reg = spc::ClassRegistry::Get();
  // class.hh
  reg->Unregister(spc::ObjectClass::Get());
  // entity\entity.hh
  reg->Unregister(spc::EntityClass::Get());
  // entity\entitystate.hh
  reg->Unregister(spc::EntityStateClass::Get());
  // entity\lightstate.hh
  reg->Unregister(spc::LightStateClass::Get());
  // entity\spatialstate.hh
  reg->Unregister(spc::SpatialStateClass::Get());
  // entity\staticmeshstate.hh
  reg->Unregister(spc::StaticMeshStateClass::Get());
  // entity\world.hh
  reg->Unregister(spc::WorldClass::Get());
  // graphics\camera.hh
  reg->Unregister(spc::CameraClass::Get());
  // graphics\idevice.hh
  reg->Unregister(spc::iDeviceClass::Get());
  // graphics\idirectionallight.hh
  reg->Unregister(spc::iDirectionalLightClass::Get());
  // graphics\iindexbuffer.hh
  reg->Unregister(spc::iIndexBufferClass::Get());
  // graphics\ilight.hh
  reg->Unregister(spc::iLightClass::Get());
  // graphics\image.hh
  reg->Unregister(spc::ImageClass::Get());
  // graphics\ipointlight.hh
  reg->Unregister(spc::iPointLightClass::Get());
  // graphics\irendermesh.hh
  reg->Unregister(spc::iRenderMeshClass::Get());
  reg->Unregister(spc::iRenderMeshGeneratorClass::Get());
  reg->Unregister(spc::iRenderMeshGeneratorFactoryClass::Get());
  // graphics\irenderpipeline.hh
  reg->Unregister(spc::iRenderPipelineClass::Get());
  // graphics\irendertarget.hh
  reg->Unregister(spc::iRenderTargetClass::Get());
  // graphics\irendertarget2d.hh
  reg->Unregister(spc::iRenderTarget2DClass::Get());
  // graphics\irendertarget2darray.hh
  reg->Unregister(spc::iRenderTarget2DArrayClass::Get());
  // graphics\irendertargetcube.hh
  reg->Unregister(spc::iRenderTargetCubeClass::Get());
  // graphics\isampler.hh
  reg->Unregister(spc::iSamplerClass::Get());
  // graphics\itexture.hh
  reg->Unregister(spc::iTextureClass::Get());
  // graphics\itexture2d.hh
  reg->Unregister(spc::iTexture2DClass::Get());
  // graphics\itexture2darray.hh
  reg->Unregister(spc::iTexture2DArrayClass::Get());
  // graphics\itexturecube.hh
  reg->Unregister(spc::iTextureCubeClass::Get());
  // graphics\ivertexbuffer.hh
  reg->Unregister(spc::iVertexBufferClass::Get());
  // graphics\material\imaterial.hh
  reg->Unregister(spc::iMaterialClass::Get());
  // graphics\material\material.hh
  reg->Unregister(spc::MaterialClass::Get());
  // graphics\material\materialinstance.hh
  reg->Unregister(spc::MaterialInstanceClass::Get());
  // graphics\mesh.hh
  reg->Unregister(spc::MeshClass::Get());
  // graphics\samplers.hh
  reg->Unregister(spc::SamplersClass::Get());
  // graphics\scene\gfxlight.hh
  reg->Unregister(spc::GfxLightClass::Get());
  // graphics\scene\gfxmesh.hh
  reg->Unregister(spc::GfxMeshClass::Get());
  // graphics\scene\gfxscene.hh
  reg->Unregister(spc::GfxSceneClass::Get());
  // graphics\shading\ishader.hh
  reg->Unregister(spc::iShaderClass::Get());
  // input\iinputsystem.hh
  reg->Unregister(spc::iInputSystemClass::Get());
  // loaders\samplerloader.hh
  reg->Unregister(spc::SamplerLoaderSpcClass::Get());
  // resource\filesystemfile.hh
  reg->Unregister(spc::FileSystemFileClass::Get());
  // resource\iassetloader.hh
  reg->Unregister(spc::iAssetLoaderClass::Get());
  reg->Unregister(spc::iAssetLoaderSpcClass::Get());
  // resource\ifile.hh
  reg->Unregister(spc::iFileClass::Get());
  // window\iwindow.hh
  reg->Unregister(spc::iWindowClass::Get());
}

