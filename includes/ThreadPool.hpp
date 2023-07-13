#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <map>

class ThreadPool
{
public:
    ThreadPool(size_t numThreads);

    std::mutex &getMutex() { return mutex; }

    std::condition_variable &getCondition() { return condition; }

    template <typename Function, typename... Args>
    void enqueue(Function &&function, Args &&...args) const
    {
        std::unique_lock<std::mutex> lock(mutex);
        tasks.emplace([=]() mutable
                      { std::invoke(function, args...); });
        condition.notify_one();
    }

    bool isFull() const;

    bool isSuperSaturated() const;

    std::map<size_t, bool> getThreadsStatus() const;

    size_t getNumThreads() const { return threads.size(); }

    ~ThreadPool();

private:
    std::vector<std::thread> threads;
    std::map<size_t, bool> _threads_status;
    mutable std::queue<std::function<void()>> tasks;
    mutable std::mutex mutex;
    mutable std::condition_variable condition;
    mutable bool stop;
};

#endif /* !THREADPOOL_HPP_ */