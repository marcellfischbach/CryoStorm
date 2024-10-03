add_definitions(-D_USE_MATH_DEFINES)


set(SOURCES
        camerahandler.cc
        camerahandler.hh
        demopostprocess.cc
        demopostprocess.hh
        exitgamestate.cc
        exitgamestate.hh
        game.cc
        game.hh
        gamemodule.cc
        gamemodule.hh
        mirrorhandler.cc
        mirrorhandler.hh
        testhandler.cc
        testhandler.hh
    )

add_library(ceGame SHARED
        ${SOURCES}
        )


target_link_libraries(ceGame
        ceCore
        )

add_dependencies(ceGame ceMOC)
CS_MOC(ceGame)


install(TARGETS ceGame
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/ceImgLoader FILES_MATCHING PATTERN "*.hh")


