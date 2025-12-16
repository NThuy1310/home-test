#include "CommandParser.h"
#include "../commands/DimensionCommand.h"
#include "../commands/MoveToCommand.h"
#include "../commands/LineToCommand.h"
#include <sstream>
#include <algorithm>
#include <cctype>

ParseResult CommandParser::parseLine(const std::string& line, size_t line_number) {
    // Trim and skip empty lines or comments
    std::string trimmed = trim(line);
    
    if (trimmed.empty() || trimmed[0] == '#') {
        // Empty line or comment - not an error, just skip
        return ParseResult();
    }

    // Split by space
    std::vector<std::string> tokens = split(trimmed, ' ');
    
    if (tokens.empty()) {
        return ParseResult();
    }

    // Convert command to uppercase for case-insensitive matching
    std::string command = tokens[0];
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    if (command == "DIMENSION") {
        return parseDimension(tokens, line_number);
    } else if (command == "MOVE_TO") {
        return parseMoveTo(tokens, line_number);
    } else if (command == "LINE_TO") {
        return parseLineTo(tokens, line_number);
    } else {
        return ParseResult::Error("Unknown command: " + tokens[0], line_number);
    }
}

std::vector<ParseResult> CommandParser::parseFile(const std::string& filename) {
    std::vector<ParseResult> results;
    std::ifstream file(filename);

    if (!file.is_open()) {
        ParseResult error = ParseResult::Error("Failed to open file: " + filename, 0);
        results.push_back(std::move(error));
        return results;
    }

    std::string line;
    size_t line_number = 0;

    while (std::getline(file, line)) {
        ++line_number;
        ParseResult result = parseLine(line, line_number);
        
        // Only add non-empty results (skip empty lines/comments)
        if (result.success || !result.error_message.empty()) {
            results.push_back(std::move(result));
        }
    }

    return results;
}

ParseResult CommandParser::parseDimension(const std::vector<std::string>& tokens, size_t line_number) {
    if (tokens.size() != 2) {
        return ParseResult::Error("DIMENSION command requires exactly one argument: DIMENSION N", line_number);
    }

    try {
        int n = std::stoi(tokens[1]);
        if (n <= 0) {
            return ParseResult::Error("DIMENSION size must be positive", line_number);
        }

        return ParseResult::Success(
            std::make_unique<DimensionCommand>(static_cast<size_t>(n)),
            line_number
        );
    } catch (const std::exception&) {
        return ParseResult::Error("Invalid DIMENSION argument: " + tokens[1], line_number);
    }
}

ParseResult CommandParser::parseMoveTo(const std::vector<std::string>& tokens, size_t line_number) {
    if (tokens.size() != 2) {
        return ParseResult::Error("MOVE_TO command requires coordinates: MOVE_TO x,y", line_number);
    }

    int x, y;
    if (!parseCoordinates(tokens[1], x, y)) {
        return ParseResult::Error("Invalid MOVE_TO coordinates: " + tokens[1], line_number);
    }

    return ParseResult::Success(
        std::make_unique<MoveToCommand>(x, y),
        line_number
    );
}

ParseResult CommandParser::parseLineTo(const std::vector<std::string>& tokens, size_t line_number) {
    if (tokens.size() != 2) {
        return ParseResult::Error("LINE_TO command requires coordinates: LINE_TO x,y", line_number);
    }

    int x, y;
    if (!parseCoordinates(tokens[1], x, y)) {
        return ParseResult::Error("Invalid LINE_TO coordinates: " + tokens[1], line_number);
    }

    return ParseResult::Success(
        std::make_unique<LineToCommand>(x, y),
        line_number
    );
}

bool CommandParser::parseCoordinates(const std::string& coord_str, int& x, int& y) {
    std::vector<std::string> coords = split(coord_str, ',');
    
    if (coords.size() != 2) {
        return false;
    }

    try {
        x = std::stoi(trim(coords[0]));
        y = std::stoi(trim(coords[1]));
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

std::string CommandParser::trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }

    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        --end;
    }

    return str.substr(start, end - start);
}

std::vector<std::string> CommandParser::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(str);

    while (std::getline(token_stream, token, delimiter)) {
        std::string trimmed = trim(token);
        if (!trimmed.empty()) {
            tokens.push_back(trimmed);
        }
    }

    return tokens;
}
