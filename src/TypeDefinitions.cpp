#include "TypeDefinitions.h"

namespace SQLParser {

std::unordered_map<std::string, TypeInfo> typeMap =
{
    {"INT", {MySQLType::INT, CppEquivalentType::INT, CppEquivalentType::UNSIGNED_INT}},
    {"TINYINT", {MySQLType::TINYINT, CppEquivalentType::CHAR, CppEquivalentType::UNSIGNED_CHAR}},
    {"SMALLINT", {MySQLType::SMALLINT, CppEquivalentType::SHORT, CppEquivalentType::UNSIGNED_SHORT}},
    {"MEDIUMINT", {MySQLType::MEDIUMINT, CppEquivalentType::INT, CppEquivalentType::UNSIGNED_INT}},
    {"BIGINT", {MySQLType::BIGINT, CppEquivalentType::LONG, CppEquivalentType::UNSIGNED_LONG}},
    {"FLOAT", {MySQLType::FLOAT, CppEquivalentType::FLOAT, CppEquivalentType::FLOAT}},
    {"DOUBLE", {MySQLType::DOUBLE, CppEquivalentType::DOUBLE, CppEquivalentType::DOUBLE}},
    {"DECIMAL", {MySQLType::DECIMAL, CppEquivalentType::DOUBLE, CppEquivalentType::DOUBLE}},
    {"DATE", {MySQLType::DATE, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"DATETIME", {MySQLType::DATETIME, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"TIMESTAMP", {MySQLType::TIMESTAMP, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"TIME", {MySQLType::TIME, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"YEAR", {MySQLType::YEAR, CppEquivalentType::INT, CppEquivalentType::UNSIGNED_INT}},
    {"CHAR", {MySQLType::CHAR, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"VARCHAR", {MySQLType::VARCHAR, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"BLOB", {MySQLType::BLOB, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"TINYTEXT", {MySQLType::TINYTEXT, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"TEXT", {MySQLType::TEXT, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"MEDIUMTEXT", {MySQLType::MEDIUMTEXT, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"LONGTEXT", {MySQLType::LONGTEXT, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"ENUM", {MySQLType::ENUM, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"SET", {MySQLType::SET, CppEquivalentType::STRING, CppEquivalentType::STRING}},
    {"BIT", {MySQLType::BIT, CppEquivalentType::BOOL, CppEquivalentType::BOOL}},
    {"UNKNOWN", {MySQLType::UNKNOWN, CppEquivalentType::UNKNOWN, CppEquivalentType::UNKNOWN}}
};

MySQLType GetMySQLType(const std::string& typeStr)
{
    auto typeStrUpper(typeStr);
    std::transform(typeStrUpper.begin(), typeStrUpper.end(), typeStrUpper.begin(), ::toupper);
    auto it = typeMap.find(typeStrUpper);
    if (it != typeMap.end())
    {
        return it->second.mySQLType;
    }
    else
    {
        return MySQLType::UNKNOWN;
    }
}

CppEquivalentType GetCppType(MySQLType type, bool isUnsigned)
{
    for (const auto& pair : typeMap)
    {
        if (pair.second.mySQLType == type)
        {
            return isUnsigned ? pair.second.unsignedCppType : pair.second.cppType;
        }
    }
    return CppEquivalentType::UNKNOWN;
}

std::string GetMySQLTypeStr(MySQLType type)
{
    for (const auto& pair : typeMap)
    {
        if (pair.second.mySQLType == type)
        {
            return pair.first;
        }
    }
    return "UNKNOWN";
}

std::string GetCppTypeStr(CppEquivalentType type)
{
    switch (type)
    {
        case CppEquivalentType::INT: return "int";
        case CppEquivalentType::UNSIGNED_INT: return "unsigned int";
        case CppEquivalentType::SHORT: return "short";
        case CppEquivalentType::UNSIGNED_SHORT: return "unsigned short";
        case CppEquivalentType::CHAR: return "char";
        case CppEquivalentType::UNSIGNED_CHAR: return "unsigned char";
        case CppEquivalentType::LONG: return "long";
        case CppEquivalentType::UNSIGNED_LONG: return "unsigned long";
        case CppEquivalentType::FLOAT: return "float";
        case CppEquivalentType::DOUBLE: return "double";
        case CppEquivalentType::STRING: return "std::string";
        case CppEquivalentType::BOOL: return "bool";
        default: return "unknown";
    }
}

CppEquivalentType TableColumn::GetCppType() const
{
    return SQLParser::GetCppType(type, isUnsigned);
}

std::string TableColumn::GetCppTypeStr() const
{
    return SQLParser::GetCppTypeStr(GetCppType());
}

TableColumn& TableContent::GetColumn(const std::string& name)
{
    std::string nameUpper;
    std::transform(name.begin(), name.end(), std::back_inserter(nameUpper), ::toupper);

    auto it = std::find_if(columns.begin(), columns.end(), [&](const TableColumn& col)
    {
        std::string colNameUpper;
        std::transform(col.name.begin(), col.name.end(), std::back_inserter(colNameUpper), ::toupper);
        return colNameUpper == nameUpper;
    });
    if (it != columns.end())
    {
        return *it;
    }
    else
    {
        throw std::runtime_error("Column not found: " + name);
    }
}

void TableContent::SetPrimaryKeys(const std::vector<std::string>& keys)
{
    for (const auto& key : keys)
        GetColumn(key).isPrimaryKey = true;
}

// Function to copy a table with a new name
TableContent CopyTableWithNewName(const std::vector<TableContent>& tables, const std::string& originalName, const std::string& newName)
{
    auto it = std::find_if(tables.begin(), tables.end(), [&](const TableContent& table)
    {
        return table.name == originalName;
    });
    if (it != tables.end())
    {
        TableContent newTable = *it;
        newTable.name = newName;
        return newTable;
    }
    else
    {
        throw std::runtime_error("Original table not found: " + originalName);
    }
}

std::ostream& operator<<(std::ostream& os, MySQLType type)
{
    os << GetMySQLTypeStr(type);
    return os;
}

std::ostream& operator<<(std::ostream& os, CppEquivalentType type)
{
    os << GetCppTypeStr(type);
    return os;
}

} // namespace SQLParser