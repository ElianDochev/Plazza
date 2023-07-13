/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** Kitchen
*/

#include "Kitchen.hpp"

Kitchen::Kitchen(int cook_nb, int restock_time, float cook_time, size_t stock)
    : _thread_pool(cook_nb), _stock(std::make_shared<Stock>("./pizzas.delamama", stock)),
      _cook_time(cook_time), _cook(_stock, cook_time),
      _cook_nb(cook_nb), _restock_time(restock_time),
      _last_restock(std::chrono::system_clock::now()), _last_change(0) {}

Kitchen::~Kitchen() {}

bool Kitchen::kitchenIsFull() const
{
    return _thread_pool.isSuperSaturated();
}

bool Kitchen::kitchenIsEmpty() const
{
    std::map<size_t, bool> status = _thread_pool.getThreadsStatus();

    for (auto &i : status)
        if (i.second == true)
            return false;
    return true;
}

std::map<size_t, bool> Kitchen::getThreadsStatus() const
{
    return _thread_pool.getThreadsStatus();
}

// should work but not tested yet
void Kitchen::addPizzaToQueue(Pizza &pizza)
{
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _stock->get_from_stock(pizza);
    }
    _thread_pool.enqueue(&Cook::cook_pizza, &_cook, pizza);
}

bool Kitchen::canCook(Pizza &pizza) const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _stock->can_get_from_stock(pizza);
}

size_t Kitchen::cooksAvailable() const
{
    std::map<size_t, bool> cooksStatus = getThreadsStatus();
    size_t cooksNbr = 0;

    for (auto &i : cooksStatus)
        if (i.second == false)
            cooksNbr++;
    return cooksNbr;
}

void Kitchen::restock()
{
    std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::_V2::system_clock::time_point restock_time = _last_restock + std::chrono::milliseconds(_restock_time);

    if (now < restock_time)
        return;
    // {
    //     std::unique_lock<std::mutex> lock(_mutex);
    //     auto stock = _stock->get_stock();
    //     for (auto &ingredient : stock)
    //     {
    //         std::cerr << ingredient.first << ": " << ingredient.second << std::endl;
    //     }
    //     std::cerr << std::endl;
    // }
    std::unique_lock<std::mutex> lock(_mutex);
    _stock->refresh_stock();
    _last_restock = std::chrono::system_clock::now();
}

bool Kitchen::changeOccured()
{
    size_t current_cooks = cooksAvailable();

    if (_last_change == current_cooks)
        return false;
    _last_change = current_cooks;
    return true;
}
