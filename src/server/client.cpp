#include <asio.hpp>
#include <iostream>
#include <string>

int main()
{
    try {
        asio::io_context io_context;

        asio::ip::udp::socket socket{io_context};
        socket.open(asio::ip::udp::v4());

        asio::ip::udp::resolver resolver(io_context);
        asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(asio::ip::udp::v4(), "127.0.0.1", "4242").begin();

        std::string message;
        while (true) {
            std::getline(std::cin, message);

            if (message == "exit") {
                socket.send_to(asio::buffer("Client deconnected"), receiver_endpoint);
                break;
            }

            socket.send_to(asio::buffer(message), receiver_endpoint);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }

    return 0;
}
