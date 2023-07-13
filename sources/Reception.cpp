/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** Reception
*/

#include "Reception.hpp"
#include <chrono>
#include <sstream>
#include <fstream>

Reception::Reception(float cooking_time, int cook_nbr, int refill_time)
    : _cookingTime(cooking_time), _cookNbr(cook_nbr), _refillTime(refill_time),
      _pizzaQueue("/pizzaQueue")
{
    if (cooking_time <= 0)
        throw Reception::ReceptionException("Reception error: Invalid cooking time");
    if (cook_nbr <= 0)
        throw Reception::ReceptionException("Reception error: Invalid number of cooks");
    if (refill_time <= 0)
        throw Reception::ReceptionException("Reception error: Invalid refill time");
    std::cout << "Hello sir welcome to la plaza de la mama!\nWhat would you like to order?" << std::endl;
    std::cout << "Type \"status\" to see how much cooks are available in the kitchens and \"pizzas\" to see the list of pizzas!" << std::endl;
};

Reception::~Reception()
{
    for (auto &kitchen : _kitchens)
        kitchen.second.terminate_child();
}

int Reception::define_pizzas_nb(std::string str)
{
    if (str[0] != 'x' && str[0] != 'X')
        return -1;
    str.erase(0, 1);
    return std::atoi(str.c_str());
}

void Reception::interpret_command(std::string line)
{
    Pizza pizza;
    std::vector<std::string> array;
    std::stringstream ss(line);
    std::string tmp;
    int pizzasNb = 0;

    while (std::getline(ss, tmp, ' '))
        array.push_back(tmp);
    if (array.size() != 3)
    {
        std::cout << "Invalid command please select a valid pizza, a size, and the numbers of pizzas you want" << std::endl;
        return;
    }
    pizzasNb = define_pizzas_nb(array[2]);
    try
    {
        pizza.create_pizza(array[0], array[1]);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
    if (pizzasNb <= 0)
    {
        std::cout << "Invalid command please select a valid pizza, a size, and the numbers of pizzas you want" << std::endl;
        return;
    }
    reset_status();
    handle_command(pizza, pizzasNb);
}

void Reception::handle_command(Pizza pizza, size_t pizzasNbr)
{
    if (get_cooks_available() < pizzasNbr)
        create_kitchens(define_new_kitchens_nb(pizzasNbr));
    for (size_t i = 0; i < pizzasNbr; i++)
        pizza.pack(_pizzaQueue);
}

void Reception::create_kitchens(int kitchens_nbr)
{
    for (int i = 0; i < kitchens_nbr; i++)
    {
        _kitchens.emplace(_kitchenCounter, _kitchenCounter);
        if (_kitchens.find(_kitchenCounter)->second.is_child())
            handle_kitchen();
        _kitchensStatus.emplace(_kitchenCounter, _cookNbr);
        _kitchenCounter++;
    }
}

size_t Reception::get_cooks_available() const
{
    size_t cooksAvailable = 0;

    for (auto &kitchen : _kitchensStatus)
        cooksAvailable += kitchen.second;
    return cooksAvailable;
}

size_t Reception::define_new_kitchens_nb(int pizzasNb)
{
    int pizzasLeft = pizzasNb - (get_cooks_available() * 2);

    if (pizzasLeft <= 0)
        return 0;
    size_t result = pizzasLeft / (_cookNbr * 2);

    if (pizzasLeft % (_cookNbr * 2) != 0)
        result++;
    return result;
}

void Reception::handle_empty_kitchen(Kitchen &kitchen, Pizza &pizza)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::string message;

    while (duration.count() != 5)
    {
        if (_pizzaQueue.receive(message))
        {
            pizza.unpack(message);
            if (kitchen.canCook(pizza))
            {
                kitchen.addPizzaToQueue(pizza);
                return;
            }
            else
                pizza.pack(_pizzaQueue);
        }
        if (!kitchen.kitchenIsEmpty())
            return;
        kitchen.restock();
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    }
    message = "f";
    _kitchens.find(_kitchenCounter)->second._returnQueue.empty_queue();
    _kitchens.find(_kitchenCounter)->second._returnQueue.send(message);
    std::cout << "Kitchen : " << _kitchenCounter << " closed" << std::endl;
    _kitchens.find(_kitchenCounter)->second.close_process();
}

void Reception::send_kitchen_status(size_t cooksAvailable)
{
    std::ostringstream oss;
    std::string message;

    oss.write(reinterpret_cast<char *>(&cooksAvailable), sizeof(size_t));
    message = oss.str();
    _kitchens.find(_kitchenCounter)->second._returnQueue.empty_queue();
    _kitchens.find(_kitchenCounter)->second._returnQueue.send(message);
}

size_t Reception::getUnsignedFromString(std::string message, int nb)
{
    std::string temp = message.substr(0, sizeof(size_t)).c_str();
    size_t res = 0;
    if (nb == 1)
    {
        for (auto &i : temp)
            res = static_cast<size_t>(static_cast<unsigned char>(i));
        return res;
    }
    else if (nb == 2)
    {
        temp = message.substr(sizeof(size_t), sizeof(size_t) * 2).c_str();
        for (auto &i : temp)
            res = static_cast<size_t>(static_cast<unsigned char>(i));
        return res;
    }
    return res;
}

void Reception::reset_status()
{
    std::string message;
    size_t cooksAvailable = 0;
    size_t kitchensNbr = _kitchenCounter;

    for (size_t i = 0; i < kitchensNbr; i++)
    {
        while (_kitchens.find(i)->second._returnQueue.receive(message))
        {
            if (message.length() == sizeof(size_t))
            {
                cooksAvailable = getUnsignedFromString(message, 1);
                _kitchensStatus[i] = cooksAvailable;
            }
            else if (message == "f")
            {
                _kitchensStatus.erase(i);
                _kitchens.erase(i);
                _kitchenCounter--;
                break;
            }
            else
                throw ReceptionException("Reception error: invalid message in the returnQueue");
        }
    }
}

void Reception::handle_kitchen()
{
    Kitchen kitchen(_cookNbr, _refillTime, _cookingTime);
    std::string message;
    Pizza pizza;

    std::cout << "Kitchen: " << _kitchenCounter << " is up!" << std::endl;
    while (1)
    {
        if (!kitchen.kitchenIsFull() && _pizzaQueue.receive(message))
        {
            pizza.unpack(message);
            if (kitchen.canCook(pizza))
                kitchen.addPizzaToQueue(pizza);
            else
                pizza.pack(_pizzaQueue);
        }
        if (kitchen.changeOccured())
            send_kitchen_status(kitchen.cooksAvailable());
        if (kitchen.kitchenIsEmpty())
            handle_empty_kitchen(kitchen, pizza);
        kitchen.restock();
    }
}

bool Reception::insensitive_compare_strings(const std::string &a, const std::string &b)
{
    return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b)
                      { return tolower(a) == tolower(b); });
}

void Reception::pizzas_list()
{
    std::ifstream file;
    std::string line;

    file.open("pizzas.delamama");
    if (!file.is_open())
        throw ReceptionException("Pizza error : Can't find las pizzas de la mama! (pizzas.delamama)");
    while (std::getline(file, line))
        std::cout << line << std::endl;
    std::cout << "If you'd like to personalize you're pizza feel free to add it in the pizza.delamama file!" << std::endl;
}

void Reception::print_status()
{
    reset_status();
    if (_kitchensStatus.empty())
        std::cout << "No kitchens are open" << std::endl;
    for (size_t i = 0; i < _kitchenCounter; i++)
        std::cout << "Kitchen : " << i << " has " << _kitchensStatus[i] << " cooks available" << std::endl;
}

void Reception::get_user_inputs()
{
    std::string line;

    while (std::getline(std::cin, line))
    {
        if (insensitive_compare_strings(line, "status"))
            print_status();
        else if (insensitive_compare_strings(line, "pizzas"))
            pizzas_list();
        else
            interpret_command(line);
    }
}
