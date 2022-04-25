cmake_minimum_required(VERSION 3.15.5)

project(OpenglPlayground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

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

subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)

foreach(SUBDIR ${SUBDIRS})
  add_subdirectory(src/${SUBDIR})
endforeach()
