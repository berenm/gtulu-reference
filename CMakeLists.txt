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

string(REGEX REPLACE "-.*$" ""             GTULU_VERSION ${libgtulu_api_VERSION})
string(REGEX REPLACE "^[^-]+-?(.*)$" "\\1" GTULU_VERSION_COMPAT  ${libgtulu_api_VERSION})
string(REGEX REPLACE "\\..*$" ""           GTULU_VERSION_MAJOR ${GTULU_VERSION})
string(REGEX REPLACE "^.*\\." ""           GTULU_VERSION_MINOR ${GTULU_VERSION})
if(GTULU_VERSION_COMPAT)
  set(GTULU_VERSION_COMPAT 1)
else()
  set(GTULU_VERSION_COMPAT 0)
endif()

set(GTULU_TARGET_PREFIX gtulu-api-${GTULU_VERSION})

set(CHEETAH_ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/bin" "GTULU_VERSION=${GTULU_VERSION}")
if(GTULU_VERSION STREQUAL "2.1")
  set(GTULU_API_SOURCE_DIR gl/${GTULU_VERSION})
else()
  if(GTULU_VERSION_COMPAT)
    list(APPEND CHEETAH_ENVIRONMENT "GTULU_COMPAT=1")
    set(GTULU_API_SOURCE_DIR gl/${GTULU_VERSION}/comp)
  else()
    set(GTULU_API_SOURCE_DIR gl/${GTULU_VERSION}/core)
  endif()
endif()


add_submodule(${GTULU_TARGET_PREFIX} ${GTULU_API_SOURCE_DIR})

file(GLOB_RECURSE API_SOURCES bin/* ${GTULU_API_SOURCE_DIR}/function/* ${GTULU_API_SOURCE_DIR}/constant/*)
set(GTULU_CHEETAH_PROPERTIES
  DEPENDS ${API_SOURCES}
  ENVIRONMENT ${CHEETAH_ENVIRONMENT}
)

cheetah_generate(${GTULU_TARGET_PREFIX}-ni-cpp src/gtulu/api_ni.tcpp OUTPUT_VARIABLE GTULU_API_NI_CPP ${GTULU_CHEETAH_PROPERTIES})
cheetah_generate(${GTULU_TARGET_PREFIX}-hpp    src/gtulu/api.thpp    OUTPUT_VARIABLE GTULU_API_HPP    ${GTULU_CHEETAH_PROPERTIES})
cheetah_generate(${GTULU_TARGET_PREFIX}-cpp    src/gtulu/api.tcpp    OUTPUT_VARIABLE GTULU_API_CPP    ${GTULU_CHEETAH_PROPERTIES})
set_source_files_properties(${GTULU_API_CPP}    PROPERTIES OBJECT_DEPENDS ${GTULU_API_HPP})
set_source_files_properties(src/gtulu/error.cpp PROPERTIES OBJECT_DEPENDS ${GTULU_API_HPP})


add_definitions(
  -DGTULU_VERSION_MAJOR=${GTULU_VERSION_MAJOR}
  -DGTULU_VERSION_MINOR=${GTULU_VERSION_MINOR}
  -DGTULU_VERSION_COMPAT=${GTULU_VERSION_COMPAT}
)
set(GTULU_LIBRARY_PROPERTIES
  VERSION       ${GTULU_VERSION}
  SOVERSION     ${GTULU_VERSION_MAJOR}
  COMPILE_FLAGS "-fvisibility=hidden"
  LINK_FLAGS    "-Wl,--as-needed -Wl,--no-undefined"
)

add_library(${GTULU_TARGET_PREFIX}_ni SHARED ${GTULU_API_NI_CPP} src/gtulu/logging.cpp src/gtulu/notimp.cpp)
set_target_properties(${GTULU_TARGET_PREFIX}_ni PROPERTIES
  SONAME        libgtulu-api-ni${GTULU_VERSION_MAJOR}
  OUTPUT_NAME   gtulu-api-ni
  ${GTULU_LIBRARY_PROPERTIES}
)
target_link_libraries(${GTULU_TARGET_PREFIX}_ni logging)


add_library(${GTULU_TARGET_PREFIX} SHARED ${GTULU_API_HPP} ${GTULU_API_CPP} src/gtulu/logging.cpp src/gtulu/error.cpp)
set_target_properties(${GTULU_TARGET_PREFIX} PROPERTIES
  SONAME        libgtulu-api${GTULU_VERSION_MAJOR}
  OUTPUT_NAME   gtulu-api
  PUBLIC_HEADER ${GTULU_API_HPP}
  PUBLIC_HEADER src/gtulu/types.h
  ${GTULU_LIBRARY_PROPERTIES}
)
target_link_libraries(${GTULU_TARGET_PREFIX} ${GTULU_TARGET_PREFIX}_ni logging)

install(TARGETS ${GTULU_TARGET_PREFIX} ${GTULU_TARGET_PREFIX}_ni
  LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


enable_testing()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/gl/${GTULU_VERSION}/core)
add_executable(test-link-gl-no test/test1.cpp)
target_link_libraries(test-link-gl-no ${GTULU_TARGET_PREFIX})
add_test(test-link-gl-no test-link-gl-no)

add_executable(test-link-gl-after test/test1.cpp)
target_link_libraries(test-link-gl-after ${GTULU_TARGET_PREFIX} GL)
add_test(test-link-gl-after test-link-gl-after)

add_executable(test-link-gl-before test/test1.cpp)
target_link_libraries(test-link-gl-before GL ${GTULU_TARGET_PREFIX})
add_test(test-link-gl-before test-link-gl-before)
