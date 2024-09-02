cmake_minimum_required(VERSION 3.14...3.22)

# ---- Project ----

# Note: update this to your new project's name and version
project(
  constexpr-to-string
  VERSION 1.0
  LANGUAGES CXX)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may
# not detect new files automatically. Keep that in mind when changing files, or
# explicitly mention them here.
file(GLOB_RECURSE headers RELATIVE
  ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS 
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# ---- Create library ----

add_library(${PROJECT_NAME} INTERFACE)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(${PROJECT_NAME}
  INTERFACE "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

target_include_directories(
  ${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(cmake/getCPM.cmake)

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

include(cmake/packages/add_packageproject.cmake)

# the location where the project's version header will be placed should match
# the project's regular header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)
string(TOLOWER ${PROJECT_NAME}/export.h EXPORT_HEADER_LOCATION)

set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION 1)

packageproject(
  NAME
  ${PROJECT_NAME}
  VERSION
  ${PROJECT_VERSION}
  NAMESPACE
  ${PROJECT_NAME}
  BINARY_DIR
  ${PROJECT_BINARY_DIR}
  INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION
  include/${PROJECT_NAME}
  VERSION_HEADER
  "${VERSION_HEADER_LOCATION}"
  EXPORT_HEADER
  "${EXPORT_HEADER_LOCATION}"
  COMPATIBILITY
  "AnyNewerVersion"
  DISABLE_VERSION_SUFFIX
  ON
)
