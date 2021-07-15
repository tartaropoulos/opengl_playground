cmake_minimum_required(VERSION 3.15.5)

project(OpenglPlayground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty")

# Все аргументы для линковки
set(ALL_LIBS "-lfreeglut -lglew32 -lglfw3 -lopengl32")

include_directories("${THIRDPARTY_DIR}/Include")
link_directories("${THIRDPARTY_DIR}/Lib")

macro(subdirlist RESULT CURDIR)
  file(GLOB CHILDREN RELATIVE ${CURDIR} ${CURDIR}/*)
  set(DIRLIST "")
  foreach(CHILD ${CHILDREN})
    if(IS_DIRECTORY ${CURDIR}/${CHILD})
      list(APPEND DIRLIST ${CHILD})
    endif()
  endforeach()
  set(${RESULT} ${DIRLIST})
endmacro()

# macro(subdirlist RESULT)
#   file(GLOB CHILDREN RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
#   set(DIRLIST "")
#   foreach(CHILD ${CHILDREN})
#     if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${CHILD})
#       list(APPEND DIRLIST ${CHILD})
#     endif()
#   endforeach()
#   set(${RESULT} ${DIRLIST})
# endmacro()

subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)
# subdirlist(SUBDIRS)

foreach(SUBDIR ${SUBDIRS})
  add_subdirectory(src/${SUBDIR})
endforeach()

#################################################################

# target_link_libraries(glew_test -lfreeglut -lglew32 -lglfw3)

# set(CMAKE_EXE_LINKER_FLAGS "-I${THIRDPARTY_DIR}/Include -L${THIRDPARTY_DIR}/Lib -lfreeglut -lglew32")
# set(CMAKE_EXE_LINKER_FLAGS "-IZ:\\Programs\\C++\\Opengl\\first\\3rdparty\\Include -LZ:\\Programs\\C++\\Opengl\\first\\3rdparty\\Lib -lfreeglut -lglew32")

# include_directories(OPENGL_LIB "${THIRDPARTY_DIR}/Include")
# # link_directories(OPENGL_INCLUDE "${THIRDPARTY_DIR}/Lib")
# link_directories(
#     "${THIRDPARTY_DIR}/Lib/libglew32.a"
#     "${THIRDPARTY_DIR}/Lib/libfreeglut_static.a")

# find_package(GLUT REQUIRED)
# if (GLUT_FOUND)
#     include_directories(${GLUT_INCLUDE_DIRS})
#     link_libraries(${GLUT_LIBRARIES})
# endif()

# find_package(GLEW REQUIRED)
# if (GLEW_FOUND)
#     include_directories(${GLEW_INCLUDE_DIRS})
#     link_libraries(${GLEW_LIBRARIES})
# endif()

################################################################

# add_subdirectory(src/first)

################################################################

# add_executable(glew_test src/glew_test.cpp)
# target_link_libraries(src -lfreeglut -lglew32 -lglfw3)


