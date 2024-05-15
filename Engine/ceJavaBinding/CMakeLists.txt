

add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DCE_JAVA_EXPORT)

set(SOURCES
        javabindingmodule.cc
        javabindingmodule.hh
        lwjglinputsystem.cc
        lwjglinputsystem.hh
        lwjglkeyboard.cc
        lwjglkeyboard.hh
        lwjglmouse.cc
        lwjglmouse.hh
        lwjglwindow.cc
        lwjglwindow.hh
)


add_library(ceJavaBinding SHARED ${SOURCES}
)

add_dependencies(ceJavaBinding
        ceMOC
        ceGame
)
add_dependencies(ceJavaBinding ceAssimpLoader)
add_dependencies(ceJavaBinding ceBullet)
add_dependencies(ceJavaBinding ceImgLoader)
add_dependencies(ceJavaBinding ceOpenGL)

CE_MOC(ceJavaBinding ceJavaBinding)


target_link_libraries(ceJavaBinding
        ceCore
        ${JNI_LIBRARIES}
)


