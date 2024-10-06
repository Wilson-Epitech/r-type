/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Server
*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Lib.hpp"
#include <array>
#include <asio.hpp>
#include <memory>

#define UPDATE_SERVER 0.02
#define NB_CLIENTS_AUTHORIZED 4

class Server {
  public:
    Server(asio::io_context &ioContext, std::size_t port, asio::thread_pool &pool);
    ~Server();

    void sendToAClient(const std::size_t idClient, const std::string message);
    void sendToAllClient(const std::string message);
    std::map<std::size_t, asio::ip::udp::endpoint> getConnectedClients();
    std::size_t getNbClients();
    std::size_t getLastConnectedClientId();
    DataToSend getEntityData();

    std::unordered_map<std::size_t, bool> getAreClientsWorldSet() {
        return _areClientsWorldSet;
    }

  private:
    void startReceiving();
    void deserializeMessage(const std::string message);
    void handleReceive(const std::error_code &error, std::size_t bytesTransferred);
    void handleSend(std::shared_ptr<std::string> message, const std::error_code &error, std::size_t bytesTransferred);
    std::size_t logNewConnection(asio::ip::udp::endpoint endpoint);

    std::thread _ioThread;
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _remoteEndpoint;
    std::array<char, 10000> _receiveBuffer;

    std::size_t _idClient;

    asio::thread_pool &_threadPool;

    std::map<std::size_t, asio::ip::udp::endpoint> _connectedClients;
    std::unordered_map<std::size_t, bool> _areClientsWorldSet;

    DataToSend _data;
    bool _worldSet = false;
};

#endif
