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

add_library(csGame SHARED
        ${SOURCES}
        )


target_link_libraries(csGame
        csCore
        )

add_dependencies(csGame csMOC)
CS_MOC(csGame)


install(TARGETS csGame
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/csImgLoader FILES_MATCHING PATTERN "*.hh")


