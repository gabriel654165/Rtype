//
// Created by thomas on 07/11/2021.
//
// Created by thomas on 07/11/2021.
//
#include <thread>
#include "CoreClient.hpp"
#include <chrono>

//_udpClient(std::make_unique<NetworkClient::Udp::UdpClient>()) to add in constructor

NetworkClient::CoreClient::CoreClient(const std::string &host, unsigned short port) :_tcpClient(std::make_unique<NetworkClient::Tcp::TcpClient>(host, port, _tcpSafeQueue)), _host(host), _readyToPlay(false),_startGame(false)
{
    _udpPort = 0;
}

NetworkClient::CoreClient::CoreClient([[maybe_unused]]NetworkClient::CoreClient &copy)
{/*
    _tcpClient = std::move(copy._tcpClient);
    _udpClient = std::move(copy._udpClient);
    _tcpSafeQueue = copy._tcpSafeQueue;
    _udpSafeQueue = copy._udpSafeQueue;

    _lobbys = copy._lobbys;

    _host = const_cast<const std::string>(copy._host);
    _threadContext = std::move(copy._threadContext);
    _readyToPlay = copy._readyToPlay;
    _startGame = copy._startGame;*/
}


void NetworkClient::CoreClient::CreateNewUdpClient()
{
    _udpClient = std::make_unique<NetworkClient::Udp::UdpClient>(_udpPort, _udpSafeQueue, _remoteEndpoint);
}

void NetworkClient::CoreClient::Run()
{
    _tcpClient->Start();
}

/*void NetworkClient::CoreClient::TcpUpdate()
{
        while (!SignalHandler::gotExitSignal());
    SignalHandler signalHandler; // TODO : METTRE DANS LE MAIN
    signalHandler.setupSignalHandlers();

    if (SignalHandler::gotExitSignal()) {
        std::cout << "SIGNAL DISCONNECT" << std::endl;
        _reqDisconnectionBySignal();
    } else {
        _tcpClient->Stop();
    }
}*/

void task1()
{
    using namespace std::chrono_literals;

    // std::cout << "3" << "\n";
    // std::this_thread::sleep_for(1000ms);
    // std::cout << "2" << "\n";
    // std::this_thread::sleep_for(1000ms);
    // std::cout << "1" << "\n";
    // std::this_thread::sleep_for(1000ms);
    std::cout << "Game Start " << "\n";
}

void NetworkClient::CoreClient::startGame() {
    std::thread thread_1(task1);
    thread_1.join();
}


const NetworkClient::Udp::UdpClient &NetworkClient::CoreClient::getUdpClient() const {
    return *_udpClient;
}

const NetworkClient::Tcp::TcpClient &NetworkClient::CoreClient::getTcpClient() const {
    return *_tcpClient;
}

void NetworkClient::CoreClient::ReqDisconnectionBySignal(const std::size_t lobbyId)
{
    Network::Protocol::Manager protocol;

    try {
        std::cout << "CLIENT SEND DISCONNECTION TO SERVER" << "\n";
        protocol.pack("d", Network::Protocol::CodeTcp::ReqDisconnectBySignal, lobbyId);
        _tcpClient->Send(reinterpret_cast<Network::Protocol::Packet &>(protocol.p));
        if (GetUdpState()) {
            std::cout << "JE STOP L'UDP SI UNE SESSION EST EN COURS" << std::endl;
            _udpClient->Stop();
            _udpClient.reset();
        }
        _tcpClient->Stop();
        std::cout << "Client close all" << std::endl;
    }
    catch (std::exception &error)
    {
        std::cerr << "Error request disconection" << "\n";
    }
}

void NetworkClient::CoreClient::_reqDisconnection()
{
    Network::Protocol::Manager protocol;

    std::cout << "Client request disconnection" << "\n";
    try
    {
        protocol.pack("d", Network::Protocol::CodeTcp::ReqDisconnect);
        _tcpClient->Send(reinterpret_cast<Network::Protocol::Packet &>(protocol.p));
    }
    catch (std::exception &error)
    {
        std::cerr << "Error request disconection" << "\n";
    }
}

void NetworkClient::CoreClient::_reqConnectLobby(const std::size_t lobbyId)
{
    Network::Protocol::Manager protocol;

    try
    {
        std::cout << "Client requets Connection to lobby" << "\n";
        protocol.pack("dd", Network::Protocol::CodeTcp::ReqConnectToLobby, lobbyId);
        _tcpClient->Send(reinterpret_cast<Network::Protocol::Packet &>(protocol.p));
    }
    catch (std::exception &error)
    {
        std::cerr << "Error request connection" << "\n";
    }
}

void NetworkClient::CoreClient::_reqDisconnectLobby()
{
    Network::Protocol::Manager protocol;
    std::size_t lobby_id = _tcpClient->getInLobby().second;

    std::cout << "Client request for disconnection from lobby n°" << lobby_id << "\n";
    try
    {
        if (_tcpClient->getInLobby().first)
        {
            protocol.pack("dd", Network::Protocol::CodeTcp::ReqDisconnectFromLobby, lobby_id);
            _tcpClient->Send(reinterpret_cast<Network::Protocol::Packet &>(protocol.p));
        }
        else
            std::cout << "client not in lobby yet\n";
    }
    catch (std::exception &error)
    {
        std::cerr << "Error request disconection" << "\n";
    }
}

void NetworkClient::CoreClient::_reqCreateLobby(std::size_t maxPlayer)
{
    Network::Protocol::Manager protocol;

    try
    {
        std::cout << "CLient ask for lobby creation with " << maxPlayer << "max player" << "\n";
        if (!_tcpClient->getInLobby().first)
        {
            protocol.pack("dd", Network::Protocol::CodeTcp::ReqCreateLobby, maxPlayer);
            _tcpClient->Send(reinterpret_cast<Network::Protocol::Packet &>(protocol.p));
        }
        else
            std::cout << "client already in lobby yet\n";
    }
    catch (std::exception &error)
    {
        std::cerr << "Error request Create lobby" << "\n";
    }
}

void NetworkClient::CoreClient::_reqGameEvent(__attribute__((unused)) const std::size_t entityId, __attribute__((unused)) const std::size_t event)
{
}

void NetworkClient::CoreClient::_reqNotReadyToPlay()
{
    Network::Protocol::Manager protocol;

    try
    {
        if (_tcpClient->getInLobby().first)
        {
            std::cout << "Client request not ready to play" << "\n";
            protocol.pack("dd", Network::Protocol::CodeTcp::ReqNotReadyToPlay, _tcpClient->getInLobby().second);
            _tcpClient->Send(*reinterpret_cast<Network::Protocol::Packet *>(&protocol.p));
            _readyToPlay = false;
        }
        else
        {
            std::cout << "Client should be in a lobby";
        }
    }
    catch (std::exception &error)
    {
        std::cerr << "Error request not ready to play" << "\n";
    }
}

void NetworkClient::CoreClient::_reqReadyToPlay()
{
    Network::Protocol::Manager protocol;
    std::cout << "Client request ready to play" << "\n";

    try
    {
        if (_tcpClient->getInLobby().first)
        {
            protocol.pack("dd", Network::Protocol::CodeTcp::ReqReadyToPlay, _tcpClient->getInLobby().second);
            _tcpClient->Send(*reinterpret_cast<Network::Protocol::Packet *>(&protocol.p));
            _readyToPlay = true;
        }
        else
        {
            std::cout << "Client should be in a lobby\n";
        }
    }
    catch (std::exception &error)
    {
        std::cerr << "Error request ready to play" << "\n";
    }
}

std::unique_ptr<NetworkClient::Tcp::TcpClient> &NetworkClient::CoreClient::GetTcpClient() {
    return _tcpClient;
}

std::unordered_map<Network::Digit, std::unique_ptr<NetworkClient::Lobby>> &NetworkClient::CoreClient::GetLobbys() {
    return _lobbys;
}

void NetworkClient::CoreClient::SetStartGame(bool sg) {
    _startGame = sg;
}

bool NetworkClient::CoreClient::getStartGame() const {
    return _startGame;
}

Network::LockedQueue<unsigned char *> &NetworkClient::CoreClient::GetTcpSafeQueue() {
    return _tcpSafeQueue;
}

Network::LockedQueue<unsigned char *> &NetworkClient::CoreClient::GetUdpSafeQueue() {
    return _udpSafeQueue;
}

void NetworkClient::CoreClient::SetUdpPort(unsigned short udpPort)
{
    _udpPort = udpPort;
}

bool NetworkClient::CoreClient::IsUdpClientStart() const
{
    return _isUdpOpen;
}

void NetworkClient::CoreClient::SetUdpState(bool isOpen)
{
    _isUdpOpen = isOpen;
}

bool NetworkClient::CoreClient::GetUdpState() const {
    return _isUdpOpen;
}

void NetworkClient::CoreClient::SetRemoteEndPoint(std::string &endpoint) {
    _remoteEndpoint = endpoint;
}

std::string &NetworkClient::CoreClient::GetRemoteEndPoint() {
    return _remoteEndpoint;
}
