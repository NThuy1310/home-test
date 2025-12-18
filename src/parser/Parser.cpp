#include "parser/Parser.h"
#include "commands/CommandFactory.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

Parser::ParseResult Parser::parseFile(const std::string& filename) {
    ParseResult result;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::string line;
    int lineNumber = 0;
    bool dimensionFound = false;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string commandType;
        iss >> commandType;
        
        try {
            if (commandType == "DIMENSION") {
                // Only take the first DIMENSION command
                if (dimensionFound) {
                    result.errors.push_back("Line " + std::to_string(lineNumber) + ": Duplicate DIMENSION command");
                    result.hasErrors = true;
                    continue;
                }
                
                size_t n;
                if (!(iss >> n) || n <= 0) {
                    result.errors.push_back("Line " + std::to_string(lineNumber) + ": Invalid DIMENSION command format");
                    result.hasErrors = true;
                    continue;
                }
                
                ParsedCommand cmd;
                cmd.type = CommandFactory::CommandType::DIMENSION;
                cmd.dimension = n;
                cmd.lineNumber = lineNumber;
                result.parsedCommands.push_back(std::move(cmd));
                result.gridSize = n;
                dimensionFound = true;
            }
            // Take MOVE_TO and LINE_TO commands after DIMENSION command.
            else if (commandType == "MOVE_TO") {
                if (!dimensionFound) {
                    result.errors.push_back("Line " + std::to_string(lineNumber) + ": Commands before DIMENSION");
                    result.hasErrors = true;
                    continue;
                }
                
                int x, y;
                char comma;
                if (!(iss >> x >> comma >> y) || comma != ',') {
                    result.errors.push_back("Line " + std::to_string(lineNumber) + ": Invalid MOVE_TO command format");
                    result.hasErrors = true;
                    continue;
                }
                
                ParsedCommand cmd;
                cmd.type = CommandFactory::CommandType::MOVE_TO;
                cmd.x = x;
                cmd.y = y;
                cmd.lineNumber = lineNumber;
                result.parsedCommands.push_back(std::move(cmd));
            }
            else if (commandType == "LINE_TO") {
                if (!dimensionFound) {
                    result.errors.push_back("Line " + std::to_string(lineNumber) + ": Commands before DIMENSION");
                    result.hasErrors = true;
                    continue;
                }
                
                int x, y;
                char comma;
                if (!(iss >> x >> comma >> y) || comma != ',') {
                    result.errors.push_back("Line " + std::to_string(lineNumber) + ": Invalid LINE_TO command format");
                    result.hasErrors = true;
                    continue;
                }
                
                ParsedCommand cmd;
                cmd.type = CommandFactory::CommandType::LINE_TO;
                cmd.x = x;
                cmd.y = y;
                cmd.lineNumber = lineNumber;
                result.parsedCommands.push_back(std::move(cmd));
            }
            else {
                result.errors.push_back("Line " + std::to_string(lineNumber) + ": Unknown command: " + commandType);
                result.hasErrors = true;
            }
        }
        catch (const std::exception& e) {
            result.errors.push_back("Line " + std::to_string(lineNumber) + ": " + e.what());
            result.hasErrors = true;
        }
    }
    
    if (!dimensionFound) {
        throw std::runtime_error("No DIMENSION command found in file");
    }
    
    return result;
}

std::vector<std::unique_ptr<ICommand>> Parser::createCommands(const ParseResult& parseResult) {
    std::vector<std::unique_ptr<ICommand>> commands;
    
    for (const auto& cmd : parseResult.parsedCommands) {
        commands.push_back(CommandFactory::createCommand(cmd.type, cmd.dimension, cmd.x, cmd.y));
    }
    
    return commands;
}

void Parser::validateCommandBounds(ParseResult& parseResult) {
    for (auto it = parseResult.parsedCommands.begin(); it != parseResult.parsedCommands.end(); ) {
        const auto& cmd = *it;
        if (cmd.type == CommandFactory::CommandType::MOVE_TO || 
            cmd.type == CommandFactory::CommandType::LINE_TO) {
            if (!(cmd.x >= 0 && cmd.x < static_cast<int>(parseResult.gridSize) &&
                  cmd.y >= 0 && cmd.y < static_cast<int>(parseResult.gridSize))) {
                parseResult.errors.push_back("Line " + std::to_string(cmd.lineNumber) + ": Point (" + std::to_string(cmd.x) + "," + std::to_string(cmd.y) + ")"
                                            + " out of bounds [0, " + std::to_string(parseResult.gridSize - 1) + "]");
                parseResult.hasErrors = true;
                it = parseResult.parsedCommands.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
}
