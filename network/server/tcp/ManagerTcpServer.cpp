//
// Created by thomas on 11/11/2021.
//

#include "ManagerTcpServer.hpp"

NetworkServer::ManagerTcpServer::ManagerTcpServer() : _worker(nullptr) {
}

void NetworkServer::ManagerTcpServer::Init() {
    _worker = std::make_unique<boost::asio::io_context::work>(_ioContext);
    _threadContext = std::thread([this]() {
        _ioContext.run();
    });
}

void NetworkServer::ManagerTcpServer::Stop() {
    _worker.reset();
    _threadContext.join();
}

std::shared_ptr<NetworkServer::TcpServer> NetworkServer::ManagerTcpServer::CreateTcpServer(Network::PairQueue<Network::Digit, unsigned char *> &messageQueue) {
    return (std::make_unique<TcpServer>(_ioContext, messageQueue));
}
/*
std::shared_ptr<NetworkServer::TcpConnection> NetworkServer::ManagerTcpServer::CreateTcpConnection() {
    return (std::make_unique<TcpConnection>(_ioContext));
}*/