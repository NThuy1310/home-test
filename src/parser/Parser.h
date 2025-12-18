#pragma once

#include "commands/ICommand.h"
#include "commands/CommandFactory.h"
#include <list>
#include <vector>
#include <memory>
#include <string>

class Parser {
public:
    struct ParsedCommand {
        CommandFactory::CommandType type;
        size_t dimension;
        int x, y;
        int lineNumber;
    };
    
    struct ParseResult {
        std::list<ParsedCommand> parsedCommands;
        std::vector<std::string> errors;
        size_t gridSize;
        bool hasErrors;
        
        ParseResult() : gridSize(0), hasErrors(false) {}
    };
    
    static ParseResult parseFile(const std::string& filename);
    static std::vector<std::unique_ptr<ICommand>> createCommands(const ParseResult& parseResult);
    static void validateCommandBounds(ParseResult& parseResult);
};
