//
// Created by thomas on 17/11/2021.
//

#pragma once
#include "Registry.hpp"
#include "UdpSession.hpp"
#include "Components.hpp"
#include "MyClock.hpp"
#include "EntityLoad.hpp"

class EcsServer {
public:
    EcsServer(unsigned short, uint32_t, Network::Digit);
    ~EcsServer();
    void Start();
    void Stop();

    ///////////////////////////////////////////////////////////


    [[nodiscard]] Network::UdpSessionMode GetSessionMode() const;
    const std::map<uint32_t, NetworkServer::ClientData> &GetClientList() const;
    void SetSessionMode(Network::UdpSessionMode);
    Network::UdpSessionRole AddClient(uint32_t);
    void InitCallBack();
    void RemoveClient(uint32_t);
    void RemoveAllClient();
    bool IsSessionFull();
    bool IsSessionEmpty();
    Network::Digit GetNbPlayerRdy();
    Network::Digit GetNbPlayer();
    Network::Digit GetNbPlayerMax() const;
    void SetClientReadyState(uint32_t, bool);
    Network::Digit GetSessionPort() const;
    void Update();
    bool IsSessionReadyToPlay();
    Network::Protocol::UnpackDataUdp &GetDataUdp();
    Network::PairQueue<Network::Digit, unsigned char *> &GetSafeQueue();
    Registry &GetRegistry();
    void SetClientSpaceshipUuid(uint32_t, std::size_t);
    std::size_t GetClientSpaceshipUuid(uint32_t) const;
    std::mutex &GetMutexKillEntity();
    EntityLoad &GetLoader();
    std::size_t AddPlayerToGameScene();


    //////////////////////////////////////////////////////////////

private:
    MyClock &getClock();
    double &getDeltaTime();
    void resetDeltaTime();
    void registerComponents();
    void addGameSystemsServer();
    void initPlayer();
    void initLimitLevel();


//    boost::asio::io_service _io_service;

    std::unique_ptr<NetworkServer::UdpSession> _udpSession;
    MyClock _clock;
    Registry _game;
    Network::PairQueue<Network::Digit, unsigned char *> _safeQueue;
    std::thread _threadGroup;
    Network::Protocol::UnpackDataUdp _dataUdp;
    Network::Digit _port;
    std::size_t _id;
    EntityLoad _loader;

    ///////////////////////////////////////////////////////////////

    std::map<uint32_t, NetworkServer::ClientData> _clientList;
    Network::UdpSessionMode _sessionMode;
    uint32_t _sessionId;
    Network::Digit _maxNbPlayer;
    std::mutex _mutexSendSpaceship;
    std::mutex _mutexKillEntity;
    bool _isPlaying;
};