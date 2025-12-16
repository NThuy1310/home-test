#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

/**
 * @brief Thread-safe queue implementation for producer-consumer pattern
 * @tparam T Type of elements stored in the queue
 */
template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() : mShutdown(false) {}

    /**
     * @brief Push an item to the queue
     * @param item Item to push
     */
    void push(T item) {
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mQueue.push(std::move(item));
        }
        mCondVar.notify_one();
    }

    /**
     * @brief Pop an item from the queue (blocking)
     * @return Optional containing the item, or nullopt if queue is shutdown
     */
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mMutex);
        mCondVar.wait(lock, [this] { return !mQueue.empty() || mShutdown; });

        if (mQueue.empty() && mShutdown) {
            return std::nullopt;
        }

        T item = std::move(mQueue.front());
        mQueue.pop();
        return item;
    }

    /**
     * @brief Try to pop an item without blocking
     * @return Optional containing the item, or nullopt if queue is empty
     */
    std::optional<T> tryPop() {
        std::lock_guard<std::mutex> lock(mMutex);
        if (mQueue.empty()) {
            return std::nullopt;
        }

        T item = std::move(mQueue.front());
        mQueue.pop();
        return item;
    }

    /**
     * @brief Check if queue is empty
     * @return true if empty, false otherwise
     */
    bool empty() const {
        std::lock_guard<std::mutex> lock(mMutex);
        return mQueue.empty();
    }

    /**
     * @brief Get queue size
     * @return Number of elements in queue
     */
    size_t size() const {
        std::lock_guard<std::mutex> lock(mMutex);
        return mQueue.size();
    }

    /**
     * @brief Signal shutdown to waiting consumers
     */
    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mShutdown = true;
        }
        mCondVar.notify_all();
    }

    /**
     * @brief Check if queue is in shutdown state
     * @return true if shutdown, false otherwise
     */
    bool isShutdown() const {
        std::lock_guard<std::mutex> lock(mMutex);
        return mShutdown;
    }

private:
    mutable std::mutex mMutex;
    std::condition_variable mCondVar;
    std::queue<T> mQueue;
    bool mShutdown;
};

#endif // THREAD_SAFE_QUEUE_H
