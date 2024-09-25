#include <asio.hpp>
#include <iostream>

int main()
{
    try
    {
        asio::io_context io_context;

        asio::ip::udp::socket socket{io_context};
        socket.open(asio::ip::udp::v4());

        socket.send_to(
                asio::buffer("Hello world!"),
                asio::ip::udp::endpoint{asio::ip::make_address("127.0.0.1"), 4242});
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    return 0;
}