#include <boost/test/unit_test.hpp>
#include "TypeDefinitions.h"
#include "HeaderGenerator.h"

using namespace SQLParser;

BOOST_AUTO_TEST_CASE(GenerateHeaderFileCheck)
{
    std::vector<TableContent> tables;
    TableContent table;
    table.name = "test";
    TableColumn column;
    column.name = "id";
    column.type = MySQLType::INT;
    column.isPrimaryKey = true;
    table.columns.push_back(column);
    tables.push_back(table);

    std::cout << "Generating header file..." << std::endl;

    GenerateHeaderFile(tables, "test_output.h");
    // Verify the content of "test_output.h" if needed
}
