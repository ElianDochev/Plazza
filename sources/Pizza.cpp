/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** Pizza
*/

#include "Pizza.hpp"
#include <iostream>
#include <fstream>

void Pizza::fill_vec(std::string line)
{
    std::stringstream ss(line);
    std::string tmp;
    std::vector<std::string> array;

    while (std::getline(ss, tmp, ' '))
        array.push_back(tmp);
    if (array.size() < 3)
        throw PizzaException("Pizza error : Not enough argument to create a pizza de la mama!");
    _type = array[0];
    _time = std::atoi(array[1].c_str());
    if (_time <= 0)
        throw PizzaException("Pizza error : Impossible time to cook a pizza de la mama!");
    for (unsigned int i = 2; i < array.size(); i++)
        _ingredients.push_back(array[i]);
}

PizzaSize Pizza::get_size(std::string size)
{
    if (size == "S" || size == "s")
        return S;
    if (size == "M" || size == "m")
        return M;
    if (size == "L" || size == "l")
        return L;
    if (size == "XL" || size == "xl")
        return XL;
    if (size == "XXL" || size == "xxl")
        return XXL;
    throw PizzaException("Pizza error : Invalid size for this pizza de la mama!");
}

std::vector<std::string> Pizza::get_ingredients() const
{
    return _ingredients;
}

void Pizza::create_pizza(std::string type, std::string size)
{
    std::ifstream file;
    std::string line;

    file.open("pizzas.delamama");
    if (!file.is_open())
        throw PizzaException("Pizza error : Can't find las pizzas de la mama! (pizzas.delamama)");
    for (size_t i = 0; std::getline(file, line); i++)
        if (!line.compare(0, type.size(), type)) {
            fill_vec(line);
            _id = i;
        }
    if (_time == 0)
        throw PizzaException("Pizza error : Can't find this pizza de la mama!");
    _size = get_size(size);
}

Pizza::Pizza(std::string type, std::string size)
{
    std::ifstream file;
    std::string line;

    _type = type;
    file.open("pizzas.delamama");
    if (!file.is_open())
        throw PizzaException("Pizza error : Can't find las pizzas de la mama! (pizzas.delamama)");
    for (size_t i = 0; std::getline(file, line); i++)
        if (!line.compare(0, type.size(), type)) {
            fill_vec(line);
            _id = i;
        }
    if (_time == 0)
        throw PizzaException("Pizza error : Can't find this pizza de la mama!");
    _size = get_size(size);
}

char Pizza::convert_size_to_byte()
{
    if (_size == S)
        return S;
    if (_size == M)
        return M;
    if (_size == L)
        return L;
    if (_size == XL)
        return XL;
    if (_size == XXL)
        return XXL;
    throw PizzaException("Pizza error : Invalid size for this pizza de la mama!");
}

void Pizza::pack(IPCMessageQueue &pizzaQueue)
{
    std::string message = "  ";

    message[0] = _id;
    message[1] = convert_size_to_byte();
    pizzaQueue << message;
}

PizzaSize Pizza::convert_byte_to_size(char size)
{
    if (size == 0)
        return S;
    if (size == 1)
        return M;
    if (size == 2)
        return L;
    if (size == 3)
        return XL;
    if (size == 4)
        return XXL;
    throw PizzaException("Pizza error : Invalid size for this pizza de la mama! while unpacking");
}

void Pizza::unpack(std::string message)
{
    std::ifstream file;
    std::string line;

    if (message.length() > 2)
        throw PizzaException("Pizza error : Invalid string for unpack function");
    file.open("pizzas.delamama");
    if (!file.is_open())
        throw PizzaException("Pizza error : Can't find las pizzas de la mama! (pizzas.delamama) while unpacking");
    for (int i = 0; i < message[0] + 1; i++)
        if (!std::getline(file, line))
            throw PizzaException("Pizza error : invalid id can't unpack pizza de la mama!");
    fill_vec(line);
    _size = convert_byte_to_size(message[1]);
    _id = message[0];
}

std::string Pizza::get_type() const
{
    return _type;
}

int Pizza::get_time() const
{
    return _time;
}

PizzaSize Pizza::get_size() const
{
    return _size;
}

std::ostream &operator<<(std::ostream &os, const PizzaSize &pizza)
{
    if (pizza == S)
        os << "S";
    if (pizza == M)
        os << "M";
    if (pizza == L)
        os << "L";
    if (pizza == XL)
        os << "XL";
    if (pizza == XXL)
        os << "XXL";
    return os;
}