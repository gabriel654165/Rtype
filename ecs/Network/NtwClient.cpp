/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-maxime.dodin
** File description:
** NtwClient.cpp
*/

#include "NtwClient.hpp"

Resource::NtwClient::NtwClient(const std::string &host, unsigned short port)
: _client(host, port), _nbPlayers(0), _yourUuid(10)//prcq 0 c un uuid
{
    //inint clients with constructors
    _client.Run();//can call receive callBack
}

Resource::NtwClient::NtwClient(Resource::NtwClient &copy): _client(copy.getClient())
{
    std::cout << "copy construct" << std::endl;
    //_protocol = copy.getPotocol();
}

Resource::NtwClient::~NtwClient()
{
    //close client sockets ??
}

NetworkClient::CoreClient &Resource::NtwClient::getClient()
{
    return _client;
}

udp::endpoint &Resource::NtwClient::getEnpoint()
{
    return _client._udpClient->server_endpoint;
}

std::list<std::pair<Network::Protocol::UnpackDataUdp, bool>> &Resource::NtwClient::getInputHistory()
{
    return _inputHistory;
}

void Resource::NtwClient::setNbPlayers(std::size_t &nbPlayers)
{
    _nbPlayers = nbPlayers;
}

std::size_t &Resource::NtwClient::getNbPlayers()
{
    return _nbPlayers;
}

void Resource::NtwClient::setYourUuid(std::size_t &yourUuid)
{
    _yourUuid = yourUuid;
}

std::size_t &Resource::NtwClient::getYourUuid()
{
    return _yourUuid;
}

void Resource::NtwClient::send(Network::Protocol::Packet &packet, udp::endpoint endPoint)
{
    //std::cout << endPoint << std::endl;
    _client._udpClient->SendToEndPt(packet, endPoint);
}