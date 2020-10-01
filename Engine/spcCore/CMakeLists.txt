

add_definitions(-DSPC_CORE_EXPORT)
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

        graphics/datatype.hh
        graphics/igraphics.hh
        graphics/renderattribute.hh
        graphics/vertexdeclaration.cc
        graphics/vertexdeclaration.hh
        graphics/shading/ishader.hh
        graphics/shading/ishaderattribute.hh
        graphics/shading/ishaderloader.hh

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

        resource/resourcelocator.cc
        resource/resourcelocator.hh
        resource/vfs.cc
        resource/vfs.hh

        #window/wnddesc.hh
        window/iwindow.hh

        )


add_library(spcCore SHARED
        ${SOURCES}
        )

target_link_libraries(spcCore
        ${PNG_LIBRARIES}

        )

add_dependencies(spcCore spcMOC)
SPICE_MOC(spcCore spcCore)


install(TARGETS spcCore
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/spcCore FILES_MATCHING PATTERN "*.hh")



