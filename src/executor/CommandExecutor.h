#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include "../commands/ICommand.h"
#include "../model/Grid.h"
#include "../model/RobotState.h"
#include "../model/ThreadSafeQueue.h"
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <functional>

/**
 * @brief Execution result for a single command
 */
struct ExecutionResult {
    bool success;
    std::string error_message;
    size_t line_number;
    std::string command_type;

    ExecutionResult(bool success_, size_t line_num, const std::string& cmd_type)
        : success(success_), line_number(line_num), command_type(cmd_type) {}

    ExecutionResult(bool success_, const std::string& error, size_t line_num, const std::string& cmd_type)
        : success(success_), error_message(error), line_number(line_num), command_type(cmd_type) {}
};

/**
 * @brief Command with metadata for execution
 */
struct CommandWithMetadata {
    std::unique_ptr<ICommand> command;
    size_t line_number;

    CommandWithMetadata() : line_number(0) {}
    CommandWithMetadata(std::unique_ptr<ICommand> cmd, size_t line_num)
        : command(std::move(cmd)), line_number(line_num) {}
};

/**
 * @brief Callback function type for real-time rendering
 * Called after each command execution for live updates
 */
using RenderCallback = std::function<void(const std::string& command_type, size_t line_number)>;

/**
 * @brief Executes commands from a thread-safe queue
 * 
 * Supports concurrent execution with command parsing.
 * Waits for DIMENSION command before processing movement commands.
 * Optionally calls a callback after each command for real-time rendering.
 */
class CommandExecutor {
public:
    /**
     * @brief Construct executor with shared grid and robot state
     * @param grid Reference to grid
     * @param robot Reference to robot state
     * @param renderCallback Optional callback for real-time rendering (called after each command)
     */
    CommandExecutor(Grid& grid, RobotState& robot, RenderCallback renderCallback = nullptr)
        : mGrid(grid), mRobot(robot), mRunning(false), mDimensionReceived(false), 
          mRenderCallback(std::move(renderCallback)) {}

    /**
     * @brief Start executor thread
     * @param commandQueue Queue to consume commands from
     */
    void start(ThreadSafeQueue<CommandWithMetadata>& commandQueue);

    /**
     * @brief Stop executor (called after parsing completes)
     */
    void stop();

    /**
     * @brief Wait for executor to finish processing all commands
     */
    void wait();

    /**
     * @brief Get execution results
     * @return Vector of execution results for all commands
     */
    std::vector<ExecutionResult> getResults() const;

    /**
     * @brief Check if any errors occurred
     * @return true if errors occurred, false otherwise
     */
    bool hasErrors() const;

private:
    Grid& mGrid;
    RobotState& mRobot;
    
    std::thread mExecutorThread;
    std::atomic<bool> mRunning;
    std::atomic<bool> mDimensionReceived;
    RenderCallback mRenderCallback;
    
    std::vector<ExecutionResult> mResults;
    mutable std::mutex mResultsMutex;

    /**
     * @brief Main execution loop (runs in separate thread)
     */
    void executionLoop(ThreadSafeQueue<CommandWithMetadata>& commandQueue);

    /**
     * @brief Execute a single command
     */
    ExecutionResult executeCommand(CommandWithMetadata& cmdMeta);
};

#endif // COMMAND_EXECUTOR_H
