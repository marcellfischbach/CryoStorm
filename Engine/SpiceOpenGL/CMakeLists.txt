

add_definitions(-DSPICE_OGL_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
    openglexport.hh
    openglmodule.cc
    openglmodule.hh
    gl4/gl4graphics.cc
    gl4/gl4graphics.hh
    )

add_library(SpiceOpenGL SHARED
    ${SOURCES}
    )

target_link_libraries(SpiceOpenGL
        SpiceCore
    	${OPENGL_LIBRARIES}
    	${GLEW_LIBRARIES}

    )

add_dependencies(SpiceOpenGL  SpiceMOC)
SPICE_MOC(SpiceOpenGL SpiceOpenGL)



install(TARGETS SpiceOpenGL 
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY . DESTINATION include/SpiceOpenGL FILES_MATCHING PATTERN "*.hh")



