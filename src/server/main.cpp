/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include <iostream>
#include "Server.hpp"

int main()
{
    try {
        asio::io_context io_context;
        Server server(io_context);
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
