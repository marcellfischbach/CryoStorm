

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


add_library(csJavaLWJGL SHARED ${SOURCES}
)

add_dependencies(csJavaLWJGL
        csMOC
        csGame
)
add_dependencies(csJavaLWJGL csAssimpLoader)
add_dependencies(csJavaLWJGL csBullet)
add_dependencies(csJavaLWJGL csImgLoader)
add_dependencies(csJavaLWJGL csOpenGL)

CS_MOC(csJavaLWJGL csJavaLWJGL)


target_link_libraries(csJavaLWJGL
        csCore
        ${JNI_LIBRARIES}
)


