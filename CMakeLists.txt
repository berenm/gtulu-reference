cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CMakeExtraBootstrap)
include(GNUInstallDirs)
include(FindOpenGL)
include(AddSubmodule)
include(FindCXX11)
include(CheetahGenerate)
include(FixCCache)
include(FixStaticLink)

project(libgtulu_api CXX)
# parse_version(libgtulu_api DEBIAN)

if(NOT libgtulu_api_VERSION)
  set(libgtulu_api_VERSION 4.3-comp)
endif()

add_submodule(libcorefungi git://github.com/berenm/libcorefungi.git lib/corefungi BRANCH master INCLUDE_DIRS include)
include_directories(${libcorefungi_INCLUDE_DIRS})

add_submodule(liblogging git://github.com/berenm/liblogging.git lib/logging BRANCH master INCLUDE_DIRS include lib/boost-log)
include_directories(${liblogging_INCLUDE_DIRS})

find_package(PkgConfig REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})

# -------------------------------------------------------------------------
# libgtulu_api
set(GTULU_API_SOURCE_DIR egl/1.4)
add_subdirectory(src gtulu/${GTULU_API_SOURCE_DIR})

set(GTULU_API_SOURCE_DIR gl/4.3/comp)
add_subdirectory(src gtulu/${GTULU_API_SOURCE_DIR})


enable_testing()

pkg_check_modules(EGL REQUIRED x11 egl)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})
add_executable(test-gtulu-api-link-gl-none test/test1.cpp)
target_link_libraries(test-gtulu-api-link-gl-none gtulu-api-egl gtulu-api-gl ${EGL_LIBRARIES})
add_test(test-gtulu-api-link-gl-none test-gtulu-api-link-gl-none)

add_executable(test-gtulu-api-link-gl-after test/test1.cpp)
target_link_libraries(test-gtulu-api-link-gl-after gtulu-api-egl gtulu-api-gl ${OPENGL_LIBRARIES})
add_test(test-gtulu-api-link-gl-after test-gtulu-api-link-gl-after)

add_executable(test-gtulu-api-link-gl-before test/test1.cpp)
target_link_libraries(test-gtulu-api-link-gl-before ${OPENGL_LIBRARIES} gtulu-api-egl gtulu-api-gl)
add_test(test-gtulu-api-link-gl-before test-gtulu-api-link-gl-before)

if(ENABLE_GCOVERALLS)
  include(GCoveralls)
  enable_gcoveralls()
endif()
