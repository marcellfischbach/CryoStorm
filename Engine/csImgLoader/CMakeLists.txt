

add_definitions(-DCS_IMGLOADER_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        csImgLoaderExport.hh
        csImgLoaderModule.cc
        csImgLoaderModule.hh
        csJpegLoader.cc
        csJpegLoader.hh
        csPngLoader.cc
        csPngLoader.hh
    )

add_library(csImgLoader SHARED
        ${SOURCES}
        )

message("JPEG libraries: ${JPEG_LIBRARIES}")

target_link_libraries(csImgLoader
        csCore
        ${PNG_LIBRARIES}
        ${JPEG_LIBRARIES}
        )

add_dependencies(csImgLoader csMOC)
CS_MOC(csImgLoader)


install(TARGETS csImgLoader
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/csImgLoader FILES_MATCHING PATTERN "*.hh")


