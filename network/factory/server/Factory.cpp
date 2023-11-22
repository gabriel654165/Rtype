#include "Factory.hpp"

namespace Network {
	std::unique_ptr<NetworkServer::IServer> Factory::CreateServer(unsigned short port)
	{
	    auto server = new NetworkServer::UdpSession(port);
	    return std::unique_ptr<NetworkServer::IServer>(server);
	}
}
