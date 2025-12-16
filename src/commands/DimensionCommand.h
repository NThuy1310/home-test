#ifndef DIMENSION_COMMAND_H
#define DIMENSION_COMMAND_H

#include "ICommand.h"
#include <string>

/**
 * @brief Command to set grid dimensions
 * Format: DIMENSION N
 */
class DimensionCommand : public ICommand {
public:
    /**
     * @brief Construct a DIMENSION command
     * @param n Grid size (N x N)
     */
    explicit DimensionCommand(size_t n) : mSize(n) {}

    void execute(Grid& grid, RobotState& robot) override;
    bool validate() const override;
    std::string getType() const override { return "DIMENSION"; }

    size_t getSize() const { return mSize; }

private:
    size_t mSize;
};

#endif // DIMENSION_COMMAND_H
