

add_definitions(-DSPC_PNGLOADER_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        pngloaderexport.hh
        pngloadermodule.cc
        pngloadermodule.hh
        pngloader.cc
        pngloader.hh
    )

add_library(spcPngLoader SHARED
        ${SOURCES}
        )

target_link_libraries(spcPngLoader
        spcCore
        ${PNG_LIBRARIES}
        )

add_dependencies(spcPngLoader spcMOC)
SPICE_MOC(spcPngLoader spcPngLoader)


install(TARGETS spcPngLoader
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/spcPngLoader FILES_MATCHING PATTERN "*.hh")


