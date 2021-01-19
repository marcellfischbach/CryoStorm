

add_definitions(-DSPC_ASSIMP_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        assimpexport.hh
        assimploadermodule.cc
        assimploadermodule.hh
        assimpmeshloader.cc
        assimpmeshloader.hh
    )

add_library(spcAssimpLoader SHARED
        ${SOURCES}
        )

target_link_libraries(spcAssimpLoader
        spcCore
        ${assimp_LIBRARIES}
        )

add_dependencies(spcAssimpLoader spcMOC)
SPC_MOC(spcAssimpLoader)


install(TARGETS spcAssimpLoader
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/spcAssimpLoader FILES_MATCHING PATTERN "*.hh")


