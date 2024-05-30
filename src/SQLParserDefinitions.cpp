#include "SQLParserDefinitions.h"

#include "TypeDefinitions.h"
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <fstream>
#include <sstream>

namespace SQLParserRule
{
    using namespace boost::spirit::x3;
    using namespace SQLParser;

    rule<class any_word, std::string> const any_word = "any_word";
    auto const any_word_def = lexeme[(alpha | char_('_')) >> *(alnum | char_('_'))];

    rule<class identifier, std::string> const identifier = "identifier";
    auto const identifier_def = -lit('`') >> any_word >> -lit('`');

    rule<class quoted_text, std::string> const quoted_text = "quoted_text";
    auto const quoted_text_def = lexeme['\'' >> *(char_ - '\'') >> '\'' | '\"' >> *(char_ - '\"') >> '\"'];

    rule<class sql_type, std::string> const sql_type = "sql_type";
    auto const sql_type_def = lexeme[+(char_ - space - '(' - ',')] >> -omit['(' >> *digit >> ')'];

    rule<class type_unsigned, bool> const type_unsigned = "type_unsigned";
    auto const type_unsigned_def = no_case["UNSIGNED"] [([](auto& ctx){ _val(ctx) = true; })];

    rule<class type, MySQLType> const type = "type";
    auto const type_def = sql_type [([](auto& ctx){ _val(ctx) = GetMySQLType(_attr(ctx)); })];

    rule<class column, TableColumn> const column = "column";
    auto const set_primary = [](auto& ctx) { _val(ctx).isPrimaryKey = true; };
    auto const column_def = identifier [([](auto& ctx){ _val(ctx).name = _attr(ctx); })]
                    >> type [([](auto& ctx){ _val(ctx).type = _attr(ctx); })]
                    >> -(type_unsigned [([](auto& ctx){ _val(ctx).isUnsigned = true; })])
                    >> -(
                        (lit("PRIMARY KEY") [set_primary]) |
                        lit("UNIQUE KEY") |
                        lit("KEY")
                        )
                    >> -omit[*(any_word | quoted_text | int_ | lit('(') >> *(char_ - ')') >> lit(')'))]
                    >> -char_(',');

    rule<class pk_constraint, std::vector<std::string>> const pk_constraint = "pk_constraint";
    auto const pk_constraint_def = no_case[lit("PRIMARY KEY")] >> '(' >> (identifier % ',') >> ')' >> -lit(',');

    rule<class ignored_constraint, std::string> const ignored_constraint = "unique_constraint";
    auto const ignored_constraint_def = (no_case[lit("UNIQUE KEY")] | no_case[lit("KEY")]) >> identifier >> lit('(') >> *(char_ - ')') >> lit(')') >> -lit(',');

    rule<class create_table, std::string> const create_table = "create_table";
    auto const create_table_def = no_case[lit("CREATE TABLE")]
                    >> -(no_case[lit("IF NOT EXISTS")])
                    >> identifier;

    rule<class table, TableContent> const table = "table";
    auto const table_def = create_table [([](auto& ctx){ _val(ctx).name = _attr(ctx); })]
                    >> '('
                    >> *(
                        pk_constraint [([](auto& ctx) { _val(ctx).SetPrimaryKeys(_attr(ctx)); })]
                        | ignored_constraint
                        | column [([](auto& ctx) { _val(ctx).columns.push_back(_attr(ctx)); })])
                    >> ')'
                    >> *(char_ - ';') // Skip the rest of the line until ';'
                    >> lit(';');

    rule<class table_like, TableLikeContent> const table_like = "table_like";
    auto const table_like_def = create_table [([](auto& ctx){ _val(ctx).name = _attr(ctx); })]
                    >> no_case[lit("LIKE")] >> identifier [([](auto& ctx){ _val(ctx).likeName = _attr(ctx); })]
                    >> lit(';');

    rule<class skip_content, std::string> const skip_content = "skip_content";
    auto const skip_content_def = +(char_ - no_case[lit("CREATE TABLE")]);

    rule<class file_content, std::vector<TableContent>> const file_content = "file_content";
    auto const file_content_def = *(
        (table [([](auto& ctx) { _val(ctx).push_back(_attr(ctx)); })]
        | table_like [([](auto& ctx) {
            auto& tables = _val(ctx);
            const auto& likeContent = _attr(ctx);
            tables.push_back(CopyTableWithNewName(tables, likeContent.likeName, likeContent.name)); })]
        | skip_content)
    );


    BOOST_SPIRIT_DEFINE(
        any_word,
        identifier,
        quoted_text,
        sql_type,
        type_unsigned,
        type,
        column,
        pk_constraint,
        ignored_constraint,
        create_table,
        table,
        table_like,
        skip_content,
        file_content
    )
}

bool ParseSQLFile(SQLParser::ParsingData &data)
{
    std::ifstream file(data.GetFileName());
    if (!file.is_open())
    {
        data.LastParsingError = "Failed to open file.";
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string input = buffer.str();


    auto iter = input.begin();
    auto end = input.end();

    bool r = phrase_parse(iter, end, SQLParserRule::file_content, boost::spirit::x3::space, data.tables);

    if (!r || iter != end)
    {
        // If the parsing failed, assign the first 100 characters of the error message
        data.LastParsingError.assign(iter, std::distance(iter, end) > 100 ? iter + 100 : end);
        //std::cout << "Parse failed\n";
        return false;
    }

    return true;
}
