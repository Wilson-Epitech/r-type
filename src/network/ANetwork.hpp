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

    class ANetwork : virtual public network::INetwork {
      public:
        ANetwork() : _isRunning(true), _magicNumber(0x9B3D), _messageId(0) {};
        ~ANetwork() = default;

        sig_atomic_t handleNetwork(const sig_atomic_t &new_status) {
            static volatile sig_atomic_t status = 1;

            if (new_status != 1)
                status = new_status;
            return status;
        }

      protected:
          std::list<DataToSend> _data; ///< A list of Data received such as game state updates.
          bool _isRunning;             ///< Indicates whether the Network instance is still running.

          const uint16_t _magicNumber; ///< Magic number used for message validation.
          uint8_t _messageId;          ///< ID of the received message.
          uint16_t _messageLen;        ///< Length of the received message.
    };
}

#endif /* !ANETWORK_HPP_ */
