/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include "Client.hpp"
#include <iostream>
#include <csignal>

static sig_atomic_t handleClient(const volatile sig_atomic_t new_status)
{
    static volatile sig_atomic_t status = 1;

    if (new_status != 1)
        status = new_status;
    return status;
}

static void handleCtrlC(int signal)
{
    if (signal == SIGINT)
        exit(0);
}

int main(int ac, char** av)
{
    if (ac != 2 || av[1] == NULL) {
        std::cout << "Usage: ./client [port]" << std::endl;
        return -1;
    }

    try {
        asio::io_context ioContext;
        Client client(ioContext, "127.0.0.1", av[1]);
        std::string message;

        signal(SIGINT, handleCtrlC);
        while (handleClient(1)) {

            if (!client.getIsRunning()) {
                client.sendMessage("Client disconnected");
                break;
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
