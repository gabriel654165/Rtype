/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-Rtype-Maxime.Dodin
** File description:
** TcpConnection.hpp
*/

#pragma once

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/placeholders.hpp>
#include <map>
#include "ThreadPair.hpp"
#include "Manager.hpp"
#include "Packet.hpp"
#include "Data.hpp"
#include "boost/asio/read.hpp"
#include <boost/asio.hpp>

namespace NetworkServer {
    class TcpConnection : public std::enable_shared_from_this<NetworkServer::TcpConnection> {
    public:
        explicit TcpConnection(uint32_t, boost::asio::io_context &,
            Network::PairQueue<Network::Digit, unsigned char *> &);
        ~TcpConnection();
        /**
         * @brief Start the read on socket for each client (on TCP)
         * 
         */
        void StartRead();
        /**
         * @brief Get the Socket object of the client
         * 
         * @return boost::asio::ip::tcp::socket& 
         */
        boost::asio::ip::tcp::socket &GetSocket();
        /**
         * @brief return true if socket open
         * 
         * @return true 
         * @return false 
         */
        bool IsConnected() const;
        /**
         * @brief Start the write function on socket for each client (on TCP)
         * 
         */
        void StartWrite(Network::Protocol::NetworkPacket &);
        /**
         * @brief Get the Id of the client
         * 
         * @return uint32_t
         */
        uint32_t GetId() const noexcept;
        /**
         * @brief Get the Remote Endpoint of the client
         * 
         * @return std::string 
         */
        std::string GetRemoteEndpoint();
        /**
         * @brief Stop io context and close the socket
         * 
         */
        void Stop();

    private:
        void HandleRead(const boost::system::error_code &, size_t);
        void HandleWrite(const boost::system::error_code &, size_t);

        uint32_t _clientId;
        unsigned char _buff[Network::BuffSize];
        boost::asio::io_context &_io_context;
        boost::asio::ip::tcp::socket _socket;
        Network::PairQueue<Network::Digit, unsigned char *> &_messageQueue;
        std::mutex _readDataClient;
        bool _isOpen;
        boost::asio::streambuf _buffer;
        std::mutex _mutexWrite;
    };
}
