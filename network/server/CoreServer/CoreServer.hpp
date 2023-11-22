//
// Created by thomas on 07/11/2021.
//

#pragma once

#include "UdpServer.hpp"
#include "ManagerTcpServer.hpp"
#include "MyClock.hpp"

namespace NetworkServer {
    class CoreServer {
    public:
        /**
         * @brief Construct a new Core Server object
         * 
         * @param port 
         */
        explicit CoreServer(unsigned short port);
        ~CoreServer() = default;
        /**
         * @brief Use to begin tcp thread for callback 
         * 
         */
        void Run();

        std::unique_ptr<NetworkServer::UdpServer> _udpServer;

    private:

        /////////////////// CALLBACK ///////////////////

        void _reqConnection([[maybe_unused]]Network::Digit, [[maybe_unused]]Network::Buffer);
        void _reqDisconnectionBySignal(Network::Digit, [[maybe_unused]]Network::Buffer);
        void _reqDisconnection(Network::Digit, [[maybe_unused]]Network::Buffer);
        void _reqConnectLobby(Network::Digit, Network::Buffer);
        void _reqDisconnectLobby(Network::Digit, Network::Buffer);
        void _reqCreateLobby(Network::Digit, Network::Buffer);
        void _reqReadyToPlay(Network::Digit, Network::Buffer);
        void _reqNotReadyToPlay(Network::Digit, Network::Buffer);
        void _reqAskForInfo(Network::Digit, [[maybe_unused]]Network::Buffer);
        ////////////////////////////////////////////////

        void TcpCallback();
        void ChronoManagement();
        [[noreturn]] void TcpUpdate();

        std::unique_ptr<NetworkServer::ManagerTcpServer> _tcpManager;
        std::map<uint32_t, std::unique_ptr<MyClock>> _chronoList;
        std::shared_ptr<NetworkServer::TcpServer> _tcpServer;
        Network::PairQueue<Network::Digit, unsigned char *> _tcpSafeQueue;
    };
}