//
// Created by thomas on 10/11/2021.
//
#include <cstring>
#include <ostream>
#include <iostream>
#include <cstdarg>
#include <vector>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#include <ws2tcpip.h>
    #include <winsock2.h>

    #define HTONL(x) (htonl(x))
    #define NTOHL(x) (ntohl(x))

    #define HTONLL(x) (htonll(x))
    #define NTOHLL(x) (ntohll(x))
#else

#include <netinet/in.h>

#define HTONL(x) (htonl(x))
#define NTOHL(x) (ntohl(x))

#define HTONLL(x) ((1==htonl(1)) ? (x) : (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
#define NTOHLL(x) ((1==ntohl(1)) ? (x) : (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))
#endif

unsigned char *packTcp(const char *format, ...)
{
    size_t integer;
    std::size_t lenData;
    std::va_list args;

    size_t sizeOfPacket;
    size_t statusCode;
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * 4096);

    //clear();
    sizeOfPacket = sizeof(size_t);
    va_start(args, format);
    for (int i = 0; format && format[i]; ++i) {
        if (format[i] == 'd') {
            integer = va_arg(args, size_t);
            if (i == 0)
                statusCode = integer;
            integer = HTONLL(integer);
            std::memcpy(buffer + sizeOfPacket, &integer, sizeof(size_t));
            sizeOfPacket += sizeof(size_t);
        } else if (format[i] == 'l') {
            lenData = va_arg(args, size_t);
            size_t _lenData = HTONLL(lenData);
            std::memcpy(buffer + sizeOfPacket, &_lenData, sizeof(size_t));
            sizeOfPacket += sizeof(size_t);
        } else if (format[i] == 's') {
            char *data = va_arg(args, char *);
            std::memcpy(buffer + sizeOfPacket, data, lenData);
            sizeOfPacket += lenData;
        }
    }
    va_end(args);
    size_t _sizeOfPacket = HTONLL(sizeOfPacket);
    std::memcpy(buffer, &_sizeOfPacket, sizeof(size_t));
    //std::cout << "type : " << statusCode << std::endl;
    //std::cout << "sizeMsg : " << sizeOfPacket << std::endl;
    return buffer;
}

size_t unpackTcp(unsigned char *rawBuffer, const char *format, ...)
{
    std::va_list args;
    static size_t tmpLenData = 0;
    size_t counter = sizeof(size_t);

    va_start(args, format);
    const auto *packSizeOfPacket = reinterpret_cast<const size_t *>(rawBuffer);
    size_t sizeOfPacket = NTOHLL(*packSizeOfPacket);
    for (int i = 0; format && format[i]; ++i) {
        if (format[i] == 'd') {
            const auto *packInteger = reinterpret_cast<const size_t *>(rawBuffer + counter);
            size_t *integer = va_arg(args, size_t *);
            *integer = NTOHLL(*packInteger);
            counter += sizeof(size_t);
        } else if (format[i] == 'l') {
            const auto *packLenData = reinterpret_cast<const size_t *>(rawBuffer + counter);
            size_t *lenData = va_arg(args, size_t *);
            *lenData = NTOHLL(*packLenData);
            tmpLenData = NTOHLL(*packLenData);
            counter += sizeof(size_t);
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

int main()
{
    //PROTOCOL : (variables in class)
    //header
    size_t sizeMsg;
    size_t type;
    //body
    std::vector<std::size_t> lol;
    size_t entityIdx;

    //unpack juste le type
    //IF (type : MovePos)
    for (int i = 0;i != 2; ++i) {
        lol.push_back(0);
    }

    unsigned char *resEncoded = packTcp("lllll", sizeof(size_t) * 5, 1, 201, 202, 1);

    size_t sizeMsgDecoded = unpackTcp(resEncoded, "lllll", &sizeMsg, &type, &lol.at(0), &lol.at(1), &entityIdx);
    std::cout << "Result : ";
    std::cout << "\tsizeMsgDecoded = " << sizeMsgDecoded << std::endl;
    std::cout << "\t\tsizeMsg = " << sizeMsg << std::endl;
    std::cout << "\t\ttype =" << type << std::endl;
    std::cout << "\t\tposX =" << lol[0] << std::endl;
    std::cout << "\t\tposY =" << lol[1] << std::endl;
    std::cout << "\t\tentityIdx =" << entityIdx << std::endl << std::endl;

    return(0);
}