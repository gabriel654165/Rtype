//
// Created by thomas on 09/11/2021.
//

#include "PortManager.hpp"
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio.hpp>

NetworkServer::PortManager::PortManager(unsigned short beginPort, Network::Digit nbPort) : _begPort(beginPort), _nbPort(nbPort) {
    _portList.reserve(_nbPort);
    bool isUse = false;
    for (Network::Digit i = 0; i < _nbPort; i++) {
        _portList.emplace_back(isUse, beginPort++);
    }
}

unsigned short NetworkServer::PortManager::GetValidSocket() {
    for (auto &it : _portList) {
        if (!it.first) {
            it.first = true;
            return it.second;
        }
    }
    std::cout << std::endl;
    return 0;
}