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

subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)

foreach(SUBDIR ${SUBDIRS})
  add_subdirectory(src/${SUBDIR})
endforeach()
