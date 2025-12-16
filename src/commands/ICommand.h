#ifndef ICOMMAND_H
#define ICOMMAND_H

#include "../model/Grid.h"
#include "../model/RobotState.h"

/**
 * @brief Interface for all robot commands
 * 
 * Uses Command Pattern to encapsulate commands as objects.
 * This enables:
 * - Easy extensibility (add new commands without modifying existing code)
 * - Command queuing and execution
 * - Command validation before execution
 * - Potential for undo/redo (future enhancement)
 */
class ICommand {
public:
    virtual ~ICommand() = default;

    /**
     * @brief Execute the command
     * @param grid Reference to the grid
     * @param robot Reference to the robot state
     * @throws std::runtime_error if execution fails
     */
    virtual void execute(Grid& grid, RobotState& robot) = 0;

    /**
     * @brief Validate command parameters
     * @return true if command is valid, false otherwise
     */
    virtual bool validate() const = 0;

    /**
     * @brief Get command type name (for debugging/logging)
     * @return Command type as string
     */
    virtual std::string getType() const = 0;
};

#endif // ICOMMAND_H
