


add_subdirectory(csAssimpLoader)
add_subdirectory(csBullet)
add_subdirectory(csCore)
add_subdirectory(csImgLoader)
add_subdirectory(csLauncher)
add_subdirectory(csOpenGL)

if (${JAVA_BINDINGS})
    add_subdirectory(csJavaLWJGL)
endif()
