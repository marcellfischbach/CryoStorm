

add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES
        csLauncherModule.cc
        csLauncherModule.hh
        main.cc
        Test.cc
        Test.hh
        window/csSDLButtonMap.hh
        window/csSDLInputSystem.cc
        window/csSDLInputSystem.hh
        window/csSDLKeyMap.hh
        window/csSDLKeyboard.cc
        window/csSDLKeyboard.hh
        window/csSDLMouse.cc
        window/csSDLMouse.hh
        window/csSDLWindow.cc
        window/csSDLWindow.hh

)


add_executable(csLauncher ${SOURCES}
)

add_dependencies(csLauncher
        csMOC
        csGame
)


CS_MOC(csLauncher "")


target_link_libraries(csLauncher
        csCore
        ${SDL_LIBRARY}
)

add_dependencies(csLauncher csAssimpLoader)
add_dependencies(csLauncher csBullet)
add_dependencies(csLauncher csImgLoader)
add_dependencies(csLauncher csOpenGL)
