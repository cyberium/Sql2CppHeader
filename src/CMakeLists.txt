# Find Boost again to ensure all the variables are available (because previous time it was found in a subdirectory)
find_package(Boost REQUIRED COMPONENTS ${Boost_REQUIRED_COMPONENTS} QUIET)

add_subdirectory(parser)
add_subdirectory(tests)