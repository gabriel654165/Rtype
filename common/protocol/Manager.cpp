/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-Babel-killian.fleury
** File description:
** Protocol.cpp
*/
#include <cstring>
#include "Manager.hpp"
#include <ostream>
#include <iostream>
#include <cstdarg>

void Network::Protocol::Manager::pack(const char *format, ...)
{
    Digit integer;
    Digit lenData;
    std::va_list args;

    clear();
    p.sizeOfPacket = sizeof(Digit);
    va_start(args, format);
    for (int i = 0; format && format[i]; ++i) {
        if (format[i] == 'd') {
            integer = va_arg(args, Digit);
            if (i == 0)
                p.typeHeader = integer;
            integer = HTONLL(integer);
            std::memcpy(p.buffer + p.sizeOfPacket, &integer, sizeof(Digit));
            p.sizeOfPacket += sizeof(Digit);
        } else if (format[i] == 'l') {//save d'une boucle a l'autre
            lenData = va_arg(args, Digit);
            Digit _lenData = HTONLL(lenData);
            std::memcpy(p.buffer + p.sizeOfPacket, &_lenData, sizeof(Digit));
            p.sizeOfPacket += sizeof(Digit);
        } else if (format[i] == 'u') {//use l to know the size to add
            Digit *data = va_arg(args, Digit *);
            std::memcpy(p.buffer + p.sizeOfPacket, data, lenData);
            p.sizeOfPacket += lenData;
        } else if (format[i] == 's') {
            char *data = va_arg(args, char *);
            std::memcpy(p.buffer + p.sizeOfPacket, data, lenData);
            p.sizeOfPacket += lenData;
        }
    }
    va_end(args);
    Digit _sizeOfPacket = HTONLL(p.sizeOfPacket);
    //std::cout << "Pack size of packet : " << NTOHLL(p.sizeOfPacket) << std::endl;
    std::memcpy(p.buffer, &_sizeOfPacket, sizeof(Digit));
//    std::cout << std::endl;
}

Network::Digit Network::Protocol::Manager::unPack(Buffer rawBuffer, const char *format, ...) {
    std::va_list args;
    static Digit tmpLenData = 0;
    Digit counter = sizeof(Digit);

    va_start(args, format);
    const auto *packSizeOfPacket = reinterpret_cast<const Digit *>(rawBuffer);
    Digit sizeOfPacket = NTOHLL(*packSizeOfPacket);
    for (int i = 0; format && format[i]; ++i) {
        if (format[i] == 'd') {
            const auto *packInteger = reinterpret_cast<const Digit *>(rawBuffer + counter);
            Digit *integer = va_arg(args, Digit *);
            *integer = NTOHLL(*packInteger);
            counter += sizeof(Digit);
        } else if (format[i] == 'l') {
            const auto *packLenData = reinterpret_cast<const Digit *>(rawBuffer + counter);
            Digit *lenData = va_arg(args, Digit *);
            *lenData = NTOHLL(*packLenData);
            tmpLenData = NTOHLL(*packLenData);
            counter += sizeof(Digit);
        } else if (format[i] == 'u') {
            Digit *str = va_arg(args, Digit *);
            std::memset(str, 0, tmpLenData);
            std::memcpy(str, rawBuffer + counter, tmpLenData);
            counter += tmpLenData;
            tmpLenData = 0;
        } else if (format[i] == 's') {
            char *str = va_arg(args, char *);
            std::memset(str, 0, tmpLenData + 1);
            std::memcpy(str, rawBuffer + counter, tmpLenData);
            counter += tmpLenData;
            tmpLenData = 0;
        }
    }
    va_end(args);
    return (sizeOfPacket);
}


void Network::Protocol::Manager::clear()
{
    std::memset(p.buffer, 0, BuffSize);
    p.sizeOfPacket = 0;
    p.typeHeader = 0;
}