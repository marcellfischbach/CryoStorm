

add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
    main.cc
    launchermodule.cc
    launchermodule.hh

    window/sdlinputsystem.cc
    window/sdlinputsystem.hh
    window/sdlkeyboard.cc
    window/sdlkeyboard.hh
    window/sdlmouse.cc
    window/sdlmouse.hh
    )


add_executable(SpiceLauncher ${SOURCES}
    )

add_dependencies(SpiceLauncher SpiceMOC)


SPICE_MOC(SpiceLauncher SpiceLauncher)



target_link_libraries(SpiceLauncher 
        SpiceCore
        SpiceOpenGL
        ${SDL_LIBRARY}
    )
