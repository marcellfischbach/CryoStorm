

add_definitions(-DSPC_FILE_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        file.hh
        file.cc
    )

add_library(spcFile SHARED
        ${SOURCES}
        )


add_executable(spcFileTest filetest.cc)
target_link_libraries(spcFileTest spcFile)


install(TARGETS spcFile
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/spcFile FILES_MATCHING PATTERN "*.hh")


