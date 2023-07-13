/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** Cook
*/

#include "Cook.hpp"
#include <chrono>
#include <cmath>

Cook::Cook(std::shared_ptr<Stock> stock_ptr, float cook_time_modif) : _stock(stock_ptr),
                                                                      _cook_time_modif(cook_time_modif) {}

Cook::~Cook() {}

// will be called by the thread pool!! Check kitchen.CanCook() before calling this function
void Cook::cook_pizza(Pizza &pizza)
{
    std::cout << "Cooking pizza: " << pizza.get_type() << ", Size: " << pizza.get_size() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds((int)std::round((pizza.get_time() * 1000) * _cook_time_modif)));
    std::cout << "Pizza " << pizza.get_type() << ", Size: " << pizza.get_size() << " is ready" << std::endl;
}