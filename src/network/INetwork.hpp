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
    /**
     * @class INetwork
     * @brief Interface for all network instances, such as server and client.
     * The class purpose is to handle network communications.
     * It provides methods for sending messages, updating data, and managing network connection.
     */

    /**
     * @enum EntityType
     * @brief Enum representing different types of network instances.
     */
    enum Type {
        NONE,   ///< No type instance
        SERVER, ///< Server instance
        CLIENT  ///< Client instance
    };

    class INetwork {
      public:
        INetwork() = default;
        virtual ~INetwork() = default;

        /**
         * @brief Sends a message to the network instance.
         * @param message The message to send.
         * @param messageType The type of the message, which is used for server-side processing.
         */
        virtual void sendMessage(const std::string &message, uint8_t messageType) = 0;

        /**
         * @brief Sends updates to the other-side network instance in order to process informations.
         * @param reg The ECS registry managing entities and components.
         * @param lastDataReceivedList The last data received from the other-side network instance, used to check what new informations can
         * be send.
         */
        virtual void sendUpdatedData(ecs::Registry &reg, std::list<DataToSend> &lastDataReceivedList) = 0;

        /**
         * @brief Confirm connection to the server client-side, and check connection of the clients server-side.
         * @return std::size_t The player ID corresponding to the client that has disconnect
         * @return std::size_t 0 If no client has disconnect
         */
        virtual std::size_t manageConnection() = 0;

        /**
         * @brief Updates the ECS registry based on the last data received by the other-side network instance
         * @param reg The ECS registry managing entities and components.
         */
        virtual void updateData(ecs::Registry &reg) = 0;

        /**
         * @brief Retrieves the data received from the other-side network instance.
         * @return std::list<DataToSend> The data received.
         */
        virtual std::list<DataToSend> getData() = 0;

        /**
         * @brief Retrieves the type of the network instance
         * @return network::Type The type of the network instance
         */
        virtual network::Type getNetworkType() = 0;

        /**
         * @brief Checks if the network instance is still connected.
         * @return true If the network instance is connected.
         * @return false If the network instance isn't.
         */
        virtual bool isConnectedInNetwork() = 0;

        /**
         * @brief Used to check if SIGINT signal has been catch.
         * @return true If SIGINT has been catch.
         * @return false Otherwise.
         */
        virtual bool isSigInt() = 0;
    };
}

#endif /* !INETWORK_HPP_ */
