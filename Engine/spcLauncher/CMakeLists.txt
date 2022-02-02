

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


add_executable(spcLauncher ${SOURCES}
        )

add_dependencies(spcLauncher spcMOC)


SPC_MOC(spcLauncher spcLauncher)


target_link_libraries(spcLauncher
        spcCore
        spcBullet
        spcOpenGL
        spcImgLoader
        spcAssimpLoader
        ${SDL_LIBRARY}
        )
