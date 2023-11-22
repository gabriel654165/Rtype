#include "UdpClient.hpp"
#include "Log.hpp"
#include <iostream>

namespace NetworkClient::Udp
{
	NetworkClient::Udp::UdpClient::UdpClient(unsigned short server_port, Network::LockedQueue<unsigned char *> &udpSafeQueue, std::string &host)
	: _isStopped(false), _udpSafeQueue(udpSafeQueue), socket(io_service, udp::endpoint(udp::v4(), 0)), service_thread(&NetworkClient::Udp::UdpClient::RunService, this), _remoteEndpoint()
	{
        std::cout << "UDP PORT ====== " << server_port << std::endl;
		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), host, std::to_string(server_port));
		server_endpoint = *resolver.resolve(query);
		start_receive();
	}

	UdpClient::~UdpClient()
	{
        std::cout << "JE PASSES DANS LE DESTRUCTEUR D'UDP CLIENT" << std::endl;
		if (!_isStopped) {
            io_service.stop();
            service_thread.join();
            socket.close();
        }
	}

	void UdpClient::Stop()
	{
		io_service.stop();
		service_thread.join();
        socket.close();
		_isStopped = true;
	}

	void UdpClient::start_receive()
	{
//        std::cout << "LE CLIENT UDP COMMENCE A RECEVOIR" << std::endl;
		const boost::system::error_code err;
        size_t byteTransferred;

		socket.async_receive_from(boost::asio::buffer(_buff), server_endpoint,
								  std::bind(&UdpClient::handle_receive, this, err, byteTransferred));
	}

	void UdpClient::handle_receive(const std::error_code &error, [[maybe_unused]]std::size_t bytes_transferred)
	{
		if (_isStopped)
			return;
		if (!error)
            //attendre la safequeue de max pour unpack le msg et envoyer a l'ecs
		{
			//Network::Digit sizeOfPacket = 0;
            //std::cout << "Client receive a packet of size " << bytes_transferred << std::endl;
			_udpSafeQueue.push(_buff);
		}
		else
		{
			Log::Error("UdpClient::handle_receive:", error);
		}
		start_receive();
	}

	void UdpClient::Send(Network::Protocol::Packet &packet)
	{
		socket.send_to(boost::asio::buffer(reinterpret_cast<Network::Protocol::NetworkPacket &>(packet).buffer), server_endpoint);
		/*
		auto packetCast = reinterpret_cast<Network::Protocol::NetworkPacket &>(packet);
		socket.send_to(boost::asio::buffer(packetCast.buffer,packetCast.sizeOfPacket), server_endpoint);
		*/
	}

	void UdpClient::SendToEndPt(Network::Protocol::Packet &packet, const udp::endpoint& serverEndpointArg)
	{
		socket.send_to(boost::asio::buffer(reinterpret_cast<Network::Protocol::NetworkPacket &>(packet).buffer, reinterpret_cast<Network::Protocol::NetworkPacket &>(packet).sizeOfPacket), serverEndpointArg);
	}

	bool UdpClient::HasMessages()
	{
		return !_udpSafeQueue.empty();
	}

	unsigned char *UdpClient::PopMessage()
	{
		if (_udpSafeQueue.empty())
			throw std::logic_error("No messages to pop");
	//	return incomingMessages.pop();
        return 0;
	}

	void UdpClient::RunService()
	{
		while (!io_service.stopped())
		{
			try
			{
				io_service.run();
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			} catch (const std::exception &e)
			{
				Log::Warning("UdpClient: network exception: ", e.what());
			}
			catch (...)
			{
				Log::Error("Unknown exception in UdpClient network thread");
			}
		}
	}

    void UdpClient::SetEndpoint(std::string &endpoint) {
        std::cout << "Bonjour" << std::endl;
        _remoteEndpoint = endpoint;
        std::cout << "Aurevoir" << std::endl;
    }
}

