include(${CMAKE_CURRENT_LIST_DIR}/../getCPM.cmake)

set(packageProject_VERSION 1.11.3)
CPMAddPackage(
  NAME PackageProject.cmake
  VERSION ${packageProject_VERSION}
  # GIT_REPOSITORY "https://github.com/TheLartians/PackageProject.cmake.git" GIT_TAG
  # "v${packageProject_VERSION}"
  GIT_TAG master
  GIT_REPOSITORY https://github.com/Arniiiii/PackageProject.cmake
  # URL "https://github.com/TheLartians/PackageProject.cmake/archive/refs/tags/v${packageProject_VERSION}.zip"
)
