#include <asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <array>

void msgFromServer(asio::ip::udp::socket& socket)
{
    std::array<char, 1024> receiveBuffer;
    asio::ip::udp::endpoint sendEndpoint;

    while (true) {
        asio::error_code error;
        std::size_t len = socket.receive_from(asio::buffer(receiveBuffer), sendEndpoint, 0, error);

        if (error) {
            std::cerr << "Receive failed: " << error.message() << std::endl;
            break;
        }

        std::cout << "Server: " << std::string(receiveBuffer.data(), len) << std::endl;
    }
}

int main(int ac, char **av)
{
    if (ac != 2 || av[1] == NULL) {
        std::cout << "Usage: ./client [port]" << std::endl;
        return -1;
    }
    try {
        asio::io_context ioContext;
        std::string message;

        asio::ip::udp::socket socket{ioContext};
        socket.open(asio::ip::udp::v4());

        asio::ip::udp::resolver resolver(ioContext);
        asio::ip::udp::endpoint receivEndpoint = *resolver.resolve(asio::ip::udp::v4(), "127.0.0.1", av[1]).begin();

        std::thread receiverThread(msgFromServer, std::ref(socket));

        while (true) {
            std::getline(std::cin, message);

            if (message == "exit") {
                socket.send_to(asio::buffer("Client disconnected"), receivEndpoint);
                break;
            }

            socket.send_to(asio::buffer(message), receivEndpoint);
        }

        receiverThread.join();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
