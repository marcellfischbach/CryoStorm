

add_definitions(-DCEW_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
    cesdlexport.hh
    cesdlwindow.cc
    cesdlwindow.hh
    cesdlwindowmodule.cc
    cesdlwindowmodule.hh
    )

add_library(ceSDLWindow SHARED
    ${SOURCES}
    )

add_dependencies(ceSDLWindow cemoc)


install(TARGETS ceSDLWindow 
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY . DESTINATION include/cesdlwindow FILES_MATCHING PATTERN "*.hh")

target_link_libraries(ceSDLWindow
    ceCore
	${PNG_LIBRARIES}
    ${SDL_LIBRARY}
    )


CE_MOC(ceSDLWindow cesdlwindow)


