

add_definitions(-DCS_OGL_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        csGLError.hh
		csOpenGLExport.hh
		csOpenGLModule.cc
		csOpenGLModule.hh
		gl4/csGL4Device.cc
		gl4/csGL4Device.hh
		gl4/csGL4DirectionalLight.cc
		gl4/csGL4DirectionalLight.hh
		gl4/csGL4Exceptions.cc
		gl4/csGL4Exceptions.hh
		gl4/csGL4IndexBuffer.cc
		gl4/csGL4IndexBuffer.hh
		gl4/csGL4PixelFormatMap.hh
		gl4/csGL4PointLight.cc
		gl4/csGL4PointLight.hh
		gl4/csGL4RenderMesh.cc
		gl4/csGL4RenderMesh.hh
		gl4/csGL4RenderTarget2D.cc
		gl4/csGL4RenderTarget2D.hh
		gl4/csGL4RenderTarget2DArray.cc
		gl4/csGL4RenderTarget2DArray.hh
		gl4/csGL4RenderTargetCube.cc
		gl4/csGL4RenderTargetCube.hh
		gl4/csGL4Sampler.cc
		gl4/csGL4Sampler.hh
		gl4/csGL4Texture2D.cc
		gl4/csGL4Texture2D.hh
		gl4/csGL4Texture2DArray.cc
		gl4/csGL4Texture2DArray.hh
		gl4/csGL4TextureCube.cc
		gl4/csGL4TextureCube.hh
		gl4/csGL4TerrainMeshGeneratorFactory.cc
		gl4/csGL4TerrainMeshGeneratorFactory.hh
		gl4/csGL4TerrainMeshCPU.cc
		gl4/csGL4TerrainMeshCPU.hh
		gl4/csGL4TerrainMeshGPU.cc
		gl4/csGL4TerrainMeshGPU.hh
		gl4/csGL4VertexBuffer.cc
		gl4/csGL4VertexBuffer.hh


		gl4/loading/csGL4ProgramLoader.cc
		gl4/loading/csGL4ProgramLoader.hh
		gl4/loading/csGL4ShaderLoader.cc
		gl4/loading/csGL4ShaderLoader.hh


		gl4/pipeline/csGL4FrameRenderer.cc
        gl4/pipeline/csGL4FrameRenderer.hh
		gl4/pipeline/csGL4MeshSorter.cc
		gl4/pipeline/csGL4MeshSorter.hh


		gl4/pipeline/deferred/csGL4DeferredDirectionalLightRenderer.cc
		gl4/pipeline/deferred/csGL4DeferredDirectionalLightRenderer.hh
		gl4/pipeline/deferred/csGL4DeferredPipeline.cc
		gl4/pipeline/deferred/csGL4DeferredPipeline.hh
		gl4/pipeline/deferred/csGL4DeferredPointLightRenderer.cc
		gl4/pipeline/deferred/csGL4DeferredPointLightRenderer.hh

		gl4/pipeline/forward/csGL4ForwardDirectionalLightRenderer.cc
		gl4/pipeline/forward/csGL4ForwardDirectionalLightRenderer.hh
		gl4/pipeline/forward/csGL4ForwardPipeline.cc
		gl4/pipeline/forward/csGL4ForwardPipeline.hh
		gl4/pipeline/forward/csGL4ForwardPointLightRenderer.cc
		gl4/pipeline/forward/csGL4ForwardPointLightRenderer.hh

		gl4/pipeline/pointsm/csGL4PointSMFilter.cc
		gl4/pipeline/pointsm/csGL4PointSMFilter.hh
		gl4/pipeline/pointsm/csGL4PointSMRenderer.cc
		gl4/pipeline/pointsm/csGL4PointSMRenderer.hh

		gl4/pipeline/pssm/csGL4PSSMRenderer.cc
		gl4/pipeline/pssm/csGL4PSSMRenderer.hh
		gl4/pipeline/pssm/csGL4PSSMFilter.cc
		gl4/pipeline/pssm/csGL4PSSMFilter.hh

		gl4/shadergraph/csGL4ShaderGraphCompiler.cc
		gl4/shadergraph/csGL4ShaderGraphCompiler_depth.cc
		gl4/shadergraph/csGL4ShaderGraphCompiler_forward.cc
		gl4/shadergraph/csGL4ShaderGraphCompiler_gbuffer.cc
		gl4/shadergraph/csGL4ShaderGraphCompiler_variables.cc
		gl4/shadergraph/csGL4ShaderGraphCompiler.hh


		gl4/shading/csGL4Program.cc
		gl4/shading/csGL4Program.hh
		gl4/shading/csGL4Shader.cc
		gl4/shading/csGL4Shader.hh
		gl4/shading/csGL4ShaderAttribute.cc
		gl4/shading/csGL4ShaderAttribute.hh
		gl4/shading/csGL4ShaderAttributeNull.cc
		gl4/shading/csGL4ShaderAttributeNull.hh
		gl4/shading/csGL4ShaderSourceFragment.cc
		gl4/shading/csGL4ShaderSourceFragment.hh
		gl4/shading/eGL4ShaderType.hh
        )


add_library(csOpenGL SHARED
        ${SOURCES}
        )

target_link_libraries(csOpenGL
        csCore
        tinyxml2
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        )

add_dependencies(csOpenGL csMOC)
CS_MOC(csOpenGL "")


install(TARGETS csOpenGL
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/csOpenGL FILES_MATCHING PATTERN "*.hh")


