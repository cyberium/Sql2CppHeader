#include <boost/test/unit_test.hpp>
#include "SQLParserDefinitions.h"

BOOST_AUTO_TEST_CASE(BasicParsing)
{
    std::string input = "CREATE TABLE test (id INT PRIMARY KEY);";

    SQLParser::ParsingData pData("");

    bool result = ParseSQLString(input, pData);

    BOOST_CHECK(result);
    BOOST_CHECK_EQUAL(pData.tables.size(), 1);
    BOOST_CHECK_EQUAL(pData.tables[0].name, "test");
    BOOST_CHECK_EQUAL(pData.tables[0].columns.size(), 1);
    BOOST_CHECK_EQUAL(pData.tables[0].columns[0].name, "id");
}
