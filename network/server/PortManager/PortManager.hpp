//
// Created by thomas on 09/11/2021.
//

#ifndef B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_PORTMANAGER_HPP
#define B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_PORTMANAGER_HPP

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/placeholders.hpp>
#include "Manager.hpp"

namespace NetworkServer {
    class PortManager {
    public:
        PortManager(unsigned short port, Network::Digit);
        /**
         * @brief Get the Valid Socket object
         * 
         * @return unsigned short 
         */
        unsigned short GetValidSocket();
    private:
        unsigned short _begPort;
        std::vector<std::pair<bool, unsigned short>> _portList;
        Network::Digit _nbPort;
    };
}


#endif //B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_PORTMANAGER_HPP
