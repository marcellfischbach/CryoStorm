
cmake_minimum_required(VERSION 3.21)


project(CryoStorm)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CryoStorm_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CryoStorm_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CryoStorm_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CryoStorm_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CryoStorm_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CryoStorm_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CryoStorm_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CryoStorm_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CryoStorm_BINARY_DIR}/bin)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
unset(CMAKE_CXX_CL_SHOWINCLUDE)

set(DISABLE_RENDERING OFF)
set(JAVA_BINDINGS ON)


include(${CryoStorm_SOURCE_DIR}/Scripts/CryoStorm.cmake)

# SET CMakePolicy 0100 to new to include .hh files in automoc und autouic of QT5
#cmake_policy(SET CMP0100 NEW)

if (WIN32)
message("Use Win32")
add_definitions(-DCS_WIN32)
else()
message("Use Linux")
add_definitions(-DCS_LINUX)
endif()

if (${DISABLE_RENDERING})
add_definitions(-DCS_DISABLE_RENDERING)
endif()

if (${JAVA_BINDINGS})
	find_package(JNI)
	if(${JNI_FOUND})
		add_definitions(-DCS_JAVA)
		include_directories(${JNI_INCLUDE_DIRS})
	endif()
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



include(${CryoStorm_SOURCE_DIR}/Scripts/Findassimp.cmake)
include(${CryoStorm_SOURCE_DIR}/Scripts/FindGLEW.cmake)
include(${CryoStorm_SOURCE_DIR}/Scripts/FindSDL2.cmake)
#include(${CryoStorm_SOURCE_DIR}/Scripts/FindPhysX3.cmake)
include(${CryoStorm_SOURCE_DIR}/Scripts/FindBullet.cmake)


include_directories("3rdparty")
include_directories(${SDL_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${JPEG_INCLUDE_DIRS})
include_directories(${BULLET_INCLUDE_DIRS})
include_directories(${CryoStorm_SOURCE_DIR}/Engine)
#include_directories(${CryoStorm_BINARY_DIR}/Engine)
include_directories(${CryoStorm_SOURCE_DIR}/Editor)
#include_directories(${CryoStorm_BINARY_DIR}/Editor)


add_subdirectory(3rdparty)
#add_subdirectory(ClassGenerator)
add_subdirectory(Engine)
add_subdirectory(GameExamples)
add_subdirectory(moc)
add_subdirectory(Editor)

