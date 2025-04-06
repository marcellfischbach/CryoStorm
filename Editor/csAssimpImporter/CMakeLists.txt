
add_definitions(-DCS_ASS_IMP_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

add_library(csAssimpImporter SHARED
        AssimpImporter.cc
        AssimpImporter.hh
        AssimpMeshExporter.cc
        AssimpMeshExporter.hh
        AssimpSkeletonExporter.cc
        AssimpSkeletonExporter.hh
        Export.hh
)

target_link_libraries(csAssimpImporter
        ${assimp_LIBRARIES}
        csCryoFile
)