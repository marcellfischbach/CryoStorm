

add_definitions(-DSPC_IMGLOADER_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        imgloaderexport.hh
        imgloadermodule.cc
        imgloadermodule.hh
        jpegloader.cc
        jpegloader.hh
        pngloader.cc
        pngloader.hh
    )

add_library(spcImgLoader SHARED
        ${SOURCES}
        )

target_link_libraries(spcImgLoader
        spcCore
        ${PNG_LIBRARIES}
        ${JPEG_LIBRARIES}
        )

add_dependencies(spcImgLoader spcMOC)
SPICE_MOC(spcImgLoader spcImgLoader)


install(TARGETS spcImgLoader
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/spcImgLoader FILES_MATCHING PATTERN "*.hh")


