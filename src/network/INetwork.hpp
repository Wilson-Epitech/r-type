/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** INetwork
*/

#ifndef INETWORK_HPP_
#define INETWORK_HPP_

#include "HandleData.hpp"
#include <array>
#include <asio.hpp>

namespace network {
    class INetwork {
      public:
        INetwork() = default;
        ~INetwork() = default;

        virtual sig_atomic_t handleNetwork(const sig_atomic_t &new_status) = 0;
        virtual void sendMessage(const std::string &message, uint8_t messageType) = 0;
        virtual void sendUpdatedData(ecs::Registry &reg, std::list<DataToSend> &lastDataReceivedList) = 0;
        virtual void clearData() = 0;
        virtual std::list<DataToSend> getData() = 0;
        virtual bool isConnectedInNetwork() = 0;

        virtual std::size_t getPlayerId() = 0;
        virtual std::size_t checkConnectedHeartbeat() = 0;
    };
}

#endif /* !INETWORK_HPP_ */
