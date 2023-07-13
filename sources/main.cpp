/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** main
*/

#include "Reception.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 4)
        return 84;
    try
    {
        Reception shell(std::atoi(argv[1]), std::atoi(argv[2]), std::atoi(argv[3]));

        shell.get_user_inputs();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 84;
    }
    return 0;
}
