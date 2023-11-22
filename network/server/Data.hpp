/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-Babel-killian.fleury
** File description:
** Data.hpp
*/

#pragma once

#include <iostream>

namespace Network {
    enum CallBack {
        CallbackTimer = 100
    };

    using Buffer = unsigned char *;
    using Digit = std::size_t;

    enum EndpointStatus {
        FULFILL,
        NONE
    };

    enum UdpSessionMode {
        LOBBY,
        GAME,
        CLOSE,
        ERROR_MODE
    };

    enum UdpSessionRole : Digit {
        ADMIN,
        GUEST,
        PASSIVE_GUEST,
        ERROR_ROLE
    };

    enum NetworkInfo {
        Timeout = 5000,
        NothingToRead = -1
    };

    enum MainParameter {
        UdpPort = 5555,
        IP = 1,
        PORT = 2
    };

    enum ProtocolInfo {
        EmptyPacket = 0,
        BuffSize = 4096,
    };

    struct HInfo {
        std::string ip;
        int port;
    };
}

