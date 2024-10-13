

add_definitions(-DCS_ASSIMP_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        csAssimpExport.hh
        csAssimpLoaderModule.cc
        csAssimpLoaderModule.hh
        csAssimpMaterialLoader.cc
        csAssimpMaterialLoader.hh
        csAssimpMeshLoader.cc
        csAssimpMeshLoader.hh
        csAssimpConverter.cc
        csAssimpConverter.hh
        csAssimpSkeletonAnimationLoader.cc
        csAssimpSkeletonAnimationLoader.hh
        csAssimpSkeletonMeshLoader.cc
        csAssimpSkeletonMeshLoader.hh
    )

add_library(csAssimpLoader SHARED
        ${SOURCES}
        )

target_link_libraries(csAssimpLoader
        csCore
        ${assimp_LIBRARIES}
        )

add_dependencies(csAssimpLoader csMOC)
CS_MOC(csAssimpLoader "")


install(TARGETS csAssimpLoader
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/csAssimpLoader FILES_MATCHING PATTERN "*.hh")


