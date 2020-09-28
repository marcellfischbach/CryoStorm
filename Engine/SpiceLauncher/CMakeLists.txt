

add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
    main.cc
    )

add_executable(ceLauncher ${SOURCES}
    )

add_dependencies(ceLauncher cemoc)

target_link_libraries(ceLauncher 
	ceCore
    ceSDLWindow
    )


CE_MOC(ceLauncher celauncher)


