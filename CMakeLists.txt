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
  set(libgtulu_api_VERSION 4.3-comp)
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
  set(API_SOURCE_DIR gl/${API_VERSION})
else()
  if(API_COMPAT)
    list(APPEND CHEETAH_ENVIRONMENT "GTULU_COMPAT=1")
    set(API_SOURCE_DIR gl/${API_VERSION}/comp)
  else()
    set(API_SOURCE_DIR gl/${API_VERSION}/core)
  endif()
endif()


add_submodule(${API_TARGET} ${API_SOURCE_DIR})

file(GLOB_RECURSE API_SOURCES bin/* ${API_SOURCE_DIR}/function/* ${API_SOURCE_DIR}/constant/*)
set(API_CHEETAH_PROPERTIES
  DEPENDS ${API_SOURCES}
  ENVIRONMENT ${CHEETAH_ENVIRONMENT}
)

cheetah_generate(${API_TARGET}-ni-cpp src/gtulu/api_ni.tcpp OUTPUT_VARIABLE GTULU_API_NI_CPP ${API_CHEETAH_PROPERTIES})
cheetah_generate(${API_TARGET}-hpp    src/gtulu/api.thpp    OUTPUT_VARIABLE GTULU_API_HPP ${API_CHEETAH_PROPERTIES})
cheetah_generate(${API_TARGET}-cpp    src/gtulu/api.tcpp    OUTPUT_VARIABLE GTULU_API_CPP ${API_CHEETAH_PROPERTIES})
set_source_files_properties(${GTULU_API_CPP} PROPERTIES OBJECT_DEPENDS ${GTULU_API_HPP})
set_source_files_properties(src/gtulu/error.cpp PROPERTIES OBJECT_DEPENDS ${GTULU_API_HPP})


add_definitions(-DGTULU_MAJOR_VERSION=${API_SOVERSION})
set(API_LIBRARY_PROPERTIES
  VERSION       ${API_VERSION}
  SOVERSION     ${API_SOVERSION}
  COMPILE_FLAGS "-fvisibility=hidden"
  LINK_FLAGS    "-Wl,--as-needed -Wl,--no-undefined"
)

add_library(${API_TARGET}_ni SHARED ${GTULU_API_NI_CPP} src/gtulu/logging.cpp src/gtulu/notimp.cpp)
set_target_properties(${API_TARGET}_ni PROPERTIES
  SONAME        libgtulu-api-ni${API_SOVERSION}
  OUTPUT_NAME   gtulu-api-ni
  ${API_LIBRARY_PROPERTIES}
)
target_link_libraries(${API_TARGET}_ni logging)


add_library(${API_TARGET} SHARED ${GTULU_API_HPP} ${GTULU_API_CPP} src/gtulu/logging.cpp src/gtulu/error.cpp)
set_target_properties(${API_TARGET} PROPERTIES
  SONAME        libgtulu-api${API_SOVERSION}
  OUTPUT_NAME   gtulu-api
  PUBLIC_HEADER ${GTULU_API_HPP}
  PUBLIC_HEADER src/gtulu/types.h
  ${API_LIBRARY_PROPERTIES}
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
