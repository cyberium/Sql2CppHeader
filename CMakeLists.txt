cmake_minimum_required(VERSION 3.28)
project(Sql2CppHeader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Disable tests if need
option(DISABLE_TESTS "Disable generation of tests" OFF)

# parralel build settings
if(MSVC)
  # Enable parrallel build for msvc
  add_compile_options("/MP")
endif()

include(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT)
if(PROCESSOR_COUNT EQUAL 0)
   message(FATAL_ERROR "Unable to determine the number of CPU cores.")
else()
    message(STATUS "Detected ${PROCESSOR_COUNT} CPU cores.")
    set(CMAKE_BUILD_PARALLEL_LEVEL ${PROCESSOR_COUNT})
endif()

# add dependencies
add_subdirectory(deps)

# Enable testing if DISABLE_TESTS option is OFF
if(NOT DISABLE_TESTS)
    enable_testing()
endif()

# add source
add_subdirectory(src)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})