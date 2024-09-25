/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** server
*/

#include <array>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <asio.hpp>

using asio::ip::udp;

class udp_server
{
    public:
        udp_server(asio::io_context& io_context): socket_(io_context, udp::endpoint(udp::v4(), 4242)) {
            start_receive();
        }

    private:
        void start_receive()
        {
            socket_.async_receive_from(
                asio::buffer(recv_buffer_), remote_endpoint_,
                std::bind(&udp_server::handle_receive, this,
                std::placeholders::_1,
                std::placeholders::_2));
        }

        void handle_receive(const std::error_code& error, std::size_t /*bytes_transferred*/)
        {
            if (!error) {
                std::shared_ptr<std::string> message(
                    new std::string("Server received\n"));

                socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
                    std::bind(&udp_server::handle_send, this, message,
                        std::placeholders::_1,
                        std::placeholders::_2));

                start_receive();
                std::cout << "Client: " << std::string(recv_buffer_.data()) << std::endl;
                std::fill(std::begin(recv_buffer_), std::end(recv_buffer_), 0);
            }
        }

        void handle_send(std::shared_ptr<std::string> /*message*/, const std::error_code& /*error*/, std::size_t /*bytes_transferred*/) {
        }

        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;
        std::size_t len;
};

int main()
{
  try
  {
    asio::io_context io_context;
    udp_server server(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
