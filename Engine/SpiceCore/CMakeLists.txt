

add_definitions(-DSPICE_CORE_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
    defs.hh
    diamond.hh
    coreexport.hh
    coremodule.cc
    coremodule.hh
    objectregistry.cc
    objectregistry.hh
    types.hh

    math/boundingbox.cc
    math/boundingbox.hh
    math/clipper.cc
    math/clipper.hh
    math/color4f.hh
    math/hysteresis.hh
    math/math.hh
    math/matrix.hh
    math/matrix3f.hh
    math/matrix4f.hh
    math/plane.hh
    math/planeclipper.cc
    math/planeclipper.hh
    math/randomrange.hh
    math/vector.hh
    math/vector2f.hh
    math/vector3f.hh
    math/vector4f.hh

    window/wnddesc.hh
    window/iwindow.hh
    )

add_library(SpiceCore SHARED
    ${SOURCES}
    )

target_link_libraries(SpiceCore
        SpiceRefl
        ${PNG_LIBRARIES}

    )

add_dependencies(SpiceCore  SpiceMOC)
SPICE_MOC(SpiceCore SpiceCore)



install(TARGETS SpiceCore 
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY . DESTINATION include/SpiceCore FILES_MATCHING PATTERN "*.hh")



