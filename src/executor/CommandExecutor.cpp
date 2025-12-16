#include "CommandExecutor.h"
#include "../commands/DimensionCommand.h"
#include <iostream>

void CommandExecutor::start(ThreadSafeQueue<CommandWithMetadata>& commandQueue) {
    mRunning = true;
    mExecutorThread = std::thread(&CommandExecutor::executionLoop, this, std::ref(commandQueue));
}

void CommandExecutor::stop() {
    mRunning = false;
}

void CommandExecutor::wait() {
    if (mExecutorThread.joinable()) {
        mExecutorThread.join();
    }
}

std::vector<ExecutionResult> CommandExecutor::getResults() const {
    std::lock_guard<std::mutex> lock(mResultsMutex);
    return mResults;
}

bool CommandExecutor::hasErrors() const {
    std::lock_guard<std::mutex> lock(mResultsMutex);
    for (const auto& result : mResults) {
        if (!result.success) {
            return true;
        }
    }
    return false;
}

void CommandExecutor::executionLoop(ThreadSafeQueue<CommandWithMetadata>& commandQueue) {
    while (mRunning || !commandQueue.empty()) {
        auto cmdOpt = commandQueue.pop();
        
        if (!cmdOpt.has_value()) {
            // Queue is shutdown and empty
            break;
        }

        CommandWithMetadata cmdMeta = std::move(cmdOpt.value());
        
        if (!cmdMeta.command) {
            continue;  // Skip null commands
        }

        // Check if this is a DIMENSION command
        if (cmdMeta.command->getType() == "DIMENSION") {
            mDimensionReceived = true;
        } else if (!mDimensionReceived) {
            // Movement command before DIMENSION - queue it back or error
            ExecutionResult result(
                false,
                "Movement command before DIMENSION",
                cmdMeta.line_number,
                cmdMeta.command->getType()
            );
            
            std::lock_guard<std::mutex> lock(mResultsMutex);
            mResults.push_back(result);
            continue;
        }

        // Execute the command
        ExecutionResult result = executeCommand(cmdMeta);
        
        {
            std::lock_guard<std::mutex> lock(mResultsMutex);
            mResults.push_back(result);
        }
    }
}

ExecutionResult CommandExecutor::executeCommand(CommandWithMetadata& cmdMeta) {
    try {
        cmdMeta.command->execute(mGrid, mRobot);
        
        // Call render callback if provided (for real-time rendering)
        if (mRenderCallback) {
            mRenderCallback(cmdMeta.command->getType(), cmdMeta.line_number);
        }
        
        return ExecutionResult(true, cmdMeta.line_number, cmdMeta.command->getType());
    } catch (const std::exception& e) {
        return ExecutionResult(
            false,
            e.what(),
            cmdMeta.line_number,
            cmdMeta.command->getType()
        );
    }
}
