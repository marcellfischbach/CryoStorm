

add_definitions(-DSPICE_CORE_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
    class.cc
    class.hh
    classregistry.cc
    classregistry.hh
    defs.hh
    diamond.hh
    coreexport.hh
    coremodule.cc
    coremodule.hh
    objectregistry.cc
    objectregistry.hh
    types.hh

    graphics/igraphics.hh

    input/iinputsystem.hh
    input/ikeyboard.hh
    input/imouse.hh
    input/input.cc
    input/input.hh
    input/key.hh

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

    #window/wnddesc.hh
    window/iwindow.hh
    )


add_library(SpiceCore SHARED
    ${SOURCES}
    )

target_link_libraries(SpiceCore
        ${PNG_LIBRARIES}

    )

add_dependencies(SpiceCore  SpiceMOC)
SPICE_MOC(SpiceCore SpiceCore)

get_property(dirs TARGET SpiceCore PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()


install(TARGETS SpiceCore 
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY . DESTINATION include/SpiceCore FILES_MATCHING PATTERN "*.hh")



