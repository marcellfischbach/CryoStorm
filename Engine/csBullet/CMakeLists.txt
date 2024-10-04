

add_definitions(-DCS_BULLET_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        csBulletCollisionShape.cc
        csBulletCollisionShape.hh
        csBulletDynamicCollider.cc
        csBulletDynamicCollider.hh
        csBulletExport.hh
        csBulletModule.cc
        csBulletModule.hh
        csBulletStaticCollider.cc
        csBulletStaticCollider.hh
        csBulletSystem.cc
        csBulletSystem.hh
        csBulletUtils.hh
        csBulletWorld.cc
        csBulletWorld.hh
        )


add_library(ceBullet SHARED
        ${SOURCES}
        )

target_link_libraries(ceBullet
        ceCore
        ${BULLET_LIBRARIES}
        )

add_dependencies(ceBullet ceMOC)
CS_MOC(ceBullet)


install(TARGETS ceBullet
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/ceBullet FILES_MATCHING PATTERN "*.hh")


