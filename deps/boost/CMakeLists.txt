message(STATUS "Setting boost ...")

# Add alternate lib dir for boost on windows
if(WIN32 AND NOT "$ENV{BOOST_ROOT}" STREQUAL "")
  set(BOOST_LIBRARYDIR "$ENV{BOOST_LIBRARYDIR}")
  set(BOOST_LIBRARYDIR "${BOOST_LIBRARYDIR};$ENV{BOOST_ROOT}\\lib\\${DEP_ARCH}")
  set(BOOST_LIBRARYDIR "${BOOST_LIBRARYDIR};$ENV{BOOST_ROOT}\\stage\\${DEP_ARCH}")
  set(BOOST_LIBRARYDIR "${BOOST_LIBRARYDIR};$ENV{BOOST_ROOT}\\stage")
endif()

# Be sure to search only static lib
add_definitions(-DBOOST_ALL_NO_LIB)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_BOOST_CMAKE ON)   # should fix build for boost 1.71
#set(Boost_DEBUG ON)

# In our case use new way to treat BOOST_ROOT environement variable
if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

# Define boost required components and make this variable available to other CMakeLists
set(Boost_REQUIRED_COMPONENTS system program_options thread regex serialization filesystem unit_test_framework CACHE INTERNAL "")

# Find needed packages and if necessery abort if something important is missing
find_package(Boost REQUIRED COMPONENTS ${Boost_REQUIRED_COMPONENTS})

if(NOT Boost_FOUND AND (BUILD_GAME_SERVER OR BUILD_LOGIN_SERVER))
  message(STATUS "BOOST_ROOT = $ENV{BOOST_ROOT}")
  message(STATUS "BOOST_LIBRARYDIR = $ENV{BOOST_LIBRARYDIR}")
  if ("$ENV{BOOST_ROOT}" STREQUAL "")
    message(WARNING "BOOST_ROOT is not set, please set it correctly to your boost folder.")
  else()
    if ("$ENV{BOOST_LIBRARYDIR}" STREQUAL "")
        message(STATUS "Please put all ${DEP_ARCH} boost lib files in $ENV{BOOST_ROOT}\\lib\\${DEP_ARCH} folder")
        message(STATUS "Or set BOOST_LIBRARYDIR to your boost lib folder.")
    else()
        message(STATUS "BOOST_LIBRARYDIR is set, please verify if correct libs files are in that folder.")
    endif()
  endif()
  message(FATAL_ERROR "This project requires boost.  Please install from http://www.boost.org")
endif()

# Display Boost version
message(STATUS "Found Boost version: ${Boost_VERSION}")