//
// Created by thomas on 11/11/2021.
//

#pragma once
#include "TcpServer.hpp"

namespace NetworkServer {
    class ManagerTcpServer {
    public:
        /**
         * @brief Construct a new Manager Tcp Server object
         * 
         */
        ManagerTcpServer();
        /**
         * @brief Stop Tcp session, reset worker and create thread to run io.run()
         * 
         */
        void Stop();
        /**
         * @brief reset worker and join thread
         * 
         */
        void Init();
        /**
         * @brief Create a Tcp Server object
         * 
         * @return std::shared_ptr<NetworkServer::TcpServer> 
         */
        std::shared_ptr<NetworkServer::TcpServer> CreateTcpServer(Network::PairQueue<Network::Digit, unsigned char *>&);
        //std::shared_ptr<NetworkServer::TcpConnection> CreateTcpConnection();
//        std::vector<std::unique_ptr<IUdpClient>> _udpCli;

    private:
       // std::vector<std::shared_ptr<TcpServer>> _tcpCli;
        /* Multi threading variables */
        std::shared_ptr<boost::asio::io_context::work> _worker;
        boost::asio::io_context _ioContext;
        std::thread _threadContext;
    };
}