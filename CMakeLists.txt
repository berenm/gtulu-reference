cmake_minimum_required(VERSION 2.8)
find_package(Git REQUIRED)
message(STATUS "updating libgtulu submodules...")
execute_process(COMMAND git submodule update --init WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_QUIET)

project(libgtulu CXX)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/KHR" OR "${GTULU_FORCE_DOWNLOAD}" EQUAL 1)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/KHR")
  message(STATUS "downloading KHR/khrplatform.h...")
  file(DOWNLOAD "http://www.khronos.org/registry/egl/api/KHR/khrplatform.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/EGL" OR "${GTULU_FORCE_DOWNLOAD}" EQUAL 1)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/EGL")
  message(STATUS "downloading EGL/egl.h...")
  file(DOWNLOAD "http://www.khronos.org/registry/egl/api/EGL/egl.h"         "${CMAKE_CURRENT_SOURCE_DIR}/include/EGL/egl.h")

  message(STATUS "downloading EGL/eglext.h...")
  file(DOWNLOAD "http://www.khronos.org/registry/egl/api/EGL/eglext.h"      "${CMAKE_CURRENT_SOURCE_DIR}/include/EGL/eglext.h")

  message(STATUS "downloading EGL/eglplatform.h...")
  file(DOWNLOAD "http://www.khronos.org/registry/egl/api/EGL/eglplatform.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/EGL/eglplatform.h")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/GLES2" OR "${GTULU_FORCE_DOWNLOAD}" EQUAL 1)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/GLES2")
  message(STATUS "downloading GLES2/gl2.h...")
  file(DOWNLOAD "http://www.khronos.org/registry/gles/api/2.0/gl2.h"         "${CMAKE_CURRENT_SOURCE_DIR}/include/GLES2/gl2.h")

  message(STATUS "downloading GLES2/gl2ext.h...")
  file(DOWNLOAD "http://www.khronos.org/registry/gles/api/2.0/gl2ext.h"      "${CMAKE_CURRENT_SOURCE_DIR}/include/GLES2/gl2ext.h")

  message(STATUS "downloading GLES2/gl2platform.h...")
  file(DOWNLOAD "http://www.khronos.org/registry/gles/api/2.0/gl2platform.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/GLES2/gl2platform.h")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/GL3" OR "${GTULU_FORCE_DOWNLOAD}" EQUAL 1)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/GL3")
  message(STATUS "downloading GL3/gl3.h...")
  file(DOWNLOAD "http://www.opengl.org/registry/api/gl3.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/GL3/gl3.h")
endif()
