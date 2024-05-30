#include <boost/test/unit_test.hpp>
#include "TypeDefinitions.h"

using namespace SQLParser;

BOOST_AUTO_TEST_CASE(MySQLTypeConversions)
{
    BOOST_CHECK_EQUAL(GetMySQLType("INT"), MySQLType::INT);
    BOOST_CHECK_EQUAL(GetCppType(MySQLType::INT, false), CppEquivalentType::INT);
    BOOST_CHECK_EQUAL(GetMySQLTypeStr(MySQLType::INT), "INT");
    BOOST_CHECK_EQUAL(GetCppTypeStr(CppEquivalentType::INT), "int");
}