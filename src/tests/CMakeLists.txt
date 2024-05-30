# Define the test sources
set(TEST_SOURCES
  test_main.cpp
  test_types.cpp
  test_sql_parser.cpp
  test_utils.cpp
  ${CMAKE_SOURCE_DIR}/src/parser/HeaderGenerator.cpp
  ${CMAKE_SOURCE_DIR}/src/parser/SQLParserDefinitions.cpp
  ${CMAKE_SOURCE_DIR}/src/parser/TypeDefinitions.cpp
)

# Define the test executable
add_executable(runTests ${TEST_SOURCES})

# Link Boost.Test to the test executable
target_link_libraries(runTests ${Boost_LIBRARIES} Boost::unit_test_framework)

# set the include directories
target_include_directories(runTests PRIVATE "${CMAKE_SOURCE_DIR}/src/parser" ${Boost_INCLUDE_DIRS})

# Add a test to be run by CTest
add_test(NAME runTests COMMAND runTests)