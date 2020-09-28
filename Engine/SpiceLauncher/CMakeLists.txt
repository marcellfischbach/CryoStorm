

add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
    main.cc

    window/sdlkeyboard.cc
    window/sdlkeyboard.hh
    )

add_executable(SpiceLauncher ${SOURCES}
    )

add_dependencies(SpiceLauncher SpiceMOC)


SPICE_MOC(SpiceLauncher SpiceLauncher)



target_link_libraries(SpiceLauncher 
        SpiceRefl
        SpiceCore
        ${SDL_LIBRARY}
    )
