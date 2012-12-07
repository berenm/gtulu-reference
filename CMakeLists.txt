cmake_minimum_required(VERSION 2.8)

# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(GNUInstallDirs)
# include(ParseVersion)
# include(AddSubmodule)
# include(FindCXX11)

project(libgtulu_api CXX)
# parse_version(libgtulu_api DEBIAN)

# add_submodule(logging lib/logging INCLUDE_DIRS include lib/boost-log)
# add_submodule(registry lib/registry INCLUDE_DIRS include)

find_package(PkgConfig REQUIRED)

# On debian, pkg-config finds the MESA libGL.so, lets try using NVidia's if available.
find_library(OPENGL_LIBRARIES libnvidia-cfg.so.1)
if(OPENGL_LIBRARIES)
  string(REPLACE "libnvidia-cfg.so.1" "libGL.so.1" OPENGL_LIBRARIES ":${OPENGL_LIBRARIES}")
  list(APPEND OPENGL_LIBRARIES X11 EGL)
else()
  pkg_check_modules(OPENGL REQUIRED gl egl)
endif()
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})

include_directories(~/src/github/libgtulu/lib/registry/include)

# -------------------------------------------------------------------------
# libgtulu_api
add_custom_target(extract-constants
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/bin/extract-constants
  COMMENT "Extracting constant lists from the man pages"
)
add_custom_target(extract-functions
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/bin/extract-functions
  COMMENT "Extracting function definitions from the man pages"
)
add_custom_target(generate-lists
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/bin/generate-lists
  COMMENT "Generating constant and function lists"
  DEPENDS extract-functions extract-constants
)

function(add_api_library API_SOURCE_DIR)
  string(REGEX REPLACE ".*(core|comp)$"           "\\1" API_TYPE      "${API_SOURCE_DIR}")
  string(REGEX REPLACE "^.*/([^/]+)/${API_TYPE}$" "\\1" API_VERSION   "${API_SOURCE_DIR}")
  string(REGEX REPLACE "\\..*$"                   ""    API_SOVERSION "${API_VERSION}")

  if(NOT API_TYPE MATCHES "core")
    return()
  endif()

  set(API_TARGET gtulu-api-${API_VERSION})
  add_library(${API_TARGET} SHARED ${API_SOURCE_DIR}/gtulu_api.cpp)
  add_dependencies(${API_TARGET} generate-api)
  set_target_properties(${API_TARGET} PROPERTIES
    VERSION       ${API_VERSION}
    SOVERSION     ${API_SOVERSION}
    SONAME        libgtulu-api${API_SOVERSION}
    OUTPUT_NAME   gtulu-api
    COMPILE_FLAGS "-fvisibility=hidden -I${API_SOURCE_DIR}"
  )
  target_link_libraries(${API_TARGET} ${OPENGL_LIBRARIES})
  install(TARGETS ${API_TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
endfunction()

file(GLOB_RECURSE API_SOURCE_DIRS gl/*/.git)
list(SORT API_SOURCE_DIRS)

set(API_SOURCES "")
foreach(API_SOURCE_DIR ${API_SOURCE_DIRS})
  get_filename_component(API_SOURCE_DIR "${API_SOURCE_DIR}" PATH)
  string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" API_SOURCE_DIR "${API_SOURCE_DIR}")

  list(APPEND API_SOURCES ${API_SOURCE_DIR}/gtulu_api.cpp)
endforeach()

add_custom_command(OUTPUT ${API_SOURCES}
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/bin/generate-api
  COMMENT "Generating API sources and headers"
  DEPENDS extract-functions extract-constants
)

foreach(API_SOURCE_DIR ${API_SOURCE_DIRS})
  get_filename_component(API_SOURCE_DIR "${API_SOURCE_DIR}" PATH)
  string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" API_SOURCE_DIR "${API_SOURCE_DIR}")

  add_api_library(${API_SOURCE_DIR})
endforeach()
