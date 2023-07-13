/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** Stock
*/

#ifndef STOCK_HPP_
#define STOCK_HPP_

#include <memory>
#include <condition_variable>
#include <map>

#include "Pizza.hpp"
//! TODO: Stock class.
/*
    must have a cobstructor that takes a size_t as parameter for max stock
    a method to refresh stock
    a method to take from stock
*/
class Stock
{
public:
    class StockException : public std::exception
    {
    public:
        StockException(std::string msg) noexcept : _msg(msg) {}
        const char *what() const noexcept override { return _msg.c_str(); };

    private:
        std::string _msg;
    };
    Stock(std::string recipes_file, size_t max_stock);
    ~Stock(){};
    void refresh_stock();              // call this method every x seconds from the kitchen
    bool get_from_stock(Pizza &pizza); // called by the cook
    bool can_get_from_stock(Pizza &pizza);
    std::map<std::string, ssize_t> get_stock() const;

private:
    void fill_vec(std::string line, std::vector<std::string> &ingredients);
    size_t _max_stock;
    std::map<std::string, ssize_t> _stock;
};

#endif /* !STOCK_HPP_ */
