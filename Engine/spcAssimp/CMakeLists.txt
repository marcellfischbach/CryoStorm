

add_definitions(-DSPC_ASSIMP_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        assimpexport.hh
        assimpimageloader.cc
        assimpimageloader.hh
        assimpmodule.cc
        assimpmodule.hh
    )

add_library(spcAssimp SHARED
        ${SOURCES}
        )

target_link_libraries(spcAssimp
        spcCore
        ${assimp_LIBRARIES}
        ${PNG_LIBRARIES}
        )

add_dependencies(spcAssimp spcMOC)
SPICE_MOC(spcAssimp spcAssimp)


install(TARGETS spcAssimp
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/spcAssimp FILES_MATCHING PATTERN "*.hh")


