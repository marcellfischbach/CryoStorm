

add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
    main.cc
    )

add_executable(SpiceLauncher ${SOURCES}
    )

add_dependencies(SpiceLauncher SpiceMOC)

target_link_libraries(SpiceLauncher 
        SpiceRefl
        SpiceCore
    	SpiceSDLWindow
    )


SPICE_MOC(SpiceLauncher SpiceLauncher)


