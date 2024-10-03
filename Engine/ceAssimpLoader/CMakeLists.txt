

add_definitions(-DCS_ASSIMP_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        assimpexport.hh
        assimploadermodule.cc
        assimploadermodule.hh
        assimpmaterialloader.cc
        assimpmaterialloader.hh
        assimpmeshloader.cc
        assimpmeshloader.hh
        assimpconverter.cc
        assimpconverter.hh
        assimpskeletonanimationloader.cc
        assimpskeletonanimationloader.hh
        assimpskeletonmeshloader.cc
        assimpskeletonmeshloader.hh
    )

add_library(ceAssimpLoader SHARED
        ${SOURCES}
        )

target_link_libraries(ceAssimpLoader
        ceCore
        ${assimp_LIBRARIES}
        )

add_dependencies(ceAssimpLoader ceMOC)
CS_MOC(ceAssimpLoader)


install(TARGETS ceAssimpLoader
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/ceAssimpLoader FILES_MATCHING PATTERN "*.hh")


