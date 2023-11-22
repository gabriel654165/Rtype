#pragma once

#include "History.hpp"
#include "SafeQueue.hpp"
#include "IClient.hpp"
#include "Data.hpp"
#include "Manager.hpp"
#include "signalHandler.hpp"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <array>
#include <thread>


using boost::asio::ip::tcp;
static const std::size_t NetworkBufferSizeTcp = 4096;

namespace NetworkClient::Tcp{
    class TcpClient : public NetworkClient::IClient, std::enable_shared_from_this<NetworkClient::Tcp::TcpClient>  {
    public:
        TcpClient(std::string host, unsigned short server_port, Network::LockedQueue<unsigned char *> &tcpSafeQueue);
        ~TcpClient() override;

        void Send(Network::Protocol::Packet &) override;

        bool HasMessages() override;

        unsigned char *PopMessage() override;
        void Start();

        std::pair<bool, std::size_t> getInLobby();
        void setInLobby(const bool inLobby, const std::size_t lobbyId);
        void Stop();
        bool IsStopped() const;
    private:

        // TcpClient send/receive stuff
        boost::asio::io_context _io_context;
        tcp::endpoint _endpoint;
        tcp::resolver::results_type _endpoints;
        tcp::socket _socket;
        std::thread _service_thread;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work {_io_context.get_executor()};
        std::string _input_buffer;

        boost::asio::steady_timer _deadline;
        boost::asio::steady_timer _heartbeat_timer;
        bool _stopped;
        unsigned char _buff[Network::BuffSize]{};
        unsigned short _server_port;

        // Queues for messages
        Network::LockedQueue<unsigned char *> &_tcpSafeQueue;

        std::pair<bool, std::size_t> _inLobby = std::make_pair(false, 0);

        void HandleConnect(const boost::system::error_code& ec, tcp::resolver::results_type::iterator endpoint_iter);
        void StartReceive();
        void HandleReceive(const std::error_code& error, std::size_t bytes_transferred);

        void HandleWrite(const std::error_code &error, std::size_t bytes_transferred);

        void RunService();
        void CheckDeadline();
        void StartConnect(tcp::resolver::results_type::iterator endpoint_iter);

        // Statistics
        Network::Historys history;
    };
}