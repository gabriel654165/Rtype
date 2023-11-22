/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-Rtype-maxime.fleury
** File description:
** TcpServer.cpp
*/
#include <map>
#include "Error.hpp"
#include "UdpSession.hpp"

NetworkServer::UdpSession::UdpSession(unsigned short port, uint32_t sessionId, Network::PairQueue<Network::Digit, unsigned char *> &safeQueue, std::map<uint32_t, NetworkServer::ClientData> &clientList) :
    _socket(_io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
    _safeQueue(safeQueue), _clientList(clientList)
{
    _threadGroup = std::thread([this]() {
        _io_service.run();
    });
//    _threadGroup = std::thread(boost::bind(&NetworkServer::UdpSession::Run, shared_from_this()));
    _sessionId = sessionId;
   StartReceive();
}

NetworkServer::UdpSession::~UdpSession()
{
    if (_isAlive) {
        _work.reset();
        _io_service.stop();
        _threadGroup.join();
        _socket.close();
    }
    std::cout << "[UDP Session " << _sessionId << "] : Get DESTROYED" << std::endl;
//    _io_service.stop();
}

void NetworkServer::UdpSession::Stop()
{
    if (_isAlive) {
        _isAlive = false;
        _work.reset();
        _io_service.stop();
        std::cout << "[UDP Session " << _sessionId << "] : Stop the io_service." << std::endl;
        _threadGroup.join();
        std::cout << "[UDP Session " << _sessionId << "] : Session join all thread." << std::endl;
        _socket.close();
        std::cout << "[UDP Session " << _sessionId << "] : Session Stop." << std::endl;
    }
}

void NetworkServer::UdpSession::HandleClientError([[maybe_unused]]const std::error_code &error_code, const boost::asio::ip::udp::endpoint &client_endpoint) {
    uint32_t temp_id = 0;
    for (const auto &it : _clientList) {
        if (it.second._remote_endpoint == client_endpoint) {
            temp_id = it.first;
            break;
        }
    }
    if (temp_id != 0) {
        std::cout << "[UDP Session " << _sessionId << "] : erase client " << temp_id << " from session." << std::endl;
        _clientList.erase(temp_id);
//        Stop();
    }
}

void NetworkServer::UdpSession::StartReceive() {

    //std::cout << "[UDP Session " << _sessionId << "] : wait for message." << std::endl;

    _socket.async_receive_from(boost::asio::buffer(_buff), remote_endpoint,
                              [this](std::error_code ec, std::size_t bytes_recvd) {
        this->HandleReceive(ec, bytes_recvd);
    });
}

void NetworkServer::UdpSession::HandleReceive([[maybe_unused]] const std::error_code& error, [[maybe_unused]] std::size_t bytes_transferred) {
    uint32_t clientId = AssignClient(remote_endpoint);
//    std::cout << "MON ADDRESSE IP : " << _socket.local_endpoint().address() << std::endl;
    if (!error) {
        _safeQueue.push(clientId, _buff);
       // std::cout << "[UDP Session " << _sessionId << "] receive message from client " << clientId << "." << std::endl;
    } else {
        std::cout << "[UDP Session " << _sessionId << "] : _clientList size = " << _clientList.size() << std::endl;
        HandleClientError(error, _clientList.at(clientId)._remote_endpoint);
    }
    StartReceive();
}


void NetworkServer::UdpSession::Send(Network::Protocol::Packet &packet, const boost::asio::ip::udp::endpoint& endpoint) {
    boost::system::error_code errorCode;
    _socket.async_send_to(boost::asio::buffer(reinterpret_cast<Network::Protocol::NetworkPacket &>(packet).buffer), endpoint,
        std::bind(&NetworkServer::UdpSession::HandleWrite, this, errorCode));
//    _socket.send_to(boost::asio::buffer(reinterpret_cast<NtwClient::Protocol::NetworkPacket &>(packet).buffer), endpoint);
}

void NetworkServer::UdpSession::HandleWrite(boost::system::error_code &error) const
{
    if (!error) {
        //std::cout << "[UDP Session " << _sessionId << "] : Send a message." << std::endl;
    } else {
        std::cout << "[UDP Session " << _sessionId << "] : Error on message receive." << std::endl;
    }
}

void NetworkServer::UdpSession::SendTo(Network::Protocol::Packet &packet, uint32_t id) {
    Send(packet, _clientList.at(id)._remote_endpoint);
}

void NetworkServer::UdpSession::SendToAll(Network::Protocol::Packet &packet) {
    for (const auto& client : _clientList) {
        //std::cout << "Client : " << client.first << " romote ENDPOINT : " << client.second._remote_endpoint << std::endl;
        Send(packet, client.second._remote_endpoint);
    }
}

void NetworkServer::UdpSession::SendToAllExcept(Network::Protocol::Packet &packet, uint32_t id) {
    for (const auto& client : _clientList) {
        if (client.first != id) {
            Send(packet, client.second._remote_endpoint);
        }
    }
}

void NetworkServer::UdpSession::Run() {
    std::cout << "[UDP Session " << _sessionId << "] : thread run is " << (_io_service.stopped() ? "stopped" : "working") << "." << std::endl;
    while (!_io_service.stopped()) {
        _io_service.run();
    }
    std::cout << "[UDP Session " << _sessionId << "] : thread run end" << std::endl;
}

uint32_t NetworkServer::UdpSession::AssignClient(const boost::asio::ip::udp::endpoint& endpoint) {
    for (auto &it : _clientList) {
        if (it.second._endPointStatus == Network::EndpointStatus::FULFILL
            && it.second._remote_endpoint == endpoint) {
//            std::cout << "CLIENT RECEIVE FROM EXISTANT ENDPOINT : " << remote_endpoint << std::endl;
            return it.first;
        }
    }
    for (auto &it : _clientList) {
        if (it.second._endPointStatus == Network::EndpointStatus::NONE) {
//            std::cout << "CLIENT RECEIVE FROM NEW ENDPOINT : " << remote_endpoint << std::endl;
            it.second._remote_endpoint = endpoint;
            it.second._endPointStatus = Network::EndpointStatus::FULFILL;
            return it.first;
        }
    }
    return 0;
}

