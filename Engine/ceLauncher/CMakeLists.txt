

add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        camerahandler.cc
        camerahandler.hh
        launchermodule.cc
        launchermodule.hh
        main.cc
        mirrorhandler.cc
        mirrorhandler.hh


        window/sdlinputsystem.cc
        window/sdlinputsystem.hh
        window/sdlkeyboard.cc
        window/sdlkeyboard.hh
        window/sdlmouse.cc
        window/sdlmouse.hh
        )


add_executable(ceLauncher ${SOURCES}
        )

add_dependencies(ceLauncher ceMOC)


CE_MOC(ceLauncher ceLauncher)


target_link_libraries(ceLauncher
        ceCore
        ceBullet
        ceOpenGL
        ceImgLoader
        ceAssimpLoader
        ${SDL_LIBRARY}
        )
