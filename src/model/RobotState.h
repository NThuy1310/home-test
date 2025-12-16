#ifndef ROBOT_STATE_H
#define ROBOT_STATE_H

#include "Point.h"
#include <mutex>

/**
 * @brief Thread-safe robot state management
 * Tracks the current position of the robot
 */
class RobotState {
public:
    RobotState() : mPosition(0, 0) {}

    /**
     * @brief Get current robot position
     * @return Current position
     */
    Point getPosition() const {
        std::lock_guard<std::mutex> lock(mMutex);
        return mPosition;
    }

    /**
     * @brief Set robot position
     * @param newPos New position
     */
    void setPosition(const Point& newPos) {
        std::lock_guard<std::mutex> lock(mMutex);
        mPosition = newPos;
    }

    /**
     * @brief Reset robot to origin (0, 0)
     */
    void reset() {
        std::lock_guard<std::mutex> lock(mMutex);
        mPosition = Point(0, 0);
    }

private:
    mutable std::mutex mMutex;
    Point mPosition;
};

#endif // ROBOT_STATE_H
