

add_definitions(-DSPC_OGL_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        openglexport.hh
        openglmodule.cc
        openglmodule.hh

		gl4/gl4exceptions.cc
		gl4/gl4exceptions.hh
        gl4/gl4graphics.cc
        gl4/gl4graphics.hh
        gl4/gl4rendermesh.cc
        gl4/gl4rendermesh.hh


		gl4/loading/gl4shaderloader.cc
		gl4/loading/gl4shaderloader.hh

		gl4/shading/gl4program.cc
		gl4/shading/gl4program.hh
		gl4/shading/gl4shader.cc
		gl4/shading/gl4shader.hh
		gl4/shading/gl4shaderattribute.cc
		gl4/shading/gl4shaderattribute.hh
		gl4/shading/gl4shadertype.hh
        )

add_library(spcOpenGL SHARED
        ${SOURCES}
        )

target_link_libraries(spcOpenGL
        spcCore
        tinyxml2
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        )

add_dependencies(spcOpenGL spcMOC)
SPICE_MOC(spcOpenGL spcOpenGL)


install(TARGETS spcOpenGL
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/spcOpenGL FILES_MATCHING PATTERN "*.hh")


