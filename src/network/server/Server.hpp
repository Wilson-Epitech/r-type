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
#include <map>
#include <memory>

#define UPDATE_SERVER 0.02
#define NB_CLIENTS_AUTHORIZED 4
#define MAXMESSAGE 255

namespace network {
  class Server {
    public:
      Server(asio::io_context &ioContext, std::string host, std::size_t port, asio::thread_pool &pool);
      ~Server();

      void sendToAClient(const std::size_t idClient, const std::string message, uint8_t messageType);
      void sendToAllClient(const std::string message, uint8_t messageType);
      std::map<std::size_t, asio::ip::udp::endpoint> getConnectedClients();
      std::size_t getNbClients();
      std::size_t getLastConnectedClientId();
      std::unordered_map<std::size_t, bool> getAreClientsWorldSet();
      DataToSend getEntityData();
      bool getDataHasBeenUsed();
      void setDataHasBeenUsed(bool value);
      std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> getHeartbeatClient();

      std::unordered_map<std::size_t, bool> getClientStillHere();
      void setClientStillHere(std::unordered_map<std::size_t, bool>);
      std::unordered_map<std::size_t, std::size_t> getCorrespondence();

      void setHeartBeatmMap(std::unordered_map<std::size_t, std::chrono::steady_clock::time_point>);

    private:
      void startReceiving();
      void deserializeMessage(const std::string message);
      void handleReceive(const std::error_code &error, std::size_t bytesTransferred);
      void handleSend(const std::error_code &error, std::size_t bytesTransferred);
      std::size_t logNewConnection(asio::ip::udp::endpoint endpoint);
      std::size_t getStillAliveClientNumber();

      std::thread _ioThread;
      asio::ip::udp::socket _socket;
      asio::ip::udp::endpoint _remoteEndpoint;
      std::array<char, 50000> _receiveBuffer;

      std::size_t _idClient;

      asio::thread_pool &_threadPool;

      std::map<std::size_t, asio::ip::udp::endpoint> _connectedClients;
      std::unordered_map<std::size_t, bool> _areClientsWorldSet;

      std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> _lastAliveClient;
      std::unordered_map<std::size_t, bool> _isClientStillHere;
      std::unordered_map<std::size_t, std::size_t> _corresOwnPlayerId;

      DataToSend _data;
      bool _dataHasBeenUsed;

      const uint16_t _magicNumber;
      uint16_t _receivedMagicNbr;
      uint8_t _messageType;
      uint8_t _messageId;
      uint16_t _messageLen;
  };
};

#endif
