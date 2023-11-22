/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-maxime.dodin
** File description:
** NtwServer.cpp
*/

#include "NtwServer.hpp"
#include "Manager.hpp"

Resource::NtwServer::NtwServer(unsigned short port): _server(port)
{
    _server.RunOnlyUdpServer();
}

Resource::NtwServer::~NtwServer()
{
    //close client sockets ??
}

NetworkServer::CoreServer &Resource::NtwServer::getServer()
{
    return _server;
}

MyClock &Resource::NtwServer::getClock()
{
    return _clock;
}

Network::Protocol::UnpackDataUdp &Resource::NtwServer::getDataClient()
{
    return _dataUdp;
}

void Resource::NtwServer::resetClock()
{
    _clock.resetDeltaTime();
}

double &Resource::NtwServer::getDeltaTime()
{
    return _clock.getElapsedTime() ;
}

NetworkServer::UdpSession * Resource::NtwServer::getSession(std::size_t idUdpSession)
{
    return _server._udpServer->_udpSessionList[idUdpSession].get();
}

Network::PairQueue<Network::Digit, unsigned char *> &Resource::NtwServer::getSafeQueue(std::size_t idUdpSession)
{
    return _server._udpServer->_udpSessionList[idUdpSession].get()->_safeQueue;
}
