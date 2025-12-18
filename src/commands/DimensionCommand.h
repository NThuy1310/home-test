#pragma once

#include "ICommand.h"
#include <string>

class DimensionCommand : public ICommand {
public:
    explicit DimensionCommand(size_t n) : mSize(n) {}

    void execute(Grid& grid, RobotState& robot) override;
    bool validate(const Grid& /*grid*/) const override;
    std::string getType() const override { return "DIMENSION"; }

    size_t getSize() const { return mSize; }

private:
    size_t mSize;
};
