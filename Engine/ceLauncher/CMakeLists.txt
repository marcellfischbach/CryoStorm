

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


        )


add_executable(ceLauncher ${SOURCES}
        )

add_dependencies(ceLauncher ceMOC)


CE_MOC(ceLauncher ceLauncher)


target_link_libraries(ceLauncher
        ceCore
        ${SDL_LIBRARY}
        )

#add_dependencies(ceLauncher ceAssimpLoader)
#add_dependencies(ceLauncher ceBullet)
#add_dependencies(ceLauncher ceImgLoader)
#add_dependencies(ceLauncher ceOpenGL)
#add_dependencies(ceLauncher ceSDLWindow)
