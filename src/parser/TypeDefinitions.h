#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>

namespace SQLParser {

enum class MySQLType
{
    INT,
    TINYINT,
    SMALLINT,
    MEDIUMINT,
    BIGINT,
    FLOAT,
    DOUBLE,
    DECIMAL,
    DATE,
    DATETIME,
    TIMESTAMP,
    TIME,
    YEAR,
    CHAR,
    VARCHAR,
    BLOB,
    TINYTEXT,
    TEXT,
    MEDIUMTEXT,
    LONGTEXT,
    ENUM,
    SET,
    BIT,
    UNKNOWN
};

enum class CppEquivalentType
{
    INT,
    UNSIGNED_INT,
    SHORT,
    UNSIGNED_SHORT,
    CHAR,
    UNSIGNED_CHAR,
    LONG,
    UNSIGNED_LONG,
    FLOAT,
    DOUBLE,
    STRING,
    BOOL,
    UNKNOWN
};

struct TypeInfo
{
    MySQLType mySQLType;
    CppEquivalentType cppType;
    CppEquivalentType unsignedCppType;
};

struct TableColumn
{
    std::string name;
    MySQLType type;
    bool isPrimaryKey = false;
    bool isUnsigned = false;

    CppEquivalentType GetCppType() const;
    std::string GetCppTypeStr() const;
};

struct TableContent
{
    std::string name;
    std::vector<TableColumn> columns;

    TableColumn& GetColumn(const std::string& name);
    void SetPrimaryKeys(const std::vector<std::string>& keys);
};

struct TableLikeContent
{
    std::string name;
    std::string likeName;
};

// Function declarations
MySQLType GetMySQLType(const std::string& typeStr);
CppEquivalentType GetCppType(MySQLType type, bool isUnsigned);
std::string GetMySQLTypeStr(MySQLType type);
std::string GetCppTypeStr(CppEquivalentType type);
TableContent CopyTableWithNewName(const std::vector<TableContent> &tables, const std::string &originalName, const std::string &newName);
std::ostream &operator<<(std::ostream &os, MySQLType type);
std::ostream& operator<<(std::ostream& os, CppEquivalentType type);

} // namespace SQLParser
