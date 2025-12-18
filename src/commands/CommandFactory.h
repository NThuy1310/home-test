#pragma once

#include "ICommand.h"
#include "DimensionCommand.h"
#include "MoveToCommand.h"
#include "LineToCommand.h"
#include <memory>
#include <string>

class CommandFactory {
public:
    enum class CommandType { DIMENSION, MOVE_TO, LINE_TO };
    
    static std::unique_ptr<ICommand> createCommand(CommandType type, size_t dimension = 0, int x = 0, int y = 0);
};
