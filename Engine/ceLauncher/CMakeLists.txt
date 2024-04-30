

add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        camerahandler.cc
        camerahandler.hh
        demopostprocess.cc
        demopostprocess.hh
        launchermodule.cc
        launchermodule.hh
        main.cc
        mirrorhandler.cc
        mirrorhandler.hh
        testhandler.cc
        testhandler.hh

        ../ceSDLWindow/sdlinputsystem.cc
        ../ceSDLWindow/sdlinputsystem.hh
        ../ceSDLWindow/sdlkeyboard.cc
        ../ceSDLWindow/sdlkeyboard.hh
        ../ceSDLWindow/sdlmouse.cc
        ../ceSDLWindow/sdlmouse.hh
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
        ceSDLWindow
        ${SDL_LIBRARY}
        )
