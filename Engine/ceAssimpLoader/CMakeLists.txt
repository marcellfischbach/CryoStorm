

add_definitions(-DCE_ASSIMP_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        assimpexport.hh
        assimploadermodule.cc
        assimploadermodule.hh
        assimpmeshloader.cc
        assimpmeshloader.hh
    )

add_library(ceAssimpLoader SHARED
        ${SOURCES}
        )

target_link_libraries(ceAssimpLoader
        ceCore
        ${assimp_LIBRARIES}
        )

add_dependencies(ceAssimpLoader ceMOC)
CE_MOC(ceAssimpLoader)


install(TARGETS ceAssimpLoader
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/ceAssimpLoader FILES_MATCHING PATTERN "*.hh")


