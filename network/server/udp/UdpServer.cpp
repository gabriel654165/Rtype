//
// Created by thomas on 08/11/2021.
//

#include "UdpServer.hpp"

NetworkServer::UdpServer::UdpServer(): _udpSessionID(0L), _portManager(6000, 10) {
}

NetworkServer::UdpServer::~UdpServer() {
}

// GETTER

Network::UdpSessionMode NetworkServer::UdpServer::GetUdpSessionMode(uint32_t sessionId) const {
    if (!_udpSessionList.empty())
        return _udpSessionList.at(sessionId)->GetSessionMode();
    return Network::UdpSessionMode::ERROR_MODE;
}

// SETTER

void NetworkServer::UdpServer::SetUdpSessionMode(uint32_t sessionId, Network::UdpSessionMode sessionMode) {
    if (!_udpSessionList.empty())
        _udpSessionList.at(sessionId)->SetSessionMode(sessionMode);
}

// UDP SESSION API

std::vector<Network::Digit> &NetworkServer::UdpServer::GetAllClientId(Network::Digit lobbyId)
{
    if (!_idList.empty()) {
        _idList.clear();
    }
    if (!_udpSessionList.empty()) {
        for (const auto &it: _udpSessionList.at(lobbyId)->GetClientList()) {
            _idList.emplace_back(it.first);
        }
    }
    return _idList;
}

uint32_t NetworkServer::UdpServer::CreateUdpSession(Network::Digit maxPlayerNb) {
    auto id = ++_udpSessionID;
    unsigned short port = _portManager.GetValidSocket();
    std::cout << "[UDP Session " << id << "] : is going to be created in port : " << port << "." << std::endl;
    _udpSessionList.insert({id, std::make_unique<EcsServer>(port, id, maxPlayerNb)});
    std::cout << "[UDP Session " << id << "] : created." << std::endl;
    return id;
}

void NetworkServer::UdpServer::RemoveUdpSession(uint32_t sessionId) {
    std::cout << "[UDP Session " << sessionId << "] : find for session " << sessionId << " to be deleted." << std::endl;
    bool goDelete = false;
    for (auto &it : _udpSessionList) {
        if (it.first == sessionId) {
            goDelete = true;
            break;
        }
    }
    if (goDelete) {
        _udpSessionList.at(sessionId)->Stop();
        _udpSessionList.erase(sessionId);
        std::cout << "[UDP Session " << sessionId << "] : delete session " << sessionId << "." << std::endl;
    }
}

Network::UdpSessionRole NetworkServer::UdpServer::AddClientToUdpSession(uint32_t clientId, uint32_t sessionId) {
    std::cout << "[UDP Session " << sessionId << "] : add client " << clientId << " to session " << sessionId << "." << std::endl;
    Network::UdpSessionRole sessionRole = Network::UdpSessionRole::ERROR_ROLE;
    if (!_udpSessionList.empty()) {
        sessionRole = _udpSessionList.at(sessionId)->AddClient(clientId);
        std::cout << "[UDP Session " << sessionId << "] : client " << clientId << " is "
                  << (sessionRole == Network::UdpSessionRole::ADMIN ? "Admin" : "Guest") << " of the session."
                  << std::endl;
    }
    return sessionRole;
}

void NetworkServer::UdpServer::SetUdpSessionClientReadyState(uint32_t clientId, uint32_t sessionId, bool isReady)
{
    if (!_udpSessionList.empty()) {
        _udpSessionList.at(sessionId)->SetClientReadyState(clientId, isReady);
    }
}

bool NetworkServer::UdpServer::IsUdpSessionReadyToPlay(uint32_t sessionId)
{
    if (!_udpSessionList.empty()) {
        return _udpSessionList.at(sessionId)->IsSessionReadyToPlay();
    }
    return false;
}

void NetworkServer::UdpServer::RemoveClientFromUdpSession(uint32_t clientId, uint32_t sessionId)
{
    if (!_udpSessionList.empty())
        _udpSessionList.at(sessionId)->RemoveClient(clientId);
}

void NetworkServer::UdpServer::InitUdpSession(uint32_t sessionId)
{
    if (!_udpSessionList.empty())
        _udpSessionList.at(sessionId)->InitCallBack();
}

bool NetworkServer::UdpServer::IsUdpSessionFull(uint32_t sessionId)
{
    return _udpSessionList.at(sessionId)->IsSessionFull();
}

bool NetworkServer::UdpServer::IsUdpSessionEmpty(uint32_t sessionId)
{
    return _udpSessionList.at(sessionId)->IsSessionEmpty();
}

Network::Digit NetworkServer::UdpServer::GetUdpSessionPort(uint32_t sessionId)
{
    return _udpSessionList.at(sessionId)->GetSessionPort();
}

Network::Digit NetworkServer::UdpServer::GetUdpSessionMaxPlayer(uint32_t sessionId)
{
    return _udpSessionList.at(sessionId)->GetNbPlayerMax();
}

Network::Digit NetworkServer::UdpServer::GetUdpSessionNbPlayer(uint32_t sessionId)
{
    return _udpSessionList.at(sessionId)->GetNbPlayer();
}

Network::Digit NetworkServer::UdpServer::GetUdpSessionNbRdyPlayer(uint32_t sessionId)
{
    return _udpSessionList.at(sessionId)->GetNbPlayerRdy();
}

void NetworkServer::UdpServer::GetEveryLobbyInfoPack()
{
    Network::Protocol::Manager protocol;

    protocol.pack("d", Network::Protocol::CodeTcp::ResDisconnected);

}

std::size_t NetworkServer::UdpServer::InitUdpSessionPlayerUuid(uint32_t sessionId)
{
    return _udpSessionList.at(sessionId)->AddPlayerToGameScene();
}

void NetworkServer::UdpServer::SetUdpSessionSpaceshipUuid(uint32_t sessionId, uint32_t clientId, std::size_t shipId)
{
    if (!_udpSessionList.empty())
        _udpSessionList.at(sessionId)->SetClientSpaceshipUuid(clientId, shipId);
}

std::size_t NetworkServer::UdpServer::GetUdpSessionSpaceshipUuid(uint32_t clientId, uint32_t sessionId) const
{
    if (!_udpSessionList.empty())
        return _udpSessionList.at(sessionId)->GetClientSpaceshipUuid(clientId);
    return 0;
}
