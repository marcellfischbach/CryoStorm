
add_definitions(-DCS_XML_EXPORT)


add_library(csXml SHARED
        csXml.cc
        csXml.hh
)



install(TARGETS csXml
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
)

install(DIRECTORY . DESTINATION include/csXml FILES_MATCHING PATTERN "*.hh")

add_executable(csXmlTest main.cc)

target_link_libraries(csXmlTest csXml)

