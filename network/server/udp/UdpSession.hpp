/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-Rtype-Maxime.Dodin
** File description:
** TcpServer.hpp
*/

#pragma once

#include "IServer.hpp"
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>
#include <iostream>
#include <thread>
#include <deque>
#include <map>

#include "Data.hpp"
#include "ThreadPair.hpp"
#include "Manager.hpp"
#include "Registry.hpp"

namespace NetworkServer {
        struct ClientData {
            Network::EndpointStatus _endPointStatus;
            boost::asio::ip::udp::endpoint _remote_endpoint;
            Network::UdpSessionRole _role;
            bool isActive;
            bool isReadyToPlay;
            std::size_t shipUuid;
        };

        class UdpSession: public NetworkServer::IServer, std::enable_shared_from_this<NetworkServer::UdpSession> {
        public:

            // SERVER UDP
            /**
             * @brief Construct a new Udp Session object
             * 
             * @param short 
             */
            explicit UdpSession(unsigned short, uint32_t, Network::PairQueue<Network::Digit, unsigned char *> &, std::map<uint32_t, NetworkServer::ClientData> &);
            ~UdpSession() override;
            /**
             * @brief Stop udp session and join all thread
             * 
             */
            void Stop();
            /**
             * @brief send to client specify
             * 
             */
            void SendTo(Network::Protocol::Packet &, uint32_t) override;
            /**
             * @brief Send to all player
             * 
             */
            void SendToAll(Network::Protocol::Packet &) override;
            /**
             * @brief Send to all player except
             * 
             */
            void SendToAllExcept(Network::Protocol::Packet &, uint32_t) override;

            // ECS

            //MSG handling
/*            Network::Protocol::CodeUdp checkTypeMsg(std::pair<size_t, unsigned char *> msgPair);
            void unpackMsgs(Network::Protocol::CodeUdp typeHeader, std::pair<size_t, unsigned char *> msgPair, Network::Protocol::UnpackDataUdp &dataUdp);
            void sendResponses(size_t idClient, Network::Protocol::CodeUdp typeHeader, Network::Protocol::UnpackDataUdp &dataUdp);
            void sendNewPositions(Network::Digit posX, Network::Digit posY, size_t entityId, uint32_t idClient);
            void sendNewEntity(Network::Digit posX, Network::Digit posY, size_t entityId, uint32_t idClient);
            Network::PairQueue<Network::Digit, unsigned char *> _safeQueue;//private*/
            //...
        private:
            void Run() override;
            uint32_t AssignClient(const boost::asio::ip::udp::endpoint &);
            void StartReceive() override;
            void Send(Network::Protocol::Packet &, const boost::asio::ip::udp::endpoint&);
            void HandleWrite(boost::system::error_code &) const;
            void HandleClientError([[maybe_unused]]const std::error_code &, const boost::asio::ip::udp::endpoint &);
            void HandleReceive(const std::error_code &, [[maybe_unused]] std::size_t) override;

            boost::asio::io_service _io_service;
            boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work
                {_io_service.get_executor()};
            boost::asio::ip::udp::socket _socket;
            boost::asio::ip::udp::endpoint remote_endpoint;
            unsigned char _buff[Network::BuffSize] = {};
            std::thread _threadGroup;
//            boost::thread_group _threadGroup;
            uint32_t _sessionId;
            Network::PairQueue<Network::Digit, unsigned char *> &_safeQueue;
            std::map<uint32_t, NetworkServer::ClientData> &_clientList;
            bool _isAlive = true;
        //            boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work {_io_service.get_executor()}; // TODO : Commenter cette ligne quand l'udp est remis dans le core
        };
    }
