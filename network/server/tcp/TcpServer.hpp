//
// Created by thomas on 08/11/2021.
//

#pragma once

#include "IServer.hpp"
#include "TcpConnection.hpp"
#include <thread>
#include <map>
#include <deque>

namespace NetworkServer {
    class TcpServer : public NetworkServer::IServer {
    public:
        explicit TcpServer(boost::asio::io_context &, Network::PairQueue<Network::Digit, unsigned char *> &);
        ~TcpServer() override = default;

        /**
         * @brief bind the socket, listen to handle new connection on server
         * 
         */
        void Start(Network::Digit);
        /**
         * @brief close the acceptor
         * 
         */
        void Stop();
        /**
         * @brief Send to client by id
         * 
         */
        void SendTo(Network::Protocol::Packet &, uint32_t) override;
        /**
         * @brief Send to all client
         * 
         */
        void SendToAll(Network::Protocol::Packet &) override;
        /**
         * @brief Send to all client except one
         * 
         */
        void SendToAllExcept(Network::Protocol::Packet &, uint32_t) override;
        /**
         * @brief erase tcp connection on the server
         * 
         */
        void StopTcpConnection(Network::Digit);
        /**
         * @brief return true if find tcp connection
         * 
         * @return true 
         * @return false 
         */
        bool IsTcpSessionAlive(Network::Digit);
        /**
         * @brief (deprecated) send tcp message to all udp session
         * 
         */
        void SendToAllUdpInTcp(Network::Protocol::Packet &, std::vector<Network::Digit> &);

    private:
        void StartReceive() override;
        void HandleAccept(std::shared_ptr<TcpConnection>);
        void HandleReceive(const std::error_code &, [[maybe_unused]] std::size_t) override;
        void Run() override;

        boost::asio::io_context &_io_context;
//        NtwClient::Digit _port;
        std::atomic_int32_t _clientIdInc;
        boost::asio::ip::tcp::acceptor _acceptor;
//        std::thread _threadContext;
        Network::PairQueue<Network::Digit, unsigned char *> &_safeQueue;
        std::deque<std::shared_ptr<NetworkServer::TcpConnection>> _clientConnection;
//        std::unique_ptr<boost::asio::io_context::work> _work;
    };
}