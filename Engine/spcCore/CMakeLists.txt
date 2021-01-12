

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
        outofboundsexception.cc
        outofboundsexception.hh
        string.cc
        string.hh
        types.hh

        entity/entity.cc
        entity/entity.hh
        entity/entitystate.cc
        entity/entitystate.hh
        entity/spatialstate.cc
        entity/spatialstate.hh
        entity/world.cc
        entity/world.hh


        graphics/camera.cc
        graphics/camera.hh
        graphics/ebufferusage.hh
        graphics/edatatype.hh
        graphics/epixelformat.hh
        graphics/eprimitivetype.hh
        graphics/eprojectionmode.hh
        graphics/erenderpass.hh
        graphics/eshaderattribute.hh
        graphics/etextureunit.hh
        graphics/etexturetype.hh
        graphics/evertexstream.hh
        graphics/graphicsexceptions.cc
        graphics/graphicsexceptions.hh
        graphics/idevice.hh
        graphics/idirectionallight.hh
        graphics/iindexbuffer.hh
        graphics/ilight.hh
        graphics/image.cc
        graphics/image.hh
        graphics/ipointlight.hh
        graphics/irendermesh.hh
        graphics/itexture.hh
        graphics/itexture2d.hh
        graphics/ivertexbuffer.hh
        graphics/mesh.cc
        graphics/mesh.hh
        graphics/projector.cc
        graphics/projector.hh
        graphics/vertexdeclaration.cc
        graphics/vertexdeclaration.hh

        graphics/loader/meshloader.cc
        graphics/loader/meshloader.hh

        graphics/material/ematerialattributetype.hh
        graphics/material/imaterial.hh
        graphics/material/material.cc
        graphics/material/material.hh
        graphics/material/materialinstance.cc
        graphics/material/materialinstance.hh

        graphics/scene/scene.cc
        graphics/scene/scene.hh
        graphics/scene/scenemesh.cc
        graphics/scene/scenemesh.hh

        graphics/shading/ishader.hh
        graphics/shading/ishaderattribute.hh

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

        resource/assetmanager.cc
        resource/assetmanager.hh
        resource/binarydata.cc
        resource/binarydata.hh
        resource/eaccessmode.hh
        resource/eopenmode.hh
        resource/eseekmode.hh
        resource/file.cc
        resource/file.hh
        resource/filesystemfile.cc
        resource/filesystemfile.hh
        resource/iassetloader.hh
        resource/ifile.hh
        resource/resourceexceptions.cc
        resource/resourceexceptions.hh
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
SPC_MOC(spcCore)


install(TARGETS spcCore
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY . DESTINATION include/spcCore FILES_MATCHING PATTERN "*.hh")



