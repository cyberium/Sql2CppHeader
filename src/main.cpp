#include <iostream>
#include <string>
#include <cstdlib>
#include "SQLParserDefinitions.h"
#include "HeaderGenerator.h"

struct ProgramOptions
{
    std::string inputFile = "mangos.sql";  // Default input file
    std::string outputFile = "ParsedTables.h";  // Default output file
    bool showHelp = false;
    bool verbose = false;
};

void printHelp(const std::string& programName)
{
    std::cout << "Usage: " << programName << " [options]" << std::endl
              << "Options:" << std::endl
              << "  -i, --input <file>    Specify input SQL file" << std::endl
              << "  -o, --output <file>   Specify output header file" << std::endl
              << "  -v, --verbose         Show description of each tables parsed" << std::endl
              << "  -h, --help            Show this help message" << std::endl
              << "example: " << programName << " -i input.sql -o output.h" << std::endl;
}

bool parseArguments(int argc, char* argv[], ProgramOptions& options)
{
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-i" || arg == "--input")
        {
            if (i + 1 < argc)
            {
                options.inputFile = argv[++i];
            }
            else
            {
                std::cerr << "Error: No input file specified" << std::endl;
                return false;
            }
        }
        else if (arg == "-o" || arg == "--output")
        {
            if (i + 1 < argc)
            {
                options.outputFile = argv[++i];
            }
            else
            {
                std::cerr << "Error: No output file specified" << std::endl;
                return false;
            }
        }
        else if (arg == "-v" || arg == "--verbose")
        {
            options.verbose = true;
        }
        else if (arg == "-h" || arg == "--help")
        {
            options.showHelp = true;
            return true;
        }
        else
        {
            std::cerr << "Error: Unknown option " << arg << std::endl;
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    ProgramOptions options;

    if (!parseArguments(argc, argv, options))
    {
        printHelp(argv[0]);
        return 1;
    }

    if (options.showHelp)
    {
        printHelp(argv[0]);
        return 0;
    }

    auto pData = SQLParser::ParsingData(options.inputFile);
    if (!ParseSQLFile(pData))
    {
        std::cerr << "Failed to parse file: " << pData.LastParsingError << std::endl;
        return 1;
    }

    if (!GenerateHeaderFile(pData.tables, options.outputFile))
    {
        std::cerr << "Failed to generate header file" << std::endl;
        return 1;
    }

    if (options.verbose)
    {
        std::cout << "Total parsed tables: " << pData.tables.size() << std::endl;
        std::cout << "Successfully generated header file: " << options.outputFile << std::endl << std::endl;
        std::cout << "Tables description:" << std::endl;
        std::cout << "------------------" << std::endl;
        for (const auto& table : pData.tables)
        {
            std::cout << "Table Name: " << table.name << std::endl << "Columns:" << std::endl;
            for (const auto& col : table.columns)
            {
                std::cout << "  " << col.name << " : " << col.type << " -> " << col.GetCppTypeStr();
                if (col.isPrimaryKey) std::cout << " [Primary Key]";
                std::cout << std::endl;
            }
            std::cout << "Number of columns: " << table.columns.size() << std::endl;
            std::cout << std::endl;
        }
    }

    return 0;
}
