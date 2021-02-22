
cmake_minimum_required(VERSION 3.3)


project(SpiceEngine)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SpiceEngine_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SpiceEngine_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SpiceEngine_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${SpiceEngine_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${SpiceEngine_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${SpiceEngine_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SpiceEngine_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${SpiceEngine_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${SpiceEngine_BINARY_DIR}/bin)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
unset(CMAKE_CXX_CL_SHOWINCLUDE)


include(${SpiceEngine_SOURCE_DIR}/Scripts/SpiceEngine.cmake) 

# SET CMakePolicy 0100 to new to include .hh files in automoc und autouic of QT5
#cmake_policy(SET CMP0100 NEW)

if (WIN32)
message("Use Win32")
add_definitions(-DSPC_WIN32)
else()
message("Use Linux")
add_definitions(-DSPC_LINUX)
endif()


string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER )
if(NOT Win32)
	if (CMAKE_BUILD_TYPE_LOWER STREQUAL "debug")
		add_definitions(-D_DEBUG)
	else()
		add_definitions(-DNDEBUG)
	endif()
endif()

find_package(OpenGL)
find_package(PNG)
find_package(JPEG)
find_package(Bullet)
#find_package(GLEW)



include(${SpiceEngine_SOURCE_DIR}/Scripts/Findassimp.cmake)
include(${SpiceEngine_SOURCE_DIR}/Scripts/FindGLEW.cmake)
include(${SpiceEngine_SOURCE_DIR}/Scripts/FindSDL2.cmake)
#include(${SpiceEngine_SOURCE_DIR}/Scripts/FindPhysX3.cmake)
#include(${SpiceEngine_SOURCE_DIR}/Scripts/FindBullet.cmake)


include_directories("3rdparty")
include_directories(${SDL_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${JPEG_INCLUDE_DIRS})
include_directories(${BULLET_INCLUDE_DIRS})
include_directories(${SpiceEngine_SOURCE_DIR}/Engine)
#include_directories(${SpiceEngine_BINARY_DIR}/Engine)
include_directories(${SpiceEngine_SOURCE_DIR}/Editor)
#include_directories(${SpiceEngine_BINARY_DIR}/Editor)


add_subdirectory(3rdparty)
#add_subdirectory(ClassGenerator)
add_subdirectory(Engine)
add_subdirectory(moc)
add_subdirectory(Editor)

