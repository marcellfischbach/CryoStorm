

add_definitions(-DCS_IMGLOADER_EXPORT)
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

add_library(ceImgLoader SHARED
        ${SOURCES}
        )

message("JPEG libraries: ${JPEG_LIBRARIES}")

target_link_libraries(ceImgLoader
        ceCore
        ${PNG_LIBRARIES}
        ${JPEG_LIBRARIES}
        )

add_dependencies(ceImgLoader ceMOC)
CS_MOC(ceImgLoader)


install(TARGETS ceImgLoader
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/ceImgLoader FILES_MATCHING PATTERN "*.hh")


