//
// Created by thomas on 17/11/2021.
//

#include "EcsServer.hpp"

void receive_system([[maybe_unused]] Registry &r, Network::PairQueue<Network::Digit, unsigned char *> &safeQueue, Network::Protocol::UnpackDataUdp &data);
void send_system([[maybe_unused]] Registry &r, std::unique_ptr<NetworkServer::UdpSession> &session, MyClock &clock, Network::Protocol::UnpackDataUdp &data, sparseArray<Position> &pos, sparseArray<Uuid> &uuid, EntityLoad &loader, std::size_t &id);
void movement_system_server([[maybe_unused]]Registry &r, MyClock &clock, sparseArray<Position> &ctrlPos, sparseArray<Velocity> &ctrlVel);
void event_system_server([[maybe_unused]]Registry &r, Network::Protocol::UnpackDataUdp &data, sparseArray<Velocity> &vel, sparseArray<Uuid> &uuid);
void collision_system(
    [[maybe_unused]] Registry &r,
    MyClock &clock,
    sparseArray<Position> &posArr,
    sparseArray<Collider> &colArr,
    sparseArray<Scale> &scaleArr,
    sparseArray<Velocity> &velArr
    );

void spawner_system(
    [[maybe_unused]] Registry &r,
    MyClock &clock,
    sparseArray<Spawner> &spawnArr,
    EntityLoad &loader
    );

void players_ship_system(
    [[maybe_unused]] Registry &r,
    sparseArray<Velocity> &velArr,
    MyClock &clock,
    std::mutex &mutexKillEntity,
    std::unique_ptr<NetworkServer::UdpSession> &session,
    sparseArray<Spawner> &spawnArr,
    sparseArray<Position> &posArr,
    sparseArray<Collider> &colArr,
    sparseArray<Scale> &,
    sparseArray<Parent> &,
    sparseArray<Follow> &,
    sparseArray<Uuid> &
);
void follow_system([[maybe_unused]] Registry &r, sparseArray<Follow> &followArr, sparseArray<Position> &posArr, sparseArray<Velocity> &velArr);
void initEnemies(Registry &game)
{
    auto mobSpawner = game.spawn_entity();
    game.add_entity_to_scene(mobSpawner, "default");
    game.tag_entity(mobSpawner, "spawners");
    game.emplace_component<Position>(mobSpawner, 1500, 500);
    auto &spawner = game.emplace_component<Spawner>(mobSpawner, Spawner::MOB);
    spawner->timeToWait = 2;
    spawner->active = true;
}

EcsServer::EcsServer(unsigned short port, uint32_t sessionId, Network::Digit maxNbPlayer) :
    _udpSession(std::make_unique<NetworkServer::UdpSession>(port, sessionId, _safeQueue, _clientList))
{
    std::cout << "ECS UDP SESSION IS CREATE" << std::endl;
    _sessionId = sessionId;
    _maxNbPlayer = maxNbPlayer;
    _sessionMode = Network::UdpSessionMode::LOBBY;
    _isPlaying = false;
    _port = port;
    _id = 1;
    registerComponents();
    _loader.loadEntity(_game);
    initEnemies(_game);
    addGameSystemsServer();
    initLimitLevel();
}

EcsServer::~EcsServer()
{
//    _io_service.stop();
    std::cout << "ECS SERVER DESTRUCTOR" << std::endl;
    if (_threadGroup.joinable())
        _threadGroup.join();
}

MyClock &EcsServer::getClock()
{
    return _clock;
}

double &EcsServer::getDeltaTime()
{
    return _clock.getElapsedTime();
}

void EcsServer::resetDeltaTime()
{
    _clock.resetDeltaTime();
}

void EcsServer::registerComponents()
{
    _game.register_component<Position>();
    _game.register_component<Velocity>();
    _game.register_component<Sprite>();
    _game.register_component<Controllable>();
    _game.register_component<IU>();
    _game.register_component<Collider>();
    _game.register_component<Scale>();
    _game.register_component<Follow>();
    _game.register_component<Spawner>();
    _game.register_component<Uuid>();
    _game.register_component<Container<std::size_t>>();
    _game.register_component<Container<bool>>();
    _game.register_component<Parent>();
    _game.register_component<BeamBonusInfo>();
}

void EcsServer::addGameSystemsServer()
{
    _game.add_system_to_current_scene<>(
            [&safeQueue = _safeQueue, &dataUdp = GetDataUdp()]
            ([[maybe_unused]] Registry &r)
            { receive_system(r, safeQueue, dataUdp); }
    );

    _game.add_system_to_current_scene<Spawner>(
            [&clock = getClock(), &loader = GetLoader()]
            (Registry &r, sparseArray<Spawner> &spawn)
            { spawner_system(r, clock, spawn, loader); }
    );//mettre un uuid sur le bullet

    _game.add_system_to_current_scene<Velocity, Uuid>(
            [&mutex = _mutexSendSpaceship, &dataUdp = GetDataUdp()]
            ([[maybe_unused]]Registry &r, sparseArray<Velocity> &vel, sparseArray<Uuid> &uuid)
            {
            //    std::unique_lock<std::mutex> lock(mutex);
                event_system_server(r, dataUdp, vel, uuid);
            }
    );
    _game.add_system_to_current_scene<Position, Collider, Scale, Velocity>(
            [&clock = getClock()]
            (Registry &r, sparseArray<Position> &ctrlPos, sparseArray<Collider> &colArr, sparseArray<Scale> &scaleArr, sparseArray<Velocity> &velArr)
            { collision_system(r, clock, ctrlPos, colArr, scaleArr, velArr); }
    );

    _game.add_system_to_current_scene<Velocity, Spawner, Position, Collider, Scale, Parent, Follow, Uuid>(
            [&clock = getClock(), &mutexKillEntity = GetMutexKillEntity(), &session = _udpSession]
            (Registry &r, sparseArray<Velocity> &vel, sparseArray<Spawner> &spawn, sparseArray<Position> &pos, sparseArray<Collider> &col, sparseArray<Scale> &scale, sparseArray<Parent> &parent, sparseArray<Follow> &follow, sparseArray<Uuid> &uuid)
            { players_ship_system(r, vel, clock, mutexKillEntity, session, spawn, pos, col, scale, parent, follow, uuid); }
    );
    _game.add_system_to_current_scene<Position, Velocity>(
            [&mutex = _mutexSendSpaceship, &clock = getClock()]
            ([[maybe_unused]]Registry &r, sparseArray<Position> &ctrlPos, sparseArray<Velocity> &ctrlVel)
            {
            //    std::unique_lock<std::mutex> lock(mutex);
                movement_system_server(r, clock, ctrlPos, ctrlVel);
            }
    );
    _game.add_system_to_current_scene<Follow, Position, Velocity>(
            []
                    (Registry &r, sparseArray<Follow> &follow, sparseArray<Position> &pos, sparseArray<Velocity> &vel)
            { follow_system(r, follow, pos, vel); }
        );
     _game.add_system_to_current_scene<Position, Uuid>(
            [&mutex = _mutexSendSpaceship, &session = _udpSession, &dataUdp = GetDataUdp(), &clock = getClock(),  &loader = GetLoader(), &id = _id]
            ([[maybe_unused]] Registry &r, sparseArray<Position> &pos, sparseArray<Uuid> &uuid)
            {
            //    std::unique_lock<std::mutex> lock(mutex);
                send_system(r, session, clock, dataUdp, pos, uuid, loader, id); }
    );
    _game.add_system_to_current_scene<>(
            [&clock = getClock(), &dataUdp = GetDataUdp()/*, &dataUdpList = GetDataUdp().list*/]
            ([[maybe_unused]] Registry &r)
            {
                clock.resetDeltaTime();
                dataUdp.typeHeader = 0;
                std::memset(dataUdp.list, 0, dataUdp.lenList);
                dataUdp.lenList = 0;
            }
    );
}

std::size_t EcsServer::AddPlayerToGameScene()
{
    auto player = _game.spawn_entity();
    std::size_t tempId = _id;
    //std::cout << "--------------Create SHIP : id = " << _id << "--------------" << std::endl;

    _game.add_entity_to_scene(player, "default");
    _game.tag_entity(player, "players");
    _game.emplace_component<Velocity>(player, 0, 0, 400);
    _game.emplace_component<Position>(player, 100, (100 * _id));
    _game.emplace_component<Collider>(player, RectangleCollider{.width = 33, .height = 17});
    _game.emplace_component<Uuid>(player, _id);
    _id += 1;
    //std::cout << "--------------GO OUT OF SHIP CREATION--------------" << std::endl;
    return tempId;
}

void EcsServer::initLimitLevel()
{
    //level init
    auto firstBlockDown = _game.spawn_entity();
    _game.add_entity_to_scene(firstBlockDown, "default");
    _game.tag_entity(firstBlockDown, "level");
    _game.emplace_component<Position>(firstBlockDown, 1200, (1080 - 250) );
    _game.emplace_component<Velocity>(firstBlockDown, -200, 0, 0);
    _game.emplace_component<Scale>(firstBlockDown, 1.3, 1);
    _game.emplace_component<Collider>(firstBlockDown, RectangleCollider{.width = 100, .height = 250});

    //screen limit
    auto lvlDownLimit = _game.spawn_entity();
    _game.add_entity_to_scene(lvlDownLimit, "default");
    _game.tag_entity(lvlDownLimit, "limit");
    _game.emplace_component<Position>(lvlDownLimit, ((1920 / 2) - ((225 / 2) * 8.3)), (1080 - 120));
    _game.emplace_component<Scale>(lvlDownLimit, 8.3, 0.5);
    //_game.emplace_component<Velocity>(lvlDownLimit, 0, 0, 0);
    _game.emplace_component<Collider>(lvlDownLimit, RectangleCollider{.width = 225, .height = 100});

    auto lvlUpLimit = _game.spawn_entity();
    _game.add_entity_to_scene(lvlUpLimit, "default");
    _game.tag_entity(lvlUpLimit, "limit");
    _game.emplace_component<Position>(lvlUpLimit, ((1920 / 2) - ((225 / 2) * 8.3)), -100);
    //_game.emplace_component<Velocity>(lvlUpLimit, 0, 0, 0);
    _game.emplace_component<Scale>(lvlUpLimit, 8.3, 1);
    _game.emplace_component<Collider>(lvlUpLimit, RectangleCollider{.width = 225, .height = 100});
}

void EcsServer::Start()
{
    while (_isPlaying) {
        _game.run_scene_systems();
        _game.handle_scenes();
        //check les .so nouveau
    }
}

Network::UdpSessionMode EcsServer::GetSessionMode() const {
    return _sessionMode;
}

void EcsServer::SetSessionMode(Network::UdpSessionMode sessionMode) {
    _sessionMode = sessionMode;
}

Network::UdpSessionRole EcsServer::AddClient(uint32_t clientId) {
    // TODO : GERER LE SPECTATE MODE
    if (_clientList.empty()) {
        _clientList.insert({clientId, {Network::EndpointStatus::NONE, {}, Network::UdpSessionRole::ADMIN, true, false, 0}});
        return (Network::UdpSessionRole::ADMIN);
    } else {
        _clientList.insert({clientId, {Network::EndpointStatus::NONE, {}, Network::UdpSessionRole::GUEST, true, false, 0}});
        return (Network::UdpSessionRole::GUEST);
    }
}

void EcsServer::RemoveClient(uint32_t clientId) {
    auto client = std::find_if(_clientList.begin(), _clientList.end(),
                               [&clientId](const std::pair<uint32_t, NetworkServer::ClientData> &e) {
                                   return (e.first == clientId);
                               });
    if (client != _clientList.end()) {
        _clientList.at(clientId).isActive = false;
        _clientList.at(clientId).shipUuid = 0;
        std::cout << "[UDP Session " << _sessionId << "] : Find client " << clientId << " and erase it." << std::endl;
    } else {
        std::cout << "[UDP Session " << _sessionId << "] : Dont find client to remove from udp session." << std::endl;
    }
}

void EcsServer::RemoveAllClient()
{
    _clientList.clear();
}

void EcsServer::Update()
{
    /*while (std::cin) { // -> remplacer par _isPlaying
        if (_safeQueue.size() > 0) {
        }
    }*/
}

void EcsServer::InitCallBack() {
    _isPlaying = true;
    _threadGroup = std::thread([this]() {
        Start();
    });
}

bool EcsServer::IsSessionFull() {
    if (_clientList.size() < _maxNbPlayer)
        return false;
    return true;
}

bool EcsServer::IsSessionEmpty() {
    for (auto &it : _clientList) {
        if (it.second.isActive)
            return false;
    }
    return true;
}

Network::Digit EcsServer::GetSessionPort() const
{
    return (_port);
}

void EcsServer::SetClientReadyState(uint32_t clientId, bool isReady)
{
    auto client = std::find_if(_clientList.begin(), _clientList.end(),
                               [&clientId](const std::pair<uint32_t, NetworkServer::ClientData> &e) {
                                   return (e.first == clientId);
                               });
    if (client != _clientList.end()) {
        std::cout << "[UDP Session " << _sessionId << "] : Say that client " << clientId << " is ready to play." << std::endl;
        _clientList.at(clientId).isReadyToPlay = isReady;
    }
}

bool EcsServer::IsSessionReadyToPlay()
{
    int i = 0;
    for (auto &it : _clientList) {
        i++;
        if (!it.second.isReadyToPlay) {
            std::cout << "CLIENT " << it.first << " IS NOT READY" << std::endl;
            return false;
        }
    }
    return (i >= 1 ? true : false);
}

Network::Digit EcsServer::GetNbPlayerMax() const {
    return _maxNbPlayer;
}

Network::Digit EcsServer::GetNbPlayer() {
    return _clientList.size();
}

Network::Digit EcsServer::GetNbPlayerRdy() {
    Network::Digit playerRdy = 0;
    for (auto &it : _clientList) {
        if (it.second.isReadyToPlay)
            playerRdy += 1;
    }
    return playerRdy;
}

const std::map<uint32_t, NetworkServer::ClientData> &EcsServer::GetClientList() const {
    return _clientList;
}

void EcsServer::Stop()
{

    _isPlaying = false;
    if (_threadGroup.joinable())
        _threadGroup.join();
    _udpSession->Stop();
    _udpSession.release();
}

Network::Protocol::UnpackDataUdp &EcsServer::GetDataUdp() {
    return _dataUdp;
}

EntityLoad &EcsServer::GetLoader() {
    return _loader;
}

Network::PairQueue<Network::Digit, unsigned char *> &EcsServer::GetSafeQueue()
{
    return _safeQueue;
}

void EcsServer::SetClientSpaceshipUuid(uint32_t clientId, std::size_t shipId)
{
    _clientList.at(clientId).shipUuid = shipId;
}

std::size_t EcsServer::GetClientSpaceshipUuid(uint32_t clientId) const
{
    return _clientList.at(clientId).shipUuid;
}


std::mutex &EcsServer::GetMutexKillEntity()
{
    _mutexKillEntity;
}