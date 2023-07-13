/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** Stock
*/

#include "Stock.hpp"
#include <iostream>
#include <fstream>
#include <utility>
#include <unordered_set>

Stock::Stock(std::string recipes_file, size_t max_stock) : _max_stock(max_stock)
{
    std::vector<std::string> ingredients;
    std::ifstream file;
    std::string line;

    file.open(recipes_file);
    if (!file.is_open())
        throw StockException("Stock error : Where is mama? We need more pizzas! (pizzas.delamama)");
    while (std::getline(file, line))
        fill_vec(line, ingredients);
    for (size_t i = 0; i < ingredients.size(); ++i)
    {
        _stock.emplace(ingredients[i], 5);
    }
};

void Stock::fill_vec(std::string line, std::vector<std::string> &ingredients)
{
    std::stringstream copy(line);
    std::string tmp;
    std::vector<std::string> array;

    while (std::getline(copy, tmp, ' '))
        array.push_back(tmp);
    for (unsigned int i = 2; i < array.size(); i++)
        ingredients.push_back(array[i]);
}

void Stock::refresh_stock()
{
    for (auto &ingredient : _stock)
    {
        ingredient.second += 1;
    }
}

bool Stock::can_get_from_stock(Pizza &pizza)
{
    for (auto &ingredient : pizza.get_ingredients())
    {
        if (_stock[ingredient] <= 0)
            return false;
    }
    return true;
}

bool Stock::get_from_stock(Pizza &pizza)
{
    std::unordered_set<std::string> ingredients;

    if (!can_get_from_stock(pizza))
        return false;
    for (auto &ingredient : pizza.get_ingredients())
    {
        if (ingredients.find(ingredient) != ingredients.end())
            continue;
        ingredients.insert(ingredient);
        _stock[ingredient] -= 1;
    }
    return true;
}

std::map<std::string, ssize_t> Stock::get_stock() const
{
    return _stock;
}
