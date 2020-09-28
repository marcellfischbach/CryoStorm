

add_definitions(-DSPICE_SDL_WINDOW_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
    sdlexport.hh
    sdlwindow.cc
    sdlwindow.hh
    sdlwindowmodule.cc
    sdlwindowmodule.hh
    )

add_library(SpiceSDLWindow SHARED
    ${SOURCES}
    )
target_link_libraries(SpiceSDLWindow
    SpiceRefl
    SpiceCore
    ${PNG_LIBRARIES}
    ${SDL_LIBRARY}
    )


add_dependencies(SpiceSDLWindow SpiceMOC)
SPICE_MOC(SpiceSDLWindow SpiceSDLWindow)

install(TARGETS SpiceSDLWindow 
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY . DESTINATION include/SpiceSDLWindow FILES_MATCHING PATTERN "*.hh")



