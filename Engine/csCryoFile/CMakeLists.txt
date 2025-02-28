

add_definitions(-DCS_CRYOFILE_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        csCryoFile.cc
        csCryoFile.hh
        )


add_library(csCryoFile SHARED
        ${SOURCES}
        )



install(TARGETS csCryoFile
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/csCryoFile FILES_MATCHING PATTERN "*.hh")


