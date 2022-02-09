

add_definitions(-DSPC_BULLET_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        bulletcollisionshape.cc
        bulletcollisionshape.hh
        bulletdynamiccollider.cc
        bulletdynamiccollider.hh
        bulletexport.hh
        bulletmodule.cc
        bulletmodule.hh
        bulletstaticcollider.cc
        bulletstaticcollider.hh
        bulletsystem.cc
        bulletsystem.hh
        bulletutils.hh
        bulletworld.cc
        bulletworld.hh
        )


add_library(spcBullet SHARED
        ${SOURCES}
        )

target_link_libraries(spcBullet
        spcCore
        ${BULLET_LIBRARIES}
        )

add_dependencies(spcBullet spcMOC)
SPC_MOC(spcBullet)


install(TARGETS spcBullet
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/spcBullet FILES_MATCHING PATTERN "*.hh")


