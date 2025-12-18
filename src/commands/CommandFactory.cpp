#include "CommandFactory.h"

std::unique_ptr<ICommand> CommandFactory::createCommand(CommandType type, size_t dimension, int x, int y) {
    switch (type) {
        case CommandType::DIMENSION:
            return std::make_unique<DimensionCommand>(dimension);
        case CommandType::MOVE_TO:
            return std::make_unique<MoveToCommand>(x, y);
        case CommandType::LINE_TO:
            return std::make_unique<LineToCommand>(x, y);
        default:
            return nullptr;
    }
}
