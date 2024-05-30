#include "HeaderGenerator.h"
#include <fstream>

bool GenerateHeaderFile(const std::vector<SQLParser::TableContent>& tables, const std::string& filename)
{
    std::ofstream headerFile(filename);
    if (!headerFile.is_open())
        return false;

    // Write the header guard and includes
    headerFile << "#ifndef PARSED_TABLES_H" << std::endl;
    headerFile << "#define PARSED_TABLES_H" << std::endl << std::endl;
    headerFile << "#include <string>" << std::endl;
    headerFile << "#include <cstdint>" << std::endl;
    headerFile << "#include <vector>" << std::endl << std::endl;
    headerFile << "namespace PDb {" << std::endl << std::endl;

    // Write the structures
    for (const auto& table : tables)
    {
        headerFile << "struct " << table.name << std::endl;
        headerFile << "{" << std::endl;
        for (const auto& col : table.columns)
        {
            headerFile << "    " << col.GetCppTypeStr() << " " << col.name << ";";
            if (col.isPrimaryKey)
            {
                headerFile << " // Primary Key";
            }
            headerFile << std::endl;
        }
        headerFile << "};" << std::endl << std::endl;
    }

    // Close the namespace and header guard
    headerFile << "} // namespace PDb" << std::endl << std::endl;
    headerFile << "#endif // PARSED_TABLES_H" << std::endl;

    headerFile.close();

    return true;
}
