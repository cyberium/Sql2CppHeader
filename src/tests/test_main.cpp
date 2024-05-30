#define BOOST_TEST_MODULE MainTest
#include <boost/test/included/unit_test.hpp>
#include "SQLParserDefinitions.h"
#include "HeaderGenerator.h"
#include <fstream>

using namespace SQLParser;

BOOST_AUTO_TEST_CASE(BasicAssertions)
{
    // Write the SQL statement to test.sql
    std::ofstream sqlFile("test.sql");
    sqlFile << "CREATE TABLE test (id INT PRIMARY KEY, name VARCHAR(50));";
    sqlFile.close();

    // Parse the SQL file
    SQLParser::ParsingData pData("test.sql");
    BOOST_CHECK(ParseSQLFile(pData));
    BOOST_CHECK(!pData.tables.empty());

    // Additional assertions for pData.tables content
    BOOST_CHECK_EQUAL(pData.tables.size(), 1);
    BOOST_CHECK_EQUAL(pData.tables[0].name, "test");
    BOOST_CHECK_EQUAL(pData.tables[0].columns.size(), 2);
    BOOST_CHECK_EQUAL(pData.tables[0].columns[0].name, "id");
    BOOST_CHECK_EQUAL(pData.tables[0].columns[0].type, MySQLType::INT);
    BOOST_CHECK(pData.tables[0].columns[0].isPrimaryKey);
    BOOST_CHECK_EQUAL(pData.tables[0].columns[1].name, "name");
    BOOST_CHECK_EQUAL(pData.tables[0].columns[1].type, MySQLType::VARCHAR);
}

BOOST_AUTO_TEST_CASE(MultipleTables)
{
    // Write another SQL statement to test.sql
    std::ofstream sqlFile("test.sql");
    sqlFile << "CREATE TABLE table1 (id INT PRIMARY KEY, name VARCHAR(50));";
    sqlFile << "CREATE TABLE IF NOT EXISTS table2 (id INT UNSIGNED, value FLOAT, PRIMARY KEY (`id`)) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='table2 test';";
    sqlFile << "CREATE TABLE `table3` (`id` DECIMAL(20,40), `description` LONGTEXT, PRIMARY KEY (`id`));";
    sqlFile.close();

    // Parse the SQL file
    SQLParser::ParsingData pData("test.sql");
    BOOST_CHECK(ParseSQLFile(pData));
    BOOST_CHECK(!pData.tables.empty());
    BOOST_CHECK_EQUAL(pData.tables.size(), 3);

    // Assertions for multiple tables
    BOOST_CHECK_EQUAL(pData.tables[0].name, "table1");
    BOOST_CHECK_EQUAL(pData.tables[0].columns.size(), 2);
    BOOST_CHECK_EQUAL(pData.tables[0].columns[0].name, "id");
    BOOST_CHECK_EQUAL(pData.tables[0].columns[0].type, MySQLType::INT);
    BOOST_CHECK(pData.tables[0].columns[0].isPrimaryKey);
    BOOST_CHECK_EQUAL(pData.tables[0].columns[1].name, "name");
    BOOST_CHECK_EQUAL(pData.tables[0].columns[1].type, MySQLType::VARCHAR);

    BOOST_CHECK_EQUAL(pData.tables[1].name, "table2");
    BOOST_CHECK_EQUAL(pData.tables[1].columns.size(), 2);
    BOOST_CHECK_EQUAL(pData.tables[1].columns[0].name, "id");
    BOOST_CHECK_EQUAL(pData.tables[1].columns[0].type, MySQLType::INT);
    BOOST_CHECK(pData.tables[1].columns[0].isUnsigned);
    BOOST_CHECK(pData.tables[1].columns[0].isPrimaryKey);
    BOOST_CHECK_EQUAL(pData.tables[1].columns[1].name, "value");
    BOOST_CHECK_EQUAL(pData.tables[1].columns[1].type, MySQLType::FLOAT);

    BOOST_CHECK_EQUAL(pData.tables[2].name, "table3");
    BOOST_CHECK_EQUAL(pData.tables[2].columns.size(), 2);
    BOOST_CHECK_EQUAL(pData.tables[2].columns[0].name, "id");
    BOOST_CHECK_EQUAL(pData.tables[2].columns[0].type, MySQLType::DECIMAL);
    BOOST_CHECK(!pData.tables[2].columns[0].isUnsigned);
    BOOST_CHECK(pData.tables[2].columns[0].isPrimaryKey);
    BOOST_CHECK_EQUAL(pData.tables[2].columns[1].name, "description");
    BOOST_CHECK_EQUAL(pData.tables[2].columns[1].type, MySQLType::LONGTEXT);
}