//
// Created by thomas on 06/11/2021.
//
#pragma once

#include <string>
#include <system_error>
#include "Packet.hpp"

namespace NetworkServer {
        class IServer {
        public:
            virtual ~IServer() = default;
            virtual void SendTo(Network::Protocol::Packet &packet, uint32_t) = 0;
            virtual void SendToAll(Network::Protocol::Packet &packet) = 0;
            virtual void SendToAllExcept(Network::Protocol::Packet &packet, uint32_t) = 0;
        private:
            virtual void Run() = 0;
            virtual void StartReceive() = 0;
            virtual void HandleReceive([[maybe_unused]]const std::error_code &, [[maybe_unused]] std::size_t) = 0;
        };
    }
