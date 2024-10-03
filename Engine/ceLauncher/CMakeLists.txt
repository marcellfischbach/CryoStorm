

add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        launchermodule.cc
        launchermodule.hh
        main.cc
        window/sdlbutton_map.hh
        window/sdlinputsystem.cc
        window/sdlinputsystem.hh
        window/sdlkey_map.hh
        window/sdlkeyboard.cc
        window/sdlkeyboard.hh
        window/sdlmouse.cc
        window/sdlmouse.hh
        window/sdlwindow.cc
        window/sdlwindow.hh

)


add_executable(ceLauncher ${SOURCES}
)

add_dependencies(ceLauncher
        ceMOC
        ceGame
)


CS_MOC(ceLauncher ceLauncher)


target_link_libraries(ceLauncher
        ceCore
        ${SDL_LIBRARY}
)

add_dependencies(ceLauncher ceAssimpLoader)
add_dependencies(ceLauncher ceBullet)
add_dependencies(ceLauncher ceImgLoader)
add_dependencies(ceLauncher ceOpenGL)
