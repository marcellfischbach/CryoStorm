

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


