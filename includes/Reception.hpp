/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** Reception
*/

#ifndef RECEPTION_HPP_
#define RECEPTION_HPP_
#include "Kitchen.hpp"
#include "Process.hpp"
#include "Pizza.hpp"
#include "MsgQueue.hpp"
#include <iostream>
#include <unordered_map>
#include <map>

class Reception {
    public:
        class ReceptionException : public std::exception {
            public:
                ReceptionException(std::string msg) noexcept : _msg(msg) {}
                const char *what() const noexcept override {return _msg.c_str();};
            private:
                std::string _msg;
        };
        Reception(float cooking_time, int cook_nbr, int refill_time);
        void get_user_inputs();
        ~Reception();

    private:
        void interpret_command(std::string line);
        void create_kitchens(int pizzas_nbr);
        void handle_kitchen();
        int define_pizzas_nb(std::string str);
        size_t define_new_kitchens_nb(int pizzasNb);
        void handle_empty_kitchen(Kitchen &kitchen, Pizza &pizza);
        void send_kitchen_status(size_t cooksAvailable);
        void reset_status();
        size_t get_cooks_available() const;
        void print_status();
        void handle_command(Pizza pizza, size_t pizzasNbr);
        size_t getUnsignedFromString(std::string message, int nb);
        bool insensitive_compare_strings(const std::string &first, const std::string &second);
        void pizzas_list();
        float _cookingTime;
        int _cookNbr;
        int _refillTime;
        IPCMessageQueue _pizzaQueue;
        std::unordered_map<size_t, Process> _kitchens;
        std::unordered_map<size_t, size_t> _kitchensStatus;
        size_t _kitchenCounter = 0;
};

#endif /* !RECEPTION_HPP_ */
