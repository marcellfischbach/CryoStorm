
add_definitions(-DCS_CORE_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        csClass.cc
        csClass.hh
        csClassRegistry.cc
        csClassRegistry.hh
        csDefs.hh
        csDiamond.hh
        csCoreExport.hh
        csCoreModule.cc
        csCoreModule.hh
        csEngine.cc
        csEngine.hh
        csFPS.cc
        csFPS.hh
        iGame.hh
        iModule.hh
        csObjectRegistry.cc
        csObjectRegistry.hh
        csOutOfBoundsException.hh
        csRef.hh
        csSettings.cc
        csSettings.hh
        csString.cc
        csString.hh
        csTicker.cc
        csTicker.hh
        csTime.cc
        csTime.hh
        csTypes.hh
        csViewport.cc
        csViewport.hh

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
        graphics/csDefaultMeshGenerators.cc
        graphics/csDefaultMeshGenerators.hh
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


        input/iInputSystem.hh
        input/iKeyboard.hh
        input/iMouse.hh
        input/csInput.cc
        input/csInput.hh
        input/eKey.hh

        loaders/csMaterialLoader.cc
        loaders/csMaterialLoader.hh
        loaders/csSamplerLoader.cc
        loaders/csSamplerLoader.hh
        loaders/csShaderGraphLoader.cc
        loaders/csShaderGraphLoader.hh
        loaders/csTerrainLayerLoader.cc
        loaders/csTerrainLayerLoader.hh
        loaders/csTextFileLoader.cc
        loaders/csTextFileLoader.hh
        loaders/csTextureLoader.cc
        loaders/csTextureLoader.hh


        math/csBoundingBox.cc
        math/csBoundingBox.hh
        math/csColor4f.hh
        math/eClippingResult.hh
        math/csHysteresis.hh
        math/iclipper.hh
        math/csMath.hh
        math/csMatrix.hh
        math/csMatrix3f.hh
        math/csMatrix4f.hh
        math/csPlane.hh
        math/csQuaternion.hh
        math/csRandomRange.hh
        math/csSphere.hh
        math/csVector.hh
        math/csVector2f.hh
        math/csVector2i.hh
        math/csVector3f.hh
        math/csVector4f.hh
        math/csVector4i.hh

        math/clipper/csBoxClipper.cc
        math/clipper/csBoxClipper.hh
        math/clipper/csCameraClipper.cc
        math/clipper/csCameraClipper.hh
        math/clipper/csCompoundClipper.cc
        math/clipper/csCompoundClipper.hh
        math/clipper/csMultiPlaneClipper.cc
        math/clipper/csMultiPlaneClipper.hh
        math/clipper/csPlaneClipper.cc
        math/clipper/csPlaneClipper.hh
        math/clipper/csSphereClipper.cc
        math/clipper/csSphereClipper.hh

        physics/iCollider.hh
        physics/iCollisionShape.hh
        physics/iPhysicsSystem.hh
        physics/iPhysicsWorld.hh

        resource/csAssetManager.cc
        resource/csAssetManager.hh
        resource/csBaseCSFAssetLoader.cc
        resource/csBaseCSFAssetLoader.hh
        resource/csBinaryData.cc
        resource/csBinaryData.hh
        resource/csCryoFile.cc
        resource/csCryoFile.hh
        resource/csFileSystemArchive.cc
        resource/csFileSystemArchive.hh
        resource/csFileSystemFile.cc
        resource/csFileSystemFile.hh
        resource/csResource.cc
        resource/csResource.hh
        resource/csResource_Impl.hh
        resource/csResourceExceptions.hh
        resource/csResourceLocator.cc
        resource/csResourceLocator.hh
        resource/csTextFile.cc
        resource/csTextFile.hh
        resource/csVFS.cc
        resource/csVFS.hh
        resource/csVFSConfigReader.cc
        resource/csVFSConfigReader.hh
        resource/eAccessMode.hh
        resource/eOpenMode.hh
        resource/eSeekMode.hh
        resource/iArchive.hh
        resource/iAssetLoader.hh
        resource/iFile.hh
        resource/iResource.hh



        window/iWindow.hh

)


if (${JAVA_BINDINGS})
    SET(SOURCES ${SOURCES}
            csJava.cc
            csJava.hh
            csJavaCalls.hh
            csJavaModule.cc
            csJavaModule.hh
            csJavaGame.cc
            csJavaGame.hh


            entity/csJavaState.cc
            entity/csJavaState.hh

            graphics/iDevice_java.cc
            graphics/iRenderMesh_java.cc
            graphics/iTexture_java.cc
            graphics/iTexture2D_java.cc
            graphics/csMesh_java.cc
            graphics/shading/iShader_java.cc
            graphics/shading/iShaderAttribute_java.cc

            input/csInput_java.cc

            resource/csJavaArchive.cc
            resource/csJavaArchive.hh
            resource/csJavaFile.cc
            resource/csJavaFile.hh

            window/csWindowJava.cc
            window/csWindowJava.hh
    )

endif ()


add_library(csCore SHARED
        ${SOURCES}
)

target_link_libraries(csCore
        ${PNG_LIBRARIES}
)

if (${JAVA_BINDINGS})
    target_link_libraries(csCore ${JNI_LIBRARIES})
endif ()

add_dependencies(csCore csMOC)
CS_MOC(csCore "${CryoStorm_SOURCE_DIR}/JCryoStorm/CsCore/src/main/generated")


install(TARGETS csCore
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
)

install(DIRECTORY . DESTINATION include/csCore FILES_MATCHING PATTERN "*.hh")



