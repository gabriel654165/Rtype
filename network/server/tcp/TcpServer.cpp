//
// Created by thomas on 08/11/2021.
//

#include "TcpServer.hpp"

NetworkServer::TcpServer::TcpServer(boost::asio::io_context &ioContext, Network::PairQueue<Network::Digit, unsigned char *> &safeQueue) : _io_context(ioContext), _clientIdInc(0L),
    _acceptor(ioContext), _safeQueue(safeQueue)
{
}

void NetworkServer::TcpServer::Stop() {
    _acceptor.close();
}

void NetworkServer::TcpServer::StopTcpConnection(Network::Digit clientId) {
    try {
        //std::cout << "size of client list before find erase : " << _clientConnection.size() << std::endl;
        auto client = std::find_if(_clientConnection.begin(), _clientConnection.end(),
                                   [&clientId](const std::shared_ptr<NetworkServer::TcpConnection> &e) {
                                        return (e->GetId() == clientId);
                                   });
        if (client != _clientConnection.end()) {
            std::cout << "Server stop the TCP session of client " << clientId << std::endl;
//            _clientConnection.at(clientId)->Stop();
//            client->get()->Stop();
            _clientConnection.erase(client);
        }
        //std::cout << "size of client list : " << _clientConnection.size() << std::endl;
    } catch (std::exception &error) {
        std::cout << "ERROR : Erase client connection on server -> " << error.what() << std::endl;
    }
/*    for (auto &it : _clientConnection) {
        if (it->GetId() == clientId) {
            if (it->IsConnected()) {
                it->Stop();
            } else
                throw std::range_error("oui");
        }
    }*/
}

void NetworkServer::TcpServer::Start(Network::Digit port)
{
    try {
        _acceptor.open(boost::asio::ip::tcp::v4());
        _acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
        _acceptor.listen();
    } catch (std::exception &error) {
//        throw err::TcpServer(std::string("Failed to open socket: " + LINE));
    }
    StartReceive();
}

void NetworkServer::TcpServer::StartReceive() {
    auto tcpConnection = std::make_shared<TcpConnection>(++_clientIdInc, _io_context, _safeQueue);
    _acceptor.async_accept(tcpConnection->GetSocket(),
        std::bind(&NetworkServer::TcpServer::HandleAccept, this, tcpConnection));
}

void NetworkServer::TcpServer::HandleAccept(std::shared_ptr<TcpConnection> tcpConnection) {
    Network::Protocol::Manager protocol;
    std::cout << "new connection on server" << std::endl;
    tcpConnection->StartRead();
    _clientConnection.emplace_back(tcpConnection);
    std::string remoteEndpoint = _clientConnection.back()->GetRemoteEndpoint();
    protocol.pack("dls", Network::Protocol::CodeTcp::ResConnectionEstablished, remoteEndpoint.size(), remoteEndpoint.c_str());
    SendTo(protocol.p, _clientIdInc);
    std::cout << "server send hello message to client n°" << _clientIdInc << std::endl;
    StartReceive();
}

void NetworkServer::TcpServer::HandleReceive([[maybe_unused]]const std::error_code &error, [[maybe_unused]] std::size_t byte_count) {}

void NetworkServer::TcpServer::SendTo(Network::Protocol::Packet &packet, uint32_t clientId) {
    for (auto &it : _clientConnection) {
        if (it->IsConnected() && it->GetId() == clientId) {
            it->StartWrite(reinterpret_cast<Network::Protocol::NetworkPacket &>(packet));
            return;
        }
    }
}

void NetworkServer::TcpServer::SendToAll(Network::Protocol::Packet &packet) {
    for (auto &it : _clientConnection) {
        if (it->IsConnected()) {
            std::cout << "J'ENVOIE AU CLIENT " << it->GetId() << std::endl;
            it->StartWrite(reinterpret_cast<Network::Protocol::NetworkPacket &>(packet));
        }
    }
}

void NetworkServer::TcpServer::SendToAllExcept(Network::Protocol::Packet &packet, uint32_t clientId) {
    for (auto &it : _clientConnection) {
        if (it->IsConnected() && it->GetId() != clientId) {
            it->StartWrite(reinterpret_cast<Network::Protocol::NetworkPacket &>(packet));
        }
    }
}

void NetworkServer::TcpServer::Run()
{
    std::cout << "tcp : "<< (_io_context.stopped() ? "stopped" : "working") << std::endl;
    while (!_io_context.stopped()) {
        _io_context.run();
    }
    std::cout << "tcp thread server end" << std::endl;
}

bool NetworkServer::TcpServer::IsTcpSessionAlive(Network::Digit tcpSessionId)
{
    auto client = std::find_if(_clientConnection.begin(), _clientConnection.end(),
                               [&tcpSessionId](const std::shared_ptr<NetworkServer::TcpConnection> &e) {
                                   return (e->GetId() == tcpSessionId);
                               });
    return (client != _clientConnection.end());
}

void NetworkServer::TcpServer::SendToAllUdpInTcp(Network::Protocol::Packet &packet, std::vector<Network::Digit> &idList)
{
    std::cout << "LISTE DE MEC DANS LA SESSION UDP = " << idList.size() << std::endl;
    for (auto &it : idList) {
        if (IsTcpSessionAlive(it)) {
            std::cout << "J'ENVOI A TOUT LES MECS DE L'UDP : " << it << std::endl;
           // std::unique_lock<std::mutex> lock(_mutexWrite);
           // lock.lock();
            _clientConnection.at(it)->StartWrite(reinterpret_cast<Network::Protocol::NetworkPacket &>(packet));
        }
    }
}
