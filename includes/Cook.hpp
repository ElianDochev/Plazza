/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** Cook
*/

#ifndef COOK_HPP_
#define COOK_HPP_

#include "Stock.hpp"
#include <memory>
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class Cook
{
public:
    Cook(std::shared_ptr<Stock> stock_ptr, float cook_time_modif);
    void cook_pizza(Pizza &pizza);
    ~Cook();

private:
    std::shared_ptr<Stock> _stock;
    float _cook_time_modif;
};

#endif /* !COOK_HPP_ */
