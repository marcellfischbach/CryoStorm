


add_subdirectory(ceAssimpLoader)
add_subdirectory(ceBullet)
add_subdirectory(ceCore)
add_subdirectory(ceImgLoader)
add_subdirectory(ceLauncher)
add_subdirectory(ceOpenGL)

if (${JAVA_BINDINGS})
    add_subdirectory(ceJavaBinding)
endif()
