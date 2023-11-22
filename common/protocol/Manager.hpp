/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-Babel-killian.fleury
** File description:
** Protocol.hpp
*/
#pragma once

#include <memory>
#include <cstring>
#include "IProtocol.hpp"
#include "Data.hpp"
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

    #include <sys/types.h>
    #include <netinet/in.h>

    #define HTONL(x) (htonl(x))
    #define NTOHL(x) (ntohl(x))

    #define HTONLL(x) ((1==htonl(1)) ? (x) : (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
    #define NTOHLL(x) ((1==ntohl(1)) ? (x) : (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))
#endif

#define DIGIT_SIZE sizeof(Babel::Network::Digit),
#define SMALL_PACKET sizeof(Babel::Network::Digit) * 2

#define CODE(x) (Protocol::CodeArray[x].typeHeader)
#define REQ_CLI (Protocol::UnpackData::RequestClient)
#define RES_CLI (Protocol::UnpackData::ResponseClient)


namespace Network::Protocol {
    struct CodeData {
        Digit typeHeader;
        std::string msg;
    };

    static const int NbrOfStatusCode = 25;

    static const std::array<CodeData, NbrOfStatusCode> CodeArray = {{
        {100, "You are connected to the server"},
        {101, "GET: <isLogin()>"}, {102, "You must be login"},
        {103, "Error with status code: <\"nbr_status_code\">"},
        {200, "POST(Login): <username: '', password: ''>"},
        {201, "POST(Logout):"},
        {202, "Welcome <'username'> to Babel !"},
        {203, "POST(Register): <username: '', password: '', ...>"},
        {204, "Can't register: <\"reason\">"},
        {205, "Your credentials are good. Welcome back to Babel"},
        {206, "Your credentials are bad, Please try again"},
        {300, "POST(StartCall): <ip: '', udp_port: '', username: ''>"},
        {301, "POST(callAccepted): <ip: '', udp_port: ''>"},
        {302, "POST(callRefused): "},
        {303, "The call is beginning"},
        {304, "No answer"},
        {305, "POST(callFinished): "},
        {306, "The call is finish"},
        {400, "POST(addContact): <username: ''>"},
        {401, "<'username'> has been added to your contact"},
        {402, "Can't add <'username'> to your contact because <\"reason\">"},
        {403, "POST(delContact): <username: ''>"},
        {404, "<'username'> has been deleted from your contact"},
        {405, "GET: <getListContact()>"},
        {406, "This is the list of your contact: <list_contact>"}
        }};

    enum MaxVariableLength : Digit {
        UsernameMaxLength = 20,
        PasswordMaxLength = 20,
        IpMaxLength = 17,
        DataMaxLength = 140
    };

    struct NetworkPacket : public Packet {
        explicit NetworkPacket() : Packet(), buffer{0}
        {
        }

        NetworkPacket(const NetworkPacket &copy) : Packet(copy), buffer{0}
        {
            sizeOfPacket = copy.sizeOfPacket;
            typeHeader = copy.typeHeader;
            std::memcpy(buffer, copy.buffer, sizeOfPacket);
        }

        unsigned char buffer[BuffSize];
    };

    struct UnpackData {
        explicit UnpackData() : typeHeader(0), lobbyId(0), maxPlayer(0), lobbyPort(0), clientRole(0), nbPlayer(0), lobbysInformation(0) {
        }
        //header
        Digit typeHeader;
        Digit lobbyId;
        Digit maxPlayer;
        Digit lobbyPort;
        Digit clientRole;
        Digit nbPlayer;
        Digit spaceShipId;
        Digit* lobbysInformation;
    };

    struct UnpackDataUdp {
        explicit UnpackDataUdp() : typeHeader(0), lenList(0), list(0) {
            //list = new std::size_t[4];
            //std::memset(list, 0, (sizeof(std::size_t) * 4));
        }
        Digit typeHeader;
        Digit lenList;
        Digit *list;
    };

    enum CodeTcp : Digit {
        //Req = request
        //Res = response
        //Not = notification
        ResConnectionEstablished = 200,
        ReqDisconnect,
        ReqDisconnectBySignal,
        ResDisconnected,
        ReqConnectToLobby,
        ResConnectedToLobby,
        NotifyConnectedToLobby,
        ResErrorConnectionToLobby,
        ReqDisconnectFromLobby,
        ResDisconnectedFromLobby,
        NotifyDisconnectedFromLobby,
        ReqCreateLobby,
        ResLobbyCreated,
        NotifyLobbyCreated,
        NotifyLobbyDeleted,
        ReqReadyToPlay,
        ReqNotReadyToPlay,
        ResReadyToPlay,
        ResNotReadyToPlay,
        NotifyReadyToPlay,
        NotifyNotReadyToPlay,
        NotifyStartGame,
        ResError,
        ReqSetSkin,
        ResAddYourSpaceship,
        ResAddASpaceship,
        ReqAskForInfo,
        ResAskForInfo
    };
    enum CodeUdp : Digit {
        //Req = request
        //Res = response
        //Not = notification
        MovePlayer,
        SpawnEntity,
        GameEvent
    };

    struct Manager : public IProtocol {
        Manager() = default;

        ~Manager() override = default;

        void pack(const char *format, ...) final;
        void clear() final;
        static Digit unPack(Buffer buffer, const char *format, ...);


        NetworkPacket p;
//        Digit TypeHeader;
//        unsigned char buffer[BuffSize];
//        Digit sizeOfPacket;
    };
}
