#pragma once

#include "UdpSession.hpp"
#include <memory>
#include <string>

namespace Network
{
	class Factory
	{
	public:
		static std::unique_ptr<NetworkServer::IServer> CreateServer(unsigned short port);
	};
}
