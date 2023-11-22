#pragma once
#include <string>
#include "Manager.hpp"

namespace NetworkClient
{
	class IClient
	{
	public:
		/**
		 * @brief Destroy the IClient object
		 *
		 */
		virtual ~IClient() = default;
		/**
		 * @brief chack if the queu got any message
		 *
		 * @return true
		 * @return false
		 */
		virtual bool HasMessages() = 0;
		/**
		 * @brief send message to a seveur
		 *
		 */
		virtual void Send(Network::Protocol::Packet &) = 0;
		/**
		 * @brief pop message from the queue
		 *
		 * @return unsigned char*
		 */
		virtual unsigned char *PopMessage() = 0;
	};
}
