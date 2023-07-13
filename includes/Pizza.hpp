/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** Pizza
*/

#ifndef PIZZA_HPP_
#define PIZZA_HPP_
#include <vector>
#include <sstream>
#include "MsgQueue.hpp"

enum PizzaSize
{
    S,
    M,
    L,
    XL,
    XXL,
    UndefinedSize
};

class Pizza
{
public:
    class PizzaException : public std::exception
    {
    public:
        PizzaException(std::string msg) noexcept : _msg(msg) {}
        const char *what() const noexcept override { return _msg.c_str(); };

    private:
        std::string _msg;
    };
    Pizza() = default;
    Pizza(std::string type, std::string size);
    void create_pizza(std::string type, std::string size);
    std::vector<std::string> get_ingredients() const;
    void pack(IPCMessageQueue &pizzaQueue);
    void unpack(std::string message);
    std::string get_type() const;
    int get_time() const;
    PizzaSize get_size() const;
    ~Pizza() = default;

private:
    PizzaSize convert_byte_to_size(char size);
    char convert_size_to_byte();
    void fill_vec(std::string line);
    PizzaSize get_size(std::string size);
    size_t _id = 0;
    std::string _type;
    PizzaSize _size = UndefinedSize;
    int _time = 0;
    std::vector<std::string> _ingredients;
};

std::ostream &operator<<(std::ostream &os, const PizzaSize &pizza);
#endif /* !PIZZA_HPP_ */
