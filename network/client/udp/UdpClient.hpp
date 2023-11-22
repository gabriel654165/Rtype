#pragma once

#include "History.hpp"
#include "SafeQueue.hpp"
#include "IClient.hpp"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <array>
#include <thread>


using boost::asio::ip::udp;
static const std::size_t NetworkBufferSizeUdp = 4096;

namespace NetworkClient::Udp {
	class UdpClient : public NetworkClient::IClient {
	public:
		UdpClient(unsigned short server_port, Network::LockedQueue<unsigned char *> &udpSafeQueue, std::string &);
		virtual ~UdpClient();

		void Send(Network::Protocol::Packet &) override;
		void SendToEndPt(Network::Protocol::Packet &, const udp::endpoint& serverEndpointArg);
        void Stop();
        void SetEndpoint(std::string &);

		bool HasMessages() override;

		unsigned char *PopMessage() override;
		bool _isStopped;

		boost::asio::io_service io_service;//private
		udp::endpoint server_endpoint;//private
    private:

		// UdpClient send/receive stuff

        Network::LockedQueue<unsigned char *> &_udpSafeQueue;//private
		udp::socket socket;
		std::thread service_thread;
		boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work {io_service.get_executor()};
		unsigned char _buff[Network::BuffSize]{};

		// Queues for messages
		//Network::LockedQueue<unsigned char *> &_udpSafeQueue;

		void start_receive();
		void handle_receive(const std::error_code& error, [[maybe_unused]]std::size_t bytes_transferred);
		void RunService();
        std::string _remoteEndpoint;
		UdpClient(UdpClient&); // block default copy constructor

		// Statistics
		Network::Historys history;
	};
}
