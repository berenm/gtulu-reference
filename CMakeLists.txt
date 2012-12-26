cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CMakeExtraBootstrap)
include(GNUInstallDirs)
include(FindOpenGL)
include(AddSubmodule)
include(FindCXX11)
include(CheetahGenerate)

project(libgtulu_api CXX)
# parse_version(libgtulu_api DEBIAN)

if(NOT libgtulu_api_VERSION)
  set(libgtulu_api_VERSION 3.3)
endif()

add_submodule(api-registry lib/registry INCLUDE_DIRS include)
add_submodule(api-man-gl2 man/gl/2)
add_submodule(api-man-gl3 man/gl/3)
add_submodule(api-man-gl4 man/gl/4)
add_submodule(logging lib/logging INCLUDE_DIRS include lib/boost-log)

find_package(PkgConfig REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})

# -------------------------------------------------------------------------
# libgtulu_api
include_directories(src ${CMAKE_CURRENT_BINARY_DIR}/src)

string(REGEX REPLACE "-.*$" "" API_VERSION ${libgtulu_api_VERSION})
string(REGEX REPLACE "^[^-]+-?(.*)$" "\\1" API_COMPAT  ${libgtulu_api_VERSION})
string(REGEX REPLACE "\\..*$" "" API_SOVERSION ${API_VERSION})
set(API_TARGET gtulu-api-${API_VERSION})

set(CHEETAH_ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/bin" "GTULU_VERSION=${API_VERSION}")
if(API_VERSION STREQUAL "2.1")
  add_submodule(${API_TARGET} gl/${API_VERSION})
  file(GLOB API_SOURCES gl/${API_VERSION}/function *)
else()
  if(API_COMPAT)
    list(APPEND CHEETAH_ENVIRONMENT "GTULU_COMPAT=1")
    add_submodule(${API_TARGET} gl/${API_VERSION}/comp)
    file(GLOB API_SOURCES gl/${API_VERSION}/comp/function *)
  else()
    add_submodule(${API_TARGET} gl/${API_VERSION}/core)
    file(GLOB API_SOURCES gl/${API_VERSION}/core/function *)
  endif()
endif()

cheetah_generate(${API_TARGET}-hpp src/gtulu/api.thpp OUTPUT_VARIABLE GTULU_API_HPP DEPENDS ${API_SOURCES}
  ENVIRONMENT ${CHEETAH_ENVIRONMENT}
)

cheetah_generate(${API_TARGET}-ni-cpp src/gtulu/api_ni.tcpp OUTPUT_VARIABLE GTULU_API_NI_CPP DEPENDS ${API_TARGET}-hpp ${API_SOURCES}
  ENVIRONMENT ${CHEETAH_ENVIRONMENT}
)
add_library(${API_TARGET}_ni SHARED ${GTULU_API_NI_CPP} src/gtulu/logging.cpp src/gtulu/notimp.cpp)
add_dependencies(${API_TARGET}_ni generate-api)
set_target_properties(${API_TARGET}_ni PROPERTIES
  VERSION       ${API_VERSION}
  SOVERSION     ${API_SOVERSION}
  SONAME        libgtulu-api-ni${API_SOVERSION}
  OUTPUT_NAME   gtulu-api-ni
  COMPILE_FLAGS "-fvisibility=hidden"
  LINK_FLAGS    "-Wl,--as-needed -Wl,--no-undefined"
)
target_link_libraries(${API_TARGET}_ni logging)

cheetah_generate(${API_TARGET}-cpp src/gtulu/api.tcpp OUTPUT_VARIABLE GTULU_API_CPP DEPENDS ${API_TARGET}-hpp ${API_SOURCES}
  ENVIRONMENT ${CHEETAH_ENVIRONMENT}
)
add_library(${API_TARGET} SHARED ${GTULU_API_CPP} src/gtulu/logging.cpp src/gtulu/error.cpp)
add_dependencies(${API_TARGET} generate-api)
set_target_properties(${API_TARGET} PROPERTIES
  VERSION       ${API_VERSION}
  SOVERSION     ${API_SOVERSION}
  SONAME        libgtulu-api${API_SOVERSION}
  OUTPUT_NAME   gtulu-api
  PUBLIC_HEADER ${GTULU_API_HPP}
  PUBLIC_HEADER src/gtulu/types.h
  COMPILE_FLAGS "-fvisibility=hidden"
  LINK_FLAGS    "-Wl,--as-needed -Wl,--no-undefined"
)
target_link_libraries(${API_TARGET} ${API_TARGET}_ni logging)

install(TARGETS ${API_TARGET} ${API_TARGET}_ni
  LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


enable_testing()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/gl/${API_VERSION}/core)
add_executable(test-link-gl-no test/test1.cpp)
target_link_libraries(test-link-gl-no ${API_TARGET})
add_test(test-link-gl-no test-link-gl-no)

add_executable(test-link-gl-after test/test1.cpp)
target_link_libraries(test-link-gl-after ${API_TARGET} GL)
add_test(test-link-gl-after test-link-gl-after)

add_executable(test-link-gl-before test/test1.cpp)
target_link_libraries(test-link-gl-before GL ${API_TARGET})
add_test(test-link-gl-before test-link-gl-before)
