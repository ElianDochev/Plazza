#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t numThreads) : stop(false)
{
    for (size_t i = 0; i < numThreads; ++i)
    {
        _threads_status[i] = false;
        threads.emplace_back([this, i]
                             {
            auto id = i;
            while (true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->mutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    if (this->stop && this->tasks.empty())
                        return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                {
                    std::unique_lock<std::mutex> lock(this->mutex);
                    _threads_status[id] = true;
                }
                task();
                {
                    std::unique_lock<std::mutex> lock(this->mutex);
                    _threads_status[id] = false;
                }
            } });
    }
}

bool ThreadPool::isFull() const
{
    std::unique_lock<std::mutex> lock(mutex);
    return tasks.size() >= threads.size();
}

bool ThreadPool::isSuperSaturated() const
{
    std::unique_lock<std::mutex> lock(mutex);
    return tasks.size() >= threads.size() * 1.9;
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &thread : threads)
        thread.join();
}

std::map<size_t, bool> ThreadPool::getThreadsStatus() const
{
    std::unique_lock<std::mutex> lock(mutex);
    return _threads_status;
}
