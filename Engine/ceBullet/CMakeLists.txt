

add_definitions(-DCE_BULLET_EXPORT)
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


add_library(ceBullet SHARED
        ${SOURCES}
        )

target_link_libraries(ceBullet
        ceCore
        ${BULLET_LIBRARIES}
        )

add_dependencies(ceBullet ceMOC)
CE_MOC(ceBullet)


install(TARGETS ceBullet
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/ceBullet FILES_MATCHING PATTERN "*.hh")


