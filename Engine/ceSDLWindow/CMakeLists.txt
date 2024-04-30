

add_definitions(-DCE_SDL_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        sdlbutton_map.hh
        sdlinputsystem.cc
        sdlinputsystem.hh
        sdlkey_map.hh
        sdlkeyboard.cc
        sdlkeyboard.hh
        sdlmouse.cc
        sdlmouse.hh
        sdlwindow.cc
        sdlwindow.hh
        sdlwindowexport.hh
        sdlwindowmodule.cc
        sdlwindowmodule.hh

        )


add_library(ceSDLWindow SHARED
        ${SOURCES}
        )

target_link_libraries(ceSDLWindow
        ceCore
		${SDL_LIBRARY}
        )

add_dependencies(ceSDLWindow ceMOC)
CE_MOC(ceSDLWindow)


install(TARGETS ceSDLWindow
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/ceSDLWindow FILES_MATCHING PATTERN "*.hh")


