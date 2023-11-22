/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-Rtype-Maxime.fleury
** File description:
** TcpConnection.cpp
*/

#include "TcpConnection.hpp"

NetworkServer::TcpConnection::TcpConnection(uint32_t clientId, boost::asio::io_context &io_context, Network::PairQueue<Network::Digit, unsigned char *> &mQueue) :
    _clientId(clientId), _buff{0}, _io_context(io_context), _socket(io_context), _messageQueue(mQueue)
{
    _isOpen = true;
}

NetworkServer::TcpConnection::~TcpConnection() {
    std::cout << "TCP CONNECTION DESTRUCTOR" << std::endl;
    _io_context.stop();
    if (_socket.is_open())
        _socket.close();
}

std::string NetworkServer::TcpConnection::GetRemoteEndpoint() {
    return _socket.local_endpoint().address().to_string();
}

void NetworkServer::TcpConnection::Stop() {
    std::cout << "Server close socket n°" << _clientId << std::endl;
    _isOpen = false;
    _io_context.stop();
    if (_socket.is_open())
        _socket.close();
}

uint32_t NetworkServer::TcpConnection::GetId() const noexcept
{
    return (_clientId);
}

bool NetworkServer::TcpConnection::IsConnected() const
{
    return (_socket.is_open());
}

void NetworkServer::TcpConnection::StartRead()
{
    const boost::system::error_code err;
    size_t byteTransferred;
    if (_isOpen) {
        //boost::asio::async_read_until(_socket, _buffer, '\n',
        //    std::bind(&NetworkServer::TcpConnection::HandleRead, shared_from_this(), err, byteTransferred));
        _socket.async_receive(boost::asio::buffer(_buff),
                              std::bind(&NetworkServer::TcpConnection::HandleRead, shared_from_this(), err,
                                        byteTransferred));
    }
}

void NetworkServer::TcpConnection::HandleRead(const boost::system::error_code &err, [[maybe_unused]]std::size_t byteTransferred)
{
    //std::cout << "server receive " << (err == boost::asio::error::eof ? "invalid" : "valid") << " message" << std::endl;

    if (!err) {
//        std::cout << "SERVER receive a packet of " << byteTransferred << " bytes" << std::endl;
        /*if (!Network::Protocol::Manager::unPack(_buff, nullptr)) {
            std::cout << "error on message receive" << std::endl;
            return;
        }*/
        //std::cout << "Client n°" << _clientId << " send message to server" << std::endl;
        _messageQueue.push(_clientId, _buff);
    } else {
        std::cout << "TCP Connection n°" << _clientId << " session is now dead due to read error on the socket" << std::endl;
        _socket.close();
    }
    StartRead();
}

void NetworkServer::TcpConnection::StartWrite(Network::Protocol::NetworkPacket &packet)
{
    boost::system::error_code errorCode;
    size_t byte_transferred;
    _socket.async_write_some(boost::asio::buffer(packet.buffer),
        std::bind(&NetworkServer::TcpConnection::HandleWrite, shared_from_this(), errorCode, byte_transferred));
}

void NetworkServer::TcpConnection::HandleWrite(const boost::system::error_code &err, [[maybe_unused]]std::size_t byte_transferred)
{
    if (!err) {
        //std::cout << "SERVER write a packet of " << byte_transferred << " byte" << std::endl;
    } else {
        std::cout << "SERVER error on socket" << std::endl;
        _socket.close();
    }
}

boost::asio::ip::tcp::socket &NetworkServer::TcpConnection::GetSocket() {
    return (_socket);
}