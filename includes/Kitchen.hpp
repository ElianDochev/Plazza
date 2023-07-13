/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** Kitchens
*/

#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_
#include <vector>
#include "Cook.hpp"
#include "ThreadPool.hpp"
#include "MsgQueue.hpp"

class Kitchen
{
public:
    Kitchen(int cook_nb, int restock_time, float cook_time = 1, size_t stock = 5);
    bool kitchenIsFull() const;
    bool kitchenIsEmpty() const;
    std::map<size_t, bool> getThreadsStatus() const;
    void addPizzaToQueue(Pizza &pizza);
    void restock(); // should be called every loop interation
    bool canCook(Pizza &pizza) const;
    size_t cooksAvailable() const;
    std::map<std::string, size_t> get_stock() const;
    bool changeOccured();
    ~Kitchen();

private:
    ThreadPool _thread_pool;
    mutable std::mutex _mutex;
    std::shared_ptr<Stock> _stock;
    float _cook_time;
    Cook _cook;
    size_t _cook_nb;
    size_t _restock_time;
    std::chrono::_V2::system_clock::time_point _last_restock;
    size_t _last_change;
};

#endif /* !KITCHEN_HPP_ */
