

message ("-- Looking for GLEW Library")
#find the include directory
find_path (GLEW_INCLUDE_DIR
          NAMES GL/glew.h
          )


find_library (GLEW_LIBRARIES
              NAMES GLEW GLEW32
              )

set (GLEW_FOUND "NO")
if (GLEW_LIBRARIES)
  set (GLEW_FOUND "YES")
endif()

if (GLEW_FOUND)
  message ("-- Looking for GLEW Library - found.")
else ()
  message ("-- Looking for GLEW Library - not found.")
endif()

