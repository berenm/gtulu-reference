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
include_directories(src)

function(add_api_library API_SOURCE_DIR)
  string(REGEX REPLACE ".*(core|comp)$"           "\\1" API_TYPE      "${API_SOURCE_DIR}")
  string(REGEX REPLACE "^.*/([^/]+)/${API_TYPE}$" "\\1" API_VERSION   "${API_SOURCE_DIR}")
  string(REGEX REPLACE "\\..*$"                   ""    API_SOVERSION "${API_VERSION}")

  if(NOT API_TYPE MATCHES "core")
    return()
  endif()

  file(MAKE_DIRECTORY ${API_SOURCE_DIR})

  set(CHEETAH_ENVIRONMENT "GTULU_VERSION=${API_VERSION}" "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/bin")
  cheetah_generate(gtulu-api-${API_VERSION}-hpp src/gtulu/api.thpp
    OUTPUT_VARIABLE GTULU_API_HPP
    ENVIRONMENT ${CHEETAH_ENVIRONMENT}
    WORKING_DIRECTORY ${API_SOURCE_DIR}
  )
  cheetah_generate(gtulu-api-${API_VERSION}-cpp src/gtulu/api.tcpp
    OUTPUT_VARIABLE GTULU_API_CPP
    ENVIRONMENT ${CHEETAH_ENVIRONMENT}
    WORKING_DIRECTORY ${API_SOURCE_DIR}
    DEPENDS ${GTULU_API_HPP}
  )
  set(API_TARGET gtulu-api-${API_VERSION})
  add_library(${API_TARGET} SHARED ${GTULU_API_CPP} src/gtulu/logging.cpp src/gtulu/error.cpp)
  add_dependencies(${API_TARGET} generate-api)
  set_target_properties(${API_TARGET} PROPERTIES
    VERSION       ${API_VERSION}
    SOVERSION     ${API_SOVERSION}
    SONAME        libgtulu-api${API_SOVERSION}
    OUTPUT_NAME   gtulu-api
    PUBLIC_HEADER ${GTULU_API_HPP}
    PUBLIC_HEADER src/gtulu/types.h
    COMPILE_FLAGS "-fvisibility=hidden -I${API_SOURCE_DIR}/src/"
    LINK_FLAGS    "-Wl,--as-needed -Wl,--no-undefined"
  )

  cheetah_generate(gtulu-api-${API_VERSION}-ni-cpp src/gtulu/api_ni.tcpp
    OUTPUT_VARIABLE GTULU_API_NI_CPP
    ENVIRONMENT ${CHEETAH_ENVIRONMENT}
    WORKING_DIRECTORY ${API_SOURCE_DIR}
    DEPENDS ${GTULU_API_HPP}
  )
  add_library(${API_TARGET}_ni SHARED ${GTULU_API_NI_CPP} src/gtulu/logging.cpp src/gtulu/notimp.cpp)
  add_dependencies(${API_TARGET}_ni generate-api)
  set_target_properties(${API_TARGET}_ni PROPERTIES
    VERSION       ${API_VERSION}
    SOVERSION     ${API_SOVERSION}
    SONAME        libgtulu-api-ni${API_SOVERSION}
    OUTPUT_NAME   gtulu-api-ni
    COMPILE_FLAGS "-fvisibility=hidden -I${API_SOURCE_DIR}/src/"
    LINK_FLAGS    "-Wl,--as-needed -Wl,--no-undefined"
  )
  target_link_libraries(${API_TARGET}_ni logging)
  target_link_libraries(${API_TARGET} ${API_TARGET}_ni logging)

  install(TARGETS ${API_TARGET} ${API_TARGET}_ni
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
endfunction()


# One can define GTULU_API_VERSION to only build a single version
if(NOT GTULU_API_VERSION)
  file(GLOB_RECURSE API_SOURCE_DIRS gl/*/.git)
  list(SORT API_SOURCE_DIRS)

  set(API_SOURCES "")
  foreach(API_SOURCE_DIR ${API_SOURCE_DIRS})
    get_filename_component(API_SOURCE_DIR "${API_SOURCE_DIR}" PATH)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" API_SOURCE_DIR "${API_SOURCE_DIR}")

    list(APPEND API_SOURCES
      ${API_SOURCE_DIR}/gtulu/api.cpp
      ${API_SOURCE_DIR}/gtulu/api.hpp
      ${API_SOURCE_DIR}/gtulu/api.h
      ${API_SOURCE_DIR}/gtulu/api_ni.cpp
      ${API_SOURCE_DIR}/gtulu/types.h
    )
  endforeach()
else()
  set(API_SOURCE_DIR gl/${GTULU_API_VERSION}/core)
  set(API_SOURCES
    ${API_SOURCE_DIR}/gtulu/api.cpp
    ${API_SOURCE_DIR}/gtulu/api.hpp
    ${API_SOURCE_DIR}/gtulu/api.h
    ${API_SOURCE_DIR}/gtulu/api_ni.cpp
    ${API_SOURCE_DIR}/gtulu/types.h
  )
  set(API_SOURCE_DIRS ${API_SOURCE_DIR}/.git)

  add_submodule(gtulu-api-${GTULU_API_VERSION}      gl/${GTULU_API_VERSION}/core)
  add_submodule(gtulu-api-${GTULU_API_VERSION}-comp gl/${GTULU_API_VERSION}/comp)
endif()

foreach(API_SOURCE_DIR ${API_SOURCE_DIRS})
  get_filename_component(API_SOURCE_DIR "${API_SOURCE_DIR}" PATH)
  string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" API_SOURCE_DIR "${API_SOURCE_DIR}")

  add_api_library(${API_SOURCE_DIR})
endforeach()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/gl/${GTULU_API_VERSION}/core)
add_executable(test-link-nogl test/test1.cpp)
target_link_libraries(test-link-nogl gtulu-api-3.3)
add_test(test-link-nogl test-link-nogl)

add_executable(test-link-glafter test/test1.cpp)
target_link_libraries(test-link-glafter gtulu-api-3.3 GL)
add_test(test-link-glafter test-link-glafter)

add_executable(test-link-glbefore test/test1.cpp)
target_link_libraries(test-link-glbefore GL gtulu-api-3.3)
add_test(test-link-glbefore test-link-glbefore)
