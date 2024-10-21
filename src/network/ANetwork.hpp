/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ANetwork
*/

#ifndef ANETWORK_HPP_
#define ANETWORK_HPP_

#include "INetwork.hpp"

namespace network {
    /**
     * @class ANetwork
     * @brief Abstract class for all network instances, such as server and client.
     * The class purpose is to define basic variables needed to send and receive data,
     * as well as basic implementation of status set used in all network instances.
     */

    class ANetwork : virtual public network::INetwork {
      public:
        ANetwork() : _isRunning(true), _magicNumber(0x9B3D), _messageId(0) {};
        ~ANetwork() = default;

        sig_atomic_t handleNetwork(const sig_atomic_t &newStatus) {
            static volatile sig_atomic_t status = 1;

            if (newStatus != 1)
                status = newStatus;
            return status;
        }

      protected:
          bool _isRunning;             ///< Indicates whether the Network instance is still running.
          std::list<DataToSend> _data; ///< A list of Data received such as game state updates.

          const uint16_t _magicNumber; ///< Magic number used for message validation.
          uint8_t _messageId;          ///< ID of the received message.
          uint16_t _messageLen;        ///< Length of the received message.
    };
}

#endif /* !ANETWORK_HPP_ */
