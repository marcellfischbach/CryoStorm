
add_definitions(-DCS_CORE_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        class.cc
        class.hh
        classregistry.cc
        classregistry.hh
        defs.hh
        diamond.hh
        coreexport.hh
        coremodule.cc
        coremodule.hh
        engine.cc
        engine.hh
        fps.cc
        fps.hh
        igame.hh
        imodule.hh
        objectregistry.cc
        objectregistry.hh
        outofboundsexception.hh
        settings.cc
        settings.hh
        string.cc
        string.hh
        ticker.cc
        ticker.hh
        time.cc
        time.hh
        types.hh

        animation/csSkeletonAnimation.cc
        animation/csSkeletonAnimation.hh
        animation/csSkeletonAnimationPlayer.cc
        animation/csSkeletonAnimationPlayer.hh


        entity/csCameraState.cc
        entity/csCameraState.hh
        entity/csCollisionState.cc
        entity/csCollisionState.hh
        entity/csEntity.cc
        entity/csEntity.hh
        entity/csEntityState.cc
        entity/csEntityState.hh
        entity/csLightState.cc
        entity/csLightState.hh
        entity/csRigidBodyState.cc
        entity/csRigidBodyState.hh
        entity/csSkeletonMeshState.cc
        entity/csSkeletonMeshState.hh
        entity/csSpatialState.cc
        entity/csSpatialState.hh
        entity/csStaticColliderState.cc
        entity/csStaticColliderState.hh
        entity/csStaticMeshState.cc
        entity/csStaticMeshState.hh
        entity/csTerrainMeshState.cc
        entity/csTerrainMeshState.hh
        entity/csTransform.cc
        entity/csTransform.hh
        entity/csWorld.cc
        entity/csWorld.hh


        graphics/csCamera.cc
        graphics/csCamera.hh
        graphics/csDefaultFrameRenderer.cc
        graphics/csDefaultFrameRenderer.hh
        graphics/eBufferUsage.hh
        graphics/eCompareFunc.hh
        graphics/eDataType.hh
        graphics/eFilterMode.hh
        graphics/ePixelFormat.hh
        graphics/ePrimitiveType.hh
        graphics/eProjectionMode.hh
        graphics/eRenderPass.hh
        graphics/eShaderAttribute.hh
        graphics/eTextureAddressMode.hh
        graphics/eTextureCompareMode.hh
        graphics/eTextureType.hh
        graphics/eTextureUnit.hh
        graphics/eVertexStream.hh
        graphics/csGBuffer.cc
        graphics/csGBuffer.hh
        graphics/csGraphicsExceptions.hh
        graphics/iDevice.hh
        graphics/iDirectionalLight.hh
        graphics/iFrameRenderer.hh
        graphics/iIndexBuffer.hh
        graphics/iLight.hh
        graphics/csImage.cc
        graphics/csImage.hh
        graphics/iPointLight.hh
        graphics/iRenderMesh.hh
        graphics/iRenderPipeline.hh
        graphics/iRenderTarget.hh
        graphics/iRenderTarget2D.hh
        graphics/iRenderTarget2DArray.hh
        graphics/iRenderTargetCube.hh
        graphics/iTerrainMesh.hh
        graphics/iTexture.hh
        graphics/iTexture2D.hh
        graphics/iTexture2DArray.hh
        graphics/iTextureCube.hh
        graphics/iSampler.hh
        graphics/iSkyboxRenderer.hh
        graphics/iVertexBuffer.hh
        graphics/csMesh.cc
        graphics/csMesh.hh
        graphics/csPostProcessing.cc
        graphics/csPostProcessing.hh
        graphics/csProjector.cc
        graphics/csProjector.hh
        graphics/csSamplers.cc
        graphics/csSamplers.hh
        graphics/csSkeleton.cc
        graphics/csSkeleton.hh
        graphics/csSkeletonMesh.cc
        graphics/csSkeletonMesh.hh
        graphics/csVertexDeclaration.cc
        graphics/csVertexDeclaration.hh


        graphics/material/eMaterialAttributeType.hh
        graphics/material/iMaterial.hh
        graphics/material/csMaterial.cc
        graphics/material/csMaterial.hh
        graphics/material/csMaterialInstance.cc
        graphics/material/csMaterialInstance.hh

        graphics/pp/csPP.hh
        graphics/pp/csPPCombine.cc
        graphics/pp/csPPCombine.hh
        graphics/pp/csPPBlurH.cc
        graphics/pp/csPPBlurH.hh
        graphics/pp/csPPBlurV.cc
        graphics/pp/csPPBlurV.hh
        graphics/pp/csPPHighPass.cc
        graphics/pp/csPPHighPass.hh
        graphics/pp/csPPScaleDown.cc
        graphics/pp/csPPScaleDown.hh

        graphics/scene/csGfxCamera.cc
        graphics/scene/csGfxCamera.hh
        graphics/scene/csGfxLight.cc
        graphics/scene/csGfxLight.hh
        graphics/scene/csGfxMesh.cc
        graphics/scene/csGfxMesh.hh
        graphics/scene/csGfxMeshCompound.cc
        graphics/scene/csGfxMeshCompound.hh
        graphics/scene/csGfxOctreeScene.cc
        graphics/scene/csGfxOctreeScene.hh
        graphics/scene/csGfxQuadtreeScene.cc
        graphics/scene/csGfxQuadtreeScene.hh
        graphics/scene/csGfxSceneCollector.cc
        graphics/scene/csGfxSceneCollector.hh
        graphics/scene/csGfxSimpleScene.cc
        graphics/scene/csGfxSimpleScene.hh
        graphics/scene/iGfxScene.hh

        graphics/shadergraph/eSGValueType.hh
        graphics/shadergraph/csSGNode.cc
        graphics/shadergraph/csSGNode.hh
        graphics/shadergraph/csSGNodes.cc
        graphics/shadergraph/csSGNodes.hh
        graphics/shadergraph/csShaderGraph.cc
        graphics/shadergraph/csShaderGraph.hh

        graphics/shading/iShader.hh
        graphics/shading/iShaderAttribute.hh


        graphics/skybox/csSimpleSkybox.cc
        graphics/skybox/csSimpleSkybox.hh
        graphics/skybox/csSkyboxMesh.cc
        graphics/skybox/csSkyboxMesh.hh


        input/iinputsystem.hh
        input/ikeyboard.hh
        input/imouse.hh
        input/input.cc
        input/input.hh
        input/key.hh

        loaders/materialloader.cc
        loaders/materialloader.hh
        loaders/samplerloader.cc
        loaders/samplerloader.hh
        loaders/shadergraphloader.cc
        loaders/shadergraphloader.hh
        loaders/terrainlayerloader.cc
        loaders/terrainlayerloader.hh
        loaders/textfileloader.cc
        loaders/textfileloader.hh
        loaders/textureloader.cc
        loaders/textureloader.hh


        math/boundingbox.cc
        math/boundingbox.hh
        math/color4f.hh
        math/eclippingresult.hh
        math/hysteresis.hh
        math/iclipper.hh
        math/math.hh
        math/matrix.hh
        math/matrix3f.hh
        math/matrix4f.hh
        math/plane.hh
        math/quaternion.hh
        math/randomrange.hh
        math/sphere.hh
        math/vector.hh
        math/vector2f.hh
        math/vector2i.hh
        math/vector3f.hh
        math/vector4f.hh
        math/vector4i.hh

        math/clipper/boxclipper.cc
        math/clipper/boxclipper.hh
        math/clipper/cameraclipper.cc
        math/clipper/cameraclipper.hh
        math/clipper/compoundclipper.cc
        math/clipper/compoundclipper.hh
        math/clipper/multiplaneclipper.cc
        math/clipper/multiplaneclipper.hh
        math/clipper/planeclipper.cc
        math/clipper/planeclipper.hh
        math/clipper/sphereclipper.cc
        math/clipper/sphereclipper.hh

        physics/icollider.hh
        physics/icollisionshape.hh
        physics/iphysicssystem.hh
        physics/iphysicsworld.hh

        resource/assetmanager.cc
        resource/assetmanager.hh
        resource/basecefassetloader.cc
        resource/basecefassetloader.hh
        resource/binarydata.cc
        resource/binarydata.hh
        resource/eaccessmode.hh
        resource/eopenmode.hh
        resource/eseekmode.hh
        resource/file.cc
        resource/file.hh
        resource/filesystemarchive.cc
        resource/filesystemarchive.hh
        resource/filesystemfile.cc
        resource/filesystemfile.hh
        resource/iarchive.hh
        resource/iassetloader.hh
        resource/ifile.hh
        resource/resourceexceptions.hh
        resource/resourcelocator.cc
        resource/resourcelocator.hh
        resource/textfile.cc
        resource/textfile.hh
        resource/vfs.cc
        resource/vfs.hh
        resource/vfsconfigreader.cc
        resource/vfsconfigreader.hh

        #window/wnddesc.hh

        window/iwindow.hh

)


if (${JAVA_BINDINGS})
    SET(SOURCES ${SOURCES}
            engine_java.cc
            java.cc
            java.hh
            java_methods.hh
            game_java.cc
            game_java.hh
            objectregistry_java.cc


            entity/csEntity_java.cc
            entity/csEntityState_java.cc
            entity/csJavaState.cc
            entity/csJavaState.hh
            entity/csSpatialState_java.cc
            entity/csStaticMeshState_java.cc
            entity/csWorld_java.cc

            graphics/iDevice_java.cc
            graphics/iRenderMesh_java.cc
            graphics/iTexture_java.cc
            graphics/iTexture2D_java.cc
            graphics/csMesh_java.cc
            graphics/material/csMaterial_java.cc
            graphics/material/csMaterialInstance_java.cc
            graphics/shading/iShader_java.cc
            graphics/shading/iShaderAttribute_java.cc

            input/input_java.cc

            resource/assetmanager_java.cc
            resource/javaarchive.cc
            resource/javaarchive.hh
            resource/javafile.cc
            resource/javafile.hh
    )

endif ()


add_library(ceCore SHARED
        ${SOURCES}
)

target_link_libraries(ceCore
        ${PNG_LIBRARIES}
)

if (${JAVA_BINDINGS})
    target_link_libraries(ceCore ${JNI_LIBRARIES})
endif ()

add_dependencies(ceCore ceMOC)
CS_MOC(ceCore)


install(TARGETS ceCore
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
)

install(DIRECTORY . DESTINATION include/ceCore FILES_MATCHING PATTERN "*.hh")



