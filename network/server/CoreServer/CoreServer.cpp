//
// Created by thomas on 07/11/2021.
//

#include "CoreServer.hpp"

NetworkServer::CoreServer::CoreServer(unsigned short port): _udpServer(std::make_unique<NetworkServer::UdpServer>()), _tcpManager(std::make_unique<NetworkServer::ManagerTcpServer>()) 
{
    _tcpManager->Init();
    _tcpServer = _tcpManager->CreateTcpServer(_tcpSafeQueue);
    _tcpServer->Start(port);
}

void NetworkServer::CoreServer::Run() {
    TcpUpdate();
}

void NetworkServer::CoreServer::ChronoManagement() {
    Network::Protocol::Manager protocol;
    bool isErase = false;
    uint32_t tempId = 0;
    //std::size_t nbPlayers;
    for (auto &it : _chronoList) {
        std::cout << "je passes dans la liste des chronos" << std::endl;
        if (it.second->getElapsedTime() >= 3) {
            std::cout << "je rentre dans + de 3 secondes" << std::endl;
            tempId = it.first;
            //nbPlayers = _udpServer->GetAllClientId(tempId).size();
            protocol.pack("dd", Network::Protocol::CodeTcp::NotifyStartGame, it.first/*, nbPlayers*/);
            _tcpServer->SendToAll(protocol.p);
            _udpServer->InitUdpSession(it.first);
            isErase = true;
            break;
        }
    }
    if (isErase) {
        _chronoList.erase(tempId);
    }
}

[[noreturn]] void NetworkServer::CoreServer::TcpUpdate() {
    while (true) {
        if (!_tcpSafeQueue.empty()) {
            ChronoManagement();
            TcpCallback();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void NetworkServer::CoreServer::TcpCallback()
{
    const boost::system::error_code err;
    static const std::map<Network::Digit, std::function<void(Network::Digit, unsigned char *, const boost::system::error_code &)>> proto = {
            {Network::Protocol::CodeTcp::ReqDisconnect, [this](Network::Digit clientId, unsigned char *packet, const boost::system::error_code &er) {
                if (!er)
                    _reqDisconnection(clientId, packet);
            }}, {Network::Protocol::CodeTcp::ReqAskForInfo, [this](Network::Digit clientId, unsigned char *packet, const boost::system::error_code &er) {
                if (!er)
                    _reqAskForInfo(clientId, packet);
            }}, {Network::Protocol::CodeTcp::ReqDisconnectBySignal, [this](Network::Digit clientId, unsigned char *packet, const boost::system::error_code &er) {
                if (!er)
                    _reqDisconnectionBySignal(clientId, packet);
            }}, {Network::Protocol::CodeTcp::ReqConnectToLobby, [this](Network::Digit clientId, unsigned char *packet, const boost::system::error_code &er) {
                if (!er)
                    _reqConnectLobby(clientId, packet);
            }}, {Network::Protocol::CodeTcp::ReqDisconnectFromLobby, [this](Network::Digit clientId, unsigned char *packet, const boost::system::error_code &er) {
                if (!er)
                    _reqDisconnectLobby(clientId, packet);
            }}, {Network::Protocol::CodeTcp::ReqCreateLobby, [this](Network::Digit clientId, unsigned char *packet, const boost::system::error_code &er) {
                if (!er)
                    _reqCreateLobby(clientId, packet);
            }}, {Network::Protocol::CodeTcp::ReqReadyToPlay, [this](Network::Digit clientId, unsigned char *packet, const boost::system::error_code &er) {
                if (!er)
                    _reqReadyToPlay(clientId, packet);
            }}, {Network::Protocol::CodeTcp::ReqNotReadyToPlay, [this](Network::Digit clientId, unsigned char *packet, const boost::system::error_code &er) {
                if (!er)
                    _reqNotReadyToPlay(clientId, packet);
            }}};

    Network::Digit statusCode = 0;
    boost::system::error_code error;

    auto packet = _tcpSafeQueue.pop();
    Network::Protocol::Manager::unPack(packet.second, "d", &statusCode);
    try {
        //std::cout << "Je trouve une callback" << std::endl;
        proto.at(statusCode)(packet.first, packet.second, error);
    } catch (std::exception &error) {
        //std::cout << "Je trouve pas de callback" << std::endl;
    }
}

/////////////////////// PUBLIC CALLBACK TCP ///////////////////////

void NetworkServer::CoreServer::_reqConnection([[maybe_unused]]Network::Digit clientId, [[maybe_unused]]Network::Buffer packet)
{
}

void NetworkServer::CoreServer::_reqDisconnection(Network::Digit clientId, [[maybe_unused]]Network::Buffer packet)
{
    Network::Protocol::Manager protocol;
    try {
        if (_tcpServer->IsTcpSessionAlive(clientId)) {
            protocol.pack("d", Network::Protocol::CodeTcp::ResDisconnected);
            std::cout << "Server send disconnection response to client n°" << clientId << std::endl;
            _tcpServer->SendTo(protocol.p, clientId);
            _tcpServer->StopTcpConnection(clientId);
        }
    } catch (std::exception &error) {
    }
}

void NetworkServer::CoreServer::_reqDisconnectionBySignal(Network::Digit clientId, [[maybe_unused]]Network::Buffer packet)
{
    Network::Protocol::Manager protocol;
    Network::Protocol::UnpackData data;

    std::cout << "Server want to disconnect client " << clientId << "." << std::endl;
    Network::Protocol::Manager::unPack(packet, "dd", &data.typeHeader, &data.lobbyId);

    try {
        if (data.lobbyId != 0) {
            auto shipId = _udpServer->GetUdpSessionSpaceshipUuid(clientId, data.lobbyId);
            std::cout << "Ship to destroy is n°" << shipId << std::endl;
            std::cout << "Server Disconnect client n°" << clientId << " and remove client from lobby n°" << data.lobbyId <<std::endl;
            _udpServer->RemoveClientFromUdpSession(clientId, data.lobbyId);
            if (_udpServer->IsUdpSessionEmpty(data.lobbyId)) {
                std::cout << "Server want to stop udp session n°" << data.lobbyId << std::endl;
                _udpServer->RemoveUdpSession(data.lobbyId);
                protocol.pack("dd", Network::Protocol::CodeTcp::NotifyLobbyDeleted, data.lobbyId);
                _tcpServer->SendToAllExcept(protocol.p, clientId);
                std::cout << "Server send to all that lobby n°" << data.lobbyId << " get destroy cause nobody inside" << std::endl;
            } else if (!_udpServer->IsUdpSessionEmpty(data.lobbyId) && shipId != 0) {
                //std::cout << "ON ENVOI A TOUT LE MONDE LE FAIT DE DÉTRUIRE LE VAISSEAU " << shipId << std::endl;
                protocol.pack("ddd", Network::Protocol::CodeTcp::NotifyDisconnectedFromLobby, data.lobbyId, shipId);
                _tcpServer->SendToAllExcept(protocol.p, clientId);
                //std::cout << "Server send to all that lobby n°" << data.lobbyId << " disconnected a client" << std::endl;
            }
        }
        if (_tcpServer->IsTcpSessionAlive(clientId)) {
            _tcpServer->StopTcpConnection(clientId);
            std::cout << "Server disconnect tcp session of client " << clientId << "." << std::endl;
        }
    } catch (std::exception &error) {
    }

}

void NetworkServer::CoreServer::_reqAskForInfo(Network::Digit clientId, [[maybe_unused]]Network::Buffer packet)
{
    std::cout << "Server receive <AskInfo> from client" << clientId << std::endl;
    Network::Protocol::Manager protocol;
    Network::Protocol::UnpackDataUdp data;
    if (!_udpServer->_udpSessionList.empty()) {
        data.list = new std::size_t[3 * _udpServer->_udpSessionList.size()];
        data.lenList = sizeof(std::size_t) * 3 * _udpServer->_udpSessionList.size();
        std::cout << "SIZE LIST = " << data.lenList << std::endl;
        for (auto &udp: _udpServer->_udpSessionList) {
            data.list[0] = udp.first;
            data.list[1] = udp.second->GetNbPlayer();
            data.list[2] = udp.second->GetNbPlayerRdy();
            std::cout << "SERVER SEND : " << std::endl <<
                         "             - lobby id : " << udp.first << std::endl <<
                         "             - nb player : " << udp.second->GetNbPlayer() << std::endl <<
                         "             - max player : " << udp.second->GetNbPlayerMax() << std::endl;
        }

        protocol.pack("dlu", Network::Protocol::CodeTcp::ResAskForInfo, data.lenList, data.list);
        _tcpServer->SendTo(protocol.p, clientId);
        delete [] data.list;
        std::cout << "Server send udp info to client " << clientId << std::endl;
    }
}

void NetworkServer::CoreServer::_reqConnectLobby(Network::Digit clientId, Network::Buffer packet) {
    Network::Protocol::Manager protocol;
    std::vector<Network::Digit> receiveTab;
    Network::Protocol::UnpackData data;

    Network::Protocol::Manager::unPack(packet, "dd", &data.typeHeader, &data.lobbyId);
    try {
        if (!_udpServer->IsUdpSessionFull(data.lobbyId)) {
            auto sessionMode = _udpServer->GetUdpSessionMode(data.lobbyId);
            _udpServer->AddClientToUdpSession(clientId, data.lobbyId);
            std::cout << "Server Connect client n°" << clientId << " to lobby n°" << data.lobbyId << std::endl;
            protocol.pack("dddd", Network::Protocol::CodeTcp::ResConnectedToLobby, data.lobbyId,
                sessionMode == Network::LOBBY ? Network::GUEST : Network::PASSIVE_GUEST, _udpServer->GetUdpSessionPort(data.lobbyId));
            _tcpServer->SendTo(protocol.p, clientId);
            std::cout << "Server send to client n°" << clientId << " connection approved" << std::endl;
            std::this_thread::sleep_for (std::chrono::milliseconds(50));
            protocol.pack("dd", Network::Protocol::CodeTcp::NotifyConnectedToLobby, data.lobbyId);
            _tcpServer->SendToAllExcept(protocol.p, clientId);
            std::cout << "Server notify that client connected to lobby " << data.lobbyId << std::endl;
        } else {
            protocol.pack("d", Network::Protocol::CodeTcp::ResErrorConnectionToLobby);
            _tcpServer->SendTo(protocol.p, clientId);
            std::cout << "server send error message to client n°" << clientId << std::endl;
        }
    } catch (std::exception &error) {
    }
}

void NetworkServer::CoreServer::_reqDisconnectLobby(Network::Digit clientId, Network::Buffer packet) {
    Network::Protocol::UnpackData data;
    Network::Protocol::Manager protocol;
    std::vector<Network::Digit> receiveTab;

    Network::Protocol::Manager::unPack(packet, "dd", &data.typeHeader, &data.lobbyId);
    try {
        auto shipId = _udpServer->GetUdpSessionSpaceshipUuid(clientId, data.lobbyId);
        std::cout << "Server Disconnect client n°" << clientId << " and remove client from lobby n°" << data.lobbyId <<std::endl;
        _udpServer->RemoveClientFromUdpSession(clientId, data.lobbyId);
        protocol.pack("dd", Network::Protocol::CodeTcp::ResDisconnectedFromLobby, data.lobbyId);
        _tcpServer->SendTo(protocol.p, clientId);
        std::cout << "server send disconnection response to client" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (_udpServer->IsUdpSessionEmpty(data.lobbyId)) {
            std::cout << "Server want to stop udp session n°" << data.lobbyId << std::endl;
            _udpServer->RemoveUdpSession(data.lobbyId);
            protocol.pack("dd", Network::Protocol::CodeTcp::NotifyLobbyDeleted, data.lobbyId);
            _tcpServer->SendToAllExcept(protocol.p, clientId);
            std::cout << "Server send to all that lobby n°" << data.lobbyId << " get destroy cause nobody inside" << std::endl;
        } else {
            protocol.pack("dd", Network::Protocol::CodeTcp::NotifyDisconnectedFromLobby, data.lobbyId, shipId);
            _tcpServer->SendToAllExcept(protocol.p, clientId);
            std::cout << "Server send to all that lobby n°" << data.lobbyId << " disconnected a client" << std::endl;
        }
        std::cout << "Server close lobby n°" << data.lobbyId << " cause lobby is empty" << std::endl;
    } catch (std::exception &error) {

    }
}

void NetworkServer::CoreServer::_reqCreateLobby(Network::Digit clientId, Network::Buffer packet) {
    Network::Protocol::UnpackData data;
    Network::Protocol::Manager protocol;

    Network::Protocol::Manager::unPack(packet, "dd", &data.typeHeader, &data.maxPlayer);
    try {
        std::cout << "Server receive req to create a lobby with " << data.maxPlayer << " max player" << std::endl;
        uint32_t udpSessionId = _udpServer->CreateUdpSession(data.maxPlayer);
        _udpServer->AddClientToUdpSession(clientId, udpSessionId);
        protocol.pack("ddddd", Network::Protocol::CodeTcp::ResConnectedToLobby, udpSessionId, Network::UdpSessionRole::ADMIN,
            _udpServer->GetUdpSessionPort(udpSessionId), data.maxPlayer);
        _tcpServer->SendTo(protocol.p, clientId);
        std::this_thread::sleep_for (std::chrono::milliseconds(50));
        std::cout << "Server send to Client n°" << clientId << " create lobby n°" << udpSessionId << " in server" << std::endl;
        protocol.pack("ddd", Network::Protocol::CodeTcp::NotifyLobbyCreated, udpSessionId, data.maxPlayer);
        _tcpServer->SendToAllExcept(protocol.p, clientId);
        std::cout << "Server notify all that client create lobby n°" << udpSessionId <<" with " << data.maxPlayer << " max player" << std::endl;
    } catch (std::exception &error) {
    }
}

void NetworkServer::CoreServer::_reqReadyToPlay(Network::Digit clientId, Network::Buffer packet) {
    Network::Protocol::UnpackData data;
    Network::Protocol::Manager protocol;

    Network::Protocol::Manager::unPack(packet, "dd", &data.typeHeader, &data.lobbyId);
    std::cout << "Server receive : client n°" << clientId << " is ready to play in lobby " << data.lobbyId << std::endl;
    try {
        _udpServer->SetUdpSessionClientReadyState(clientId, data.lobbyId, true);
        if (_udpServer->IsUdpSessionReadyToPlay(data.lobbyId)) {

            std::cout << "Server notify all that game Started." << std::endl;

            _udpServer->InitUdpSession(data.lobbyId);
            protocol.pack("ddd", Network::Protocol::CodeTcp::NotifyStartGame, data.lobbyId, data.nbPlayer);
            _tcpServer->SendToAll(protocol.p);

            std::this_thread::sleep_for (std::chrono::milliseconds(50));
            data.nbPlayer = _udpServer->GetAllClientId(data.lobbyId).size();
            for (auto &udpClientId : _udpServer->GetAllClientId(data.lobbyId)) {
                auto spaceShipUuid = _udpServer->InitUdpSessionPlayerUuid(data.lobbyId);
                _udpServer->SetUdpSessionSpaceshipUuid(data.lobbyId, udpClientId, spaceShipUuid);
                protocol.pack("ddd", Network::Protocol::CodeTcp::ResAddYourSpaceship, data.lobbyId, spaceShipUuid);
                _tcpServer->SendTo(protocol.p, (uint32_t)udpClientId);
                std::cout << "Server send to client " << udpClientId << " that he have to add spaceShip n°" << spaceShipUuid << std::endl;
                protocol.pack("ddd", Network::Protocol::CodeTcp::ResAddASpaceship, data.lobbyId, spaceShipUuid);
                std::cout << "Server send to all client that they have to add spaceShip n°" << spaceShipUuid << std::endl;
                _tcpServer->SendToAllExcept(protocol.p, (uint32_t)udpClientId);
                std::this_thread::sleep_for (std::chrono::milliseconds(50));
            }
        } else {
            protocol.pack("dd", Network::Protocol::CodeTcp::ResReadyToPlay, data.lobbyId);
            _tcpServer->SendTo(protocol.p, clientId);
            std::cout << "JE PASSES DANS LE NOTIFY READY TO PLAY" << std::endl;
//        std::cout << "Server send to Client n°" << clientId << " response for ready to play" << std::endl;
            protocol.pack("dd", Network::Protocol::CodeTcp::NotifyReadyToPlay, data.lobbyId);
            _tcpServer->SendToAllExcept(protocol.p, clientId);
            std::cout << "Server notify all that client " << clientId << " is ready to play" << std::endl;
        }
    } catch (std::exception &error) {
    }
}

void NetworkServer::CoreServer::_reqNotReadyToPlay(Network::Digit clientId, Network::Buffer packet) {
    Network::Protocol::UnpackData data;
    Network::Protocol::Manager protocol;

    Network::Protocol::Manager::unPack(packet, "dd", &data.typeHeader, &data.lobbyId);
    std::cout << "Server receive request for not ready to play from client " << clientId << std::endl;
    try {
        _udpServer->SetUdpSessionClientReadyState(clientId, data.lobbyId, false);
        protocol.pack("dd", Network::Protocol::CodeTcp::NotifyNotReadyToPlay, data.lobbyId);
        _tcpServer->SendToAll(protocol.p);
        std::cout << "Server notify all that client " << clientId << " is not ready to play" << std::endl;

    } catch (std::exception &error) {
    }
}