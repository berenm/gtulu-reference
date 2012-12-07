cmake_minimum_required(VERSION 2.8)

project(registry CXX)

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

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/GLES3" OR "${GTULU_FORCE_DOWNLOAD}" EQUAL 1)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/GLES3")
  message(STATUS "downloading GLES3/gl3.h...")
  file(DOWNLOAD "http://www.khronos.org/registry/gles/api/3.0/gl3.h"         "${CMAKE_CURRENT_SOURCE_DIR}/include/GLES3/gl3.h")

  message(STATUS "downloading GLES3/gl3ext.h...")
  file(DOWNLOAD "http://www.khronos.org/registry/gles/api/3.0/gl3ext.h"      "${CMAKE_CURRENT_SOURCE_DIR}/include/GLES3/gl3ext.h")

  message(STATUS "downloading GLES3/gl3platform.h...")
  file(DOWNLOAD "http://www.khronos.org/registry/gles/api/3.0/gl3platform.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/GLES3/gl3platform.h")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/GL" OR "${GTULU_FORCE_DOWNLOAD}" EQUAL 1)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/GL")
  message(STATUS "downloading GL/glcorearb.h...")
  file(DOWNLOAD "http://www.opengl.org/registry/api/glcorearb.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/GL/glcorearb.h")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/share/gl/spec" OR "${GTULU_FORCE_DOWNLOAD}" EQUAL 1)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/share/gl/spec")
  message(STATUS "downloading enum.spec...")
  file(DOWNLOAD "http://www.opengl.org/registry/api/enum.spec" "${CMAKE_CURRENT_SOURCE_DIR}/share/gl/spec/enum.spec")

  message(STATUS "downloading enumext.spec...")
  file(DOWNLOAD "http://www.opengl.org/registry/api/enumext.spec" "${CMAKE_CURRENT_SOURCE_DIR}/share/gl/spec/enumext.spec")

  message(STATUS "downloading gl.spec...")
  file(DOWNLOAD "http://www.opengl.org/registry/api/gl.spec" "${CMAKE_CURRENT_SOURCE_DIR}/share/gl/spec/gl.spec")

  message(STATUS "downloading gl.tm...")
  file(DOWNLOAD "http://www.opengl.org/registry/api/gl.tm" "${CMAKE_CURRENT_SOURCE_DIR}/share/gl/spec/gl.tm")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/share/egl/spec" OR "${GTULU_FORCE_DOWNLOAD}" EQUAL 1)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/share/egl/spec")
  message(STATUS "downloading glenum.spec...")
  file(DOWNLOAD "http://www.khronos.org/registry/egl/api/eglenum.spec" "${CMAKE_CURRENT_SOURCE_DIR}/share/egl/spec/glenum.spec")
endif()
