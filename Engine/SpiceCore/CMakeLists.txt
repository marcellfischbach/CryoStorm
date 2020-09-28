

add_definitions(-DSPICE_CORE_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
    cedefs.hh
    cediamond.hh
    ceexport.hh
    cetypes.hh

    math/ceboundingbox.cc
    math/ceboundingbox.hh
    math/ceclipper.cc
    math/ceclipper.hh
    math/cecolor4f.hh
    math/cehysteresis.hh
    math/cemath.hh
    math/cematrix.hh
    math/cematrix3f.hh
    math/cematrix4f.hh
    math/ceplane.hh
    math/ceplaneclipper.cc
    math/ceplaneclipper.hh
    math/cerandomrange.hh
    math/cevector.hh
    math/cevector2f.hh
    math/cevector3f.hh
    math/cevector4f.hh

    window/cewnddesc.hh
    window/iwindow.hh
    )

add_library(SpiceCore SHARED
    ${SOURCES}
    )

add_dependencies(SpiceCore cemoc)


install(TARGETS (SpiceCore 
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY . DESTINATION include/SpiceCore FILES_MATCHING PATTERN "*.hh")

target_link_libraries(SpiceCore 
	${PNG_LIBRARIES}
    )


CE_MOC(ceCore cecore)


