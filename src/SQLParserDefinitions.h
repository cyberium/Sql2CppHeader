#pragma once

#include "TypeDefinitions.h"
#include <string>
#include <vector>

namespace SQLParser{

struct ParsingData
{
    ParsingData(const std::string& file) : fileName(file) {}

    ParsingData(const ParsingData&) = delete;
    ParsingData& operator=(const ParsingData&) = delete;
    ParsingData(ParsingData&&) = delete;
    ParsingData& operator=(ParsingData&&) = delete;
    ParsingData() = delete;

    std::vector<TableContent> tables;

    std::string LastParsingError;

    std::string const& GetFileName() const { return fileName; }

private:
    std::string fileName;
};


} // namespace SQLParser

bool ParseSQLFile(SQLParser::ParsingData& data);