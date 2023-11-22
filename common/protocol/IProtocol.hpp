/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-killian.fleury
** File description:
** IProtocol.hpp
*/
#pragma once

#include "Packet.hpp"

namespace Network::Protocol {
    class IProtocol {
        public:
        virtual ~IProtocol() = default;

        virtual void pack(const char *format, ...) = 0;

        virtual void clear() = 0;
    };
}
