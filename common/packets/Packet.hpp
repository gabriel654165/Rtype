/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-Babel-killian.fleury
** File description:
** Packet.hpp
*/
#pragma once

#include <iostream>
#include <cstring>
#include "Data.hpp"

namespace Network::Protocol {
    struct Packet {
        Packet();
        ~Packet() = default;

        Packet(const Packet &copy)
        {
            sizeOfPacket = copy.sizeOfPacket;
            typeHeader = copy.typeHeader;
        }
        Digit sizeOfPacket;
        Digit typeHeader;

    };
}
