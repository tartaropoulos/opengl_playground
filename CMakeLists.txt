cmake_minimum_required(VERSION 3.15.5)

project(OpenglPlayground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty")
set(THIRDPARTY_LIB "${THIRDPARTY_DIR}/Lib")

# Все аргументы для линковки
set(ALL_LIBS
  "${THIRDPARTY_LIB}/libglew32.a"
  "${THIRDPARTY_LIB}/libglfw3.a"
  "-lopengl32")

include_directories("${THIRDPARTY_DIR}/Include")

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
