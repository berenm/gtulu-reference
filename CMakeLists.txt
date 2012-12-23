cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CMakeExtraBootstrap)
include(GNUInstallDirs)
include(FindOpenGL)
include(AddSubmodule)
include(FindCXX11)

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
file(GLOB_RECURSE API_MAN_SOURCES man/*.xml)
add_custom_command(
  OUTPUT  list/gl2/constants
          list/gl3/constants
          list/gl4/constants 
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/bin/extract-constants ${GTULU_API_VERSION}
  COMMENT "Extracting constant lists from the man pages"
  DEPENDS bin/extract-constants ${API_MAN_SOURCES}
)

add_custom_command(
  OUTPUT  list/gl2/functions
          list/gl2/functions.api
          list/gl2/functions.call
          list/gl3/functions
          list/gl3/functions.api
          list/gl3/functions.call
          list/gl4/functions
          list/gl4/functions.api
          list/gl4/functions.call 
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/bin/extract-functions ${GTULU_API_VERSION}
  COMMENT "Extracting function definitions from the man pages"
  DEPENDS bin/extract-functions ${API_MAN_SOURCES}
)

include_directories(src)

function(add_api_library API_SOURCE_DIR)
  string(REGEX REPLACE ".*(core|comp)$"           "\\1" API_TYPE      "${API_SOURCE_DIR}")
  string(REGEX REPLACE "^.*/([^/]+)/${API_TYPE}$" "\\1" API_VERSION   "${API_SOURCE_DIR}")
  string(REGEX REPLACE "\\..*$"                   ""    API_SOVERSION "${API_VERSION}")

  if(NOT API_TYPE MATCHES "core")
    return()
  endif()

  set(API_TARGET gtulu-api-${API_VERSION})
  add_library(${API_TARGET} SHARED ${API_SOURCE_DIR}/gtulu/api.cpp src/gtulu/logging.cpp src/gtulu/error.cpp)
  add_dependencies(${API_TARGET} generate-api)
  set_target_properties(${API_TARGET} PROPERTIES
    VERSION       ${API_VERSION}
    SOVERSION     ${API_SOVERSION}
    SONAME        libgtulu-api${API_SOVERSION}
    OUTPUT_NAME   gtulu-api
    PUBLIC_HEADER ${API_SOURCE_DIR}/gtulu/api.hpp
    PUBLIC_HEADER ${API_SOURCE_DIR}/gtulu/types.h
    COMPILE_FLAGS "-fvisibility=hidden -I${API_SOURCE_DIR}"
    LINK_FLAGS    "-Wl,--as-needed -Wl,--no-undefined"
  )
  add_library(${API_TARGET}_ni SHARED ${API_SOURCE_DIR}/gtulu/api_ni.cpp src/gtulu/logging.cpp src/gtulu/notimp.cpp)
  add_dependencies(${API_TARGET}_ni generate-api)
  set_target_properties(${API_TARGET}_ni PROPERTIES
    VERSION       ${API_VERSION}
    SOVERSION     ${API_SOVERSION}
    SONAME        libgtulu-api-ni${API_SOVERSION}
    OUTPUT_NAME   gtulu-api-ni
    COMPILE_FLAGS "-fvisibility=hidden -I${API_SOURCE_DIR}"
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


add_custom_command(OUTPUT ${API_SOURCES}
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/bin/generate-header ${GTULU_API_VERSION}
  COMMENT "Generating API sources and headers"
  DEPENDS list/gl2/constants
          list/gl3/constants
          list/gl4/constants
          list/gl2/functions
          list/gl2/functions.api
          list/gl2/functions.call
          list/gl3/functions
          list/gl3/functions.api
          list/gl3/functions.call
          list/gl4/functions
          list/gl4/functions.api
          list/gl4/functions.call
          bin/generate-header
)

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
