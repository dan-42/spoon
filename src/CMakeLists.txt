cmake_minimum_required(VERSION 3.2)

project(libspoon  VERSION "1.0.0")


# Release mode
if (CMAKE_BUILD_TYPE MATCHES "RELEASE")

  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (CMAKE_LINKER_FLAGS " -s --gc-sections")

    add_compile_options(
      -s
      -fvisibility=hidden 
      -O3
      -ffunction-sections
      -fdata-sections)
  endif()

endif()

list (INSERT LIB_SPOON_COMPILE_FLAGS 0
                --std=c++14
                -Wall
                -Werror
                -Wno-unused-variable
                )

add_compile_options(${LIB_SPOON_COMPILE_FLAGS})


if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
      # debug symbols compatible with C++11 lambdas
      add_definitions(-g
                      -gdwarf-2) 
  endif (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
endif()


# pre
#hunter_add_package(lib-cpp-pre)
#find_package(lib-cpp-pre REQUIRED)
#include_directories(AFTER ${LIB_CPP_PRE_INCLUDE_DIRS})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})



## library  header only 
include_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR})
include_directories(BEFORE ${libspoon_BINARY_DIR}/generated/)

add_library(libspoon INTERFACE)
add_library(libspoon::libspoon ALIAS libspoon) 
target_link_libraries(libspoon INTERFACE)
target_include_directories(libspoon
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
            $<BUILD_INTERFACE:${libspoon_BINARY_DIR}/generated/>
            $<INSTALL_INTERFACE:include/>)

set_target_properties(libspoon
  PROPERTIES
  INTERFACE_COMPILE_OPTIONS "${LIB_SPOON_COMPILE_FLAGS}")

set_target_properties(libspoon
  PROPERTIES
  INTERFACE_COMPILE_FLAGS "${LIB_SPOON_COMPILE_FLAGS}")

## Binaries
install(TARGETS 
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib)



# Installing

# Layout. This works for all platforms:
#   * <prefix>/lib/cmake/<PROJECT-NAME>
#   * <prefix>/lib/
#   * <prefix>/include/
set(config_install_dir lib/cmake/${PROJECT_NAME})
set(include_install_dir "include")

set(generated_dir ${CMAKE_CURRENT_BINARY_DIR}/generated)

# Configuration
set(version_config ${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake)
set(project_config ${generated_dir}/${PROJECT_NAME}Config.cmake)
set(targets_export_name ${PROJECT_NAME}Targets)
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Note: PROJECT_VERSION is used as a VERSION
write_basic_package_version_file(
    ${version_config} COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * targets_export_name
#   * PROJECT_NAME
configure_package_config_file(
    "cmake/Config.cmake.in"
    ${project_config}
    INSTALL_DESTINATION ${config_install_dir}
)


# Targets:
install(
    TARGETS libspoon
    EXPORT ${targets_export_name}
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION ${include_install_dir}
)

# Headers:
install(
    DIRECTORY "src/"
    DESTINATION ${include_install_dir}
    FILES_MATCHING PATTERN "*.[ih]pp"
)
install(
    FILES "src/.hpp" 
    DESTINATION ${include_install_dir}   
)

install(
    FILES ${libspoon_BINARY_DIR}/generated//detail/_configuration.hpp
    DESTINATION ${include_install_dir}//detail/
)

# Config
#   * <prefix>/lib/cmake/Foo/FooConfig.cmake
#   * <prefix>/lib/cmake/Foo/FooConfigVersion.cmake
#   * <prefix>/lib/cmake/Foo/FooTargets.cmake
install(
    FILES ${project_config} ${version_config}
    DESTINATION ${config_install_dir}
)
install(
    EXPORT ${targets_export_name}
    NAMESPACE ${namespace}
    DESTINATION ${config_install_dir}
)


