

add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DCS_JAVA_EXPORT)

set(SOURCES
        csJavaBindingModule.cc
        csJavaBindingModule.hh
        csLwjglInputSystem.cc
        csLwjglInputSystem.hh
        csLwjglKeyboard.cc
        csLwjglKeyboard.hh
        csLwjglMouse.cc
        csLwjglMouse.hh
        csLwjglWindow.cc
        csLwjglWindow.hh
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

CS_MOC(ceJavaLWJGL ceJavaLWJGL)


target_link_libraries(ceJavaLWJGL
        ceCore
        ${JNI_LIBRARIES}
)


