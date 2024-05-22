

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


add_library(ceJavaLWJGL SHARED ${SOURCES}
)

add_dependencies(ceJavaLWJGL
        ceMOC
        ceGame
)
add_dependencies(ceJavaLWJGL ceAssimpLoader)
add_dependencies(ceJavaLWJGL ceBullet)
add_dependencies(ceJavaLWJGL ceImgLoader)
add_dependencies(ceJavaLWJGL ceOpenGL)

CE_MOC(ceJavaLWJGL ceJavaLWJGL)


target_link_libraries(ceJavaLWJGL
        ceCore
        ${JNI_LIBRARIES}
)


