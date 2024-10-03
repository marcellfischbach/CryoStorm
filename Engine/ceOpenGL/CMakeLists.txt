

add_definitions(-DCS_OGL_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        glerror.hh
        openglexport.hh
        openglmodule.cc
        openglmodule.hh
        gl4/gl4device.cc
        gl4/gl4device.hh
        gl4/gl4directionallight.cc
        gl4/gl4directionallight.hh
        gl4/gl4exceptions.cc
		gl4/gl4exceptions.hh
        gl4/gl4indexbuffer.cc
        gl4/gl4indexbuffer.hh
        gl4/gl4pixelformatmap.hh
        gl4/gl4pointlight.cc
        gl4/gl4pointlight.hh
        gl4/gl4rendermesh.cc
        gl4/gl4rendermesh.hh
		gl4/gl4rendertarget2d.cc
		gl4/gl4rendertarget2d.hh
		gl4/gl4rendertarget2darray.cc
		gl4/gl4rendertarget2darray.hh
		gl4/gl4rendertargetcube.cc
		gl4/gl4rendertargetcube.hh
        gl4/gl4sampler.cc
        gl4/gl4sampler.hh
        gl4/gl4texture2d.cc
        gl4/gl4texture2d.hh
        gl4/gl4texture2darray.cc
        gl4/gl4texture2darray.hh
		gl4/gl4texturecube.cc
		gl4/gl4texturecube.hh
		gl4/gl4terrainmesh.cc
		gl4/gl4terrainmesh.hh
		gl4/gl4terrainmesh_cpu.cc
		gl4/gl4terrainmesh_cpu.hh
		gl4/gl4terrainmesh_gpu.cc
		gl4/gl4terrainmesh_gpu.hh
        gl4/gl4vertexbuffer.cc
        gl4/gl4vertexbuffer.hh


		gl4/loading/gl4programloader.cc
		gl4/loading/gl4programloader.hh
		gl4/loading/gl4shaderloader.cc
		gl4/loading/gl4shaderloader.hh


		gl4/pipeline/gl4meshsorter.cc
		gl4/pipeline/gl4meshsorter.hh

		gl4/pipeline/deferred/gl4deferreddirectionallightrenderer.cc
		gl4/pipeline/deferred/gl4deferreddirectionallightrenderer.hh
		gl4/pipeline/deferred/gl4deferredpipeline.cc
		gl4/pipeline/deferred/gl4deferredpipeline.hh
		gl4/pipeline/deferred/gl4deferredpointlightrenderer.cc
		gl4/pipeline/deferred/gl4deferredpointlightrenderer.hh

		gl4/pipeline/forward/gl4forwarddirectionallightrenderer.cc
		gl4/pipeline/forward/gl4forwarddirectionallightrenderer.hh
		gl4/pipeline/forward/gl4forwardpipeline.cc
		gl4/pipeline/forward/gl4forwardpipeline.hh
		gl4/pipeline/forward/gl4forwardpointlightrenderer.cc
		gl4/pipeline/forward/gl4forwardpointlightrenderer.hh

		gl4/pipeline/pointsm/gl4pointsmfilter.cc
		gl4/pipeline/pointsm/gl4pointsmfilter.hh
		gl4/pipeline/pointsm/gl4pointsmrenderer.cc
		gl4/pipeline/pointsm/gl4pointsmrenderer.hh

		gl4/pipeline/pssm/gl4pssmrenderer.cc
		gl4/pipeline/pssm/gl4pssmrenderer.hh
		gl4/pipeline/pssm/gl4pssmfilter.cc
		gl4/pipeline/pssm/gl4pssmfilter.hh

		gl4/shadergraph/gl4shadergraphcompiler.cc
		gl4/shadergraph/gl4shadergraphcompiler_depth.cc
		gl4/shadergraph/gl4shadergraphcompiler_forward.cc
		gl4/shadergraph/gl4shadergraphcompiler_gbuffer.cc
		gl4/shadergraph/gl4shadergraphcompiler_variables.cc
		gl4/shadergraph/gl4shadergraphcompiler.hh


		gl4/shading/gl4program.cc
		gl4/shading/gl4program.hh
		gl4/shading/gl4shader.cc
		gl4/shading/gl4shader.hh
		gl4/shading/gl4shaderattribute.cc
		gl4/shading/gl4shaderattribute.hh
		gl4/shading/gl4shaderattributenull.cc
		gl4/shading/gl4shaderattributenull.hh
		gl4/shading/gl4shadersourcefragment.cc
		gl4/shading/gl4shadersourcefragment.hh
		gl4/shading/gl4shadertype.hh
        )


add_library(ceOpenGL SHARED
        ${SOURCES}
        )

target_link_libraries(ceOpenGL
        ceCore
        tinyxml2
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        )

add_dependencies(ceOpenGL ceMOC)
CS_MOC(ceOpenGL)


install(TARGETS ceOpenGL
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/ceOpenGL FILES_MATCHING PATTERN "*.hh")


