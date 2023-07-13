#include "ThreadPool.hpp"

#include <iostream>
#include <unistd.h>
class Task
{
public:
    Task() = default;
    Task(const Task &src) = default;
    Task &operator=(const Task &src) = default;
    Task(Task &&src) = default;
    Task &operator=(Task &&src) = default;
    virtual ~Task() = default;
    void Test(int num)
    {
        std::unique_lock<std::mutex> lock(mutex);
        ++_count;
        std::cout << "Test: " << num << " Count: " << _count << std::endl;
        sleep(1);
    }

private:
    std::mutex mutex;
    size_t _count = 0;
};

void test_func(int num)
{
    sleep(1);
    std::cout << "Test: " << num << std::endl;
}

int main(void)
{
    ThreadPool pool(5);
    Task test;
    for (int i = 0; i < 30; i++)
    {
        pool.enqueue(&Task::Test, &test, i); // pass the method and the object and then the arguments
        // if (pool.isFull())
        //     std::cerr << "Pool is full " << i << std::endl;
        if (i % 3 == 0 && i != 0)
        {
            auto status = pool.getThreadsStatus();
            for (auto &i : status)
            {
                std::cerr << "Thread " << i.first << " is " << (i.second ? "busy" : "free") << std::endl;
            }
        }
    }
    sleep(1);
    pool.enqueue(test_func, 10);
    return (0);
}