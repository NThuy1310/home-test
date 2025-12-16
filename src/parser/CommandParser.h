#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "../commands/ICommand.h"
#include <memory>
#include <string>
#include <vector>
#include <fstream>

/**
 * @brief Parse result structure
 */
struct ParseResult {
    std::unique_ptr<ICommand> command;
    bool success;
    std::string error_message;
    size_t line_number;

    ParseResult() : success(false), line_number(0) {}
    
    static ParseResult Success(std::unique_ptr<ICommand> cmd, size_t line_num) {
        ParseResult result;
        result.command = std::move(cmd);
        result.success = true;
        result.line_number = line_num;
        return result;
    }

    static ParseResult Error(const std::string& error, size_t line_num) {
        ParseResult result;
        result.success = false;
        result.error_message = error;
        result.line_number = line_num;
        return result;
    }
};

/**
 * @brief Parser for robot commands
 * 
 * Supports streaming parsing for large files.
 * Parses commands line-by-line without loading entire file into memory.
 */
class CommandParser {
public:
    /**
     * @brief Parse a single command line
     * @param line Command line to parse
     * @param line_number Line number (for error reporting)
     * @return ParseResult containing command or error
     */
    static ParseResult parseLine(const std::string& line, size_t line_number);

    /**
     * @brief Parse all commands from a file (streaming)
     * @param filename Path to command file
     * @return Vector of parse results
     */
    static std::vector<ParseResult> parseFile(const std::string& filename);

private:
    /**
     * @brief Trim whitespace from string
     */
    static std::string trim(const std::string& str);

    /**
     * @brief Split string by delimiter
     */
    static std::vector<std::string> split(const std::string& str, char delimiter);

    /**
     * @brief Parse DIMENSION command
     */
    static ParseResult parseDimension(const std::vector<std::string>& tokens, size_t line_number);

    /**
     * @brief Parse MOVE_TO command
     */
    static ParseResult parseMoveTo(const std::vector<std::string>& tokens, size_t line_number);

    /**
     * @brief Parse LINE_TO command
     */
    static ParseResult parseLineTo(const std::vector<std::string>& tokens, size_t line_number);

    /**
     * @brief Parse coordinate pair (x,y)
     */
    static bool parseCoordinates(const std::string& coord_str, int& x, int& y);
};

#endif // COMMAND_PARSER_H
