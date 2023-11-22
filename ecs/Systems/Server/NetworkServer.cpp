#include "Registry.hpp"
#include "sparseArray.hpp"

#include "NtwServer.hpp"
#include "ThreadPair.hpp"

///////////////// RECEIVE /////////////////
static Network::Protocol::CodeUdp checkTypeMsg(std::pair<size_t, unsigned char *> msgPair)
{
    Network::Protocol::CodeUdp type{0};
    Network::Protocol::Manager::unPack(msgPair.second, "d", &type);
    //std::cout << "---typeMs = " << type << std::endl;
    return type;
}

void receive_system([[maybe_unused]] Registry &r, Network::PairQueue<Network::Digit, unsigned char *> &safeQueue, Network::Protocol::UnpackDataUdp &data)
{
    while (!safeQueue.empty()) {
        std::pair<size_t, unsigned char *> msgPair;
        msgPair = safeQueue.pop();
        Network::Protocol::CodeUdp typeHeader = checkTypeMsg(msgPair);

        if (typeHeader == Network::Protocol::CodeUdp::MovePlayer) {
            delete [] data.list;
            data.list = new std::size_t[3];
            Network::Protocol::Manager::unPack(msgPair.second, "dlu", &data.typeHeader, &data.lenList, data.list);
        } else if (typeHeader == Network::Protocol::CodeUdp::GameEvent) {
            delete [] data.list;
            data.list = new std::size_t[4];
            Network::Protocol::Manager::unPack(msgPair.second, "dlu", &data.typeHeader, &data.lenList, data.list);
        } else if (typeHeader == Network::Protocol::CodeUdp::SpawnEntity) {
            delete [] data.list;
            data.list = new std::size_t[4];
            Network::Protocol::Manager::unPack(msgPair.second, "dlu", &data.typeHeader, &data.lenList, data.list);
        }
    }
}
//////////////// SEND ////////////////
void sendNewPositions(Network::Digit posX, Network::Digit posY, std::size_t entityUuid, std::unique_ptr<NetworkServer::UdpSession> &session)
{
    Network::Protocol::Manager protocol;
    Network::Protocol::UnpackDataUdp newData;

    newData.typeHeader = Network::Protocol::CodeUdp::MovePlayer;
    newData.list = new std::size_t[3];
    newData.list[0] = posX;//sizeX la ou tu estime qu'il va
    newData.list[1] = posY;//sizeY la ou tu estime qu'il va
    newData.list[2] = entityUuid;//entity_t qui a envoyé les pos
    newData.lenList = sizeof(size_t) * 3;
    protocol.pack("dlu", newData.typeHeader, newData.lenList, newData.list);
    //std::cout << "Server send a msg !!!" << std::endl << "PosX = " << newData.list[1] << " PosY = " << newData.list[2] << " From uuid : " << newData.list[2] << std::endl << std::endl;
    session->SendToAll(*reinterpret_cast<Network::Protocol::Packet *>(&protocol.p));
    delete [] newData.list;
}

void sendDirectionsEnities(std::unique_ptr<NetworkServer::UdpSession> &session, Network::Protocol::UnpackDataUdp &data)
{
    Network::Protocol::Manager protocol;
    //std::cout << "Server send a msg !!!" << std::endl << "NewVelX = " << data.list[1] << " NewVelY = " << data.list[2] << " From uuid : " << data.list[3] << std::endl;
    protocol.pack("dlu", data.typeHeader, data.lenList, data.list);
    session->SendToAll(*reinterpret_cast<Network::Protocol::Packet *>(&protocol.p));
}

void sendBulletSpawn(std::unique_ptr<NetworkServer::UdpSession> &session, Network::Protocol::UnpackDataUdp &data)
{
    Network::Protocol::Manager protocol;

    //std::cout << "Server send a msg !!!" << std::endl << "Pos bullet X = " << data.list[1] << " Pos bullet Y = " << data.list[2] << " From uuid : " << data.list[3] << std::endl << std::endl;
    protocol.pack("dlu", data.typeHeader, data.lenList, data.list);
    session->SendToAll(*reinterpret_cast<Network::Protocol::Packet *>(&protocol.p));
}//mettre les pos du bullet par rapport au player uuid

void spawnMob(std::unique_ptr<NetworkServer::UdpSession> &session, std::size_t newUuid, [[maybe_unused]]Registry &r, std::string filename) 
{
    Network::Protocol::Manager protocol;
    Network::Protocol::UnpackDataUdp newData;
    std::string msg[16] = {};

    std::size_t ite = 3;
    newData.list = new std::size_t[2];
    newData.list[0] = ite;
    newData.list[1] = newUuid;
    newData.lenList = sizeof(std::size_t) * 2;
    protocol.pack("dluls", Network::Protocol::CodeUdp::SpawnEntity, newData.lenList, newData.list, filename.length(), filename.data());
    session->SendToAll(reinterpret_cast<Network::Protocol::Packet &>(protocol.p));
    delete [] newData.list;
}

void send_system([[maybe_unused]] Registry &r, std::unique_ptr<NetworkServer::UdpSession> &session, MyClock &clock, Network::Protocol::UnpackDataUdp &data, sparseArray<Position> &pos, sparseArray<Uuid> &uuid, [[maybe_unused]]EntityLoad &loader, std::size_t &id)
{
    static float elapsed = 0;
    auto deltaTime = clock.getElapsedTime();

    if (data.typeHeader == Network::Protocol::CodeUdp::GameEvent && data.list[0] == 1)//directions
        sendDirectionsEnities(session, data);
    if (data.typeHeader == Network::Protocol::CodeUdp::SpawnEntity && (data.list[0] == 1 || data.list[0] == 2))//spawn (1 = bullet)
        sendBulletSpawn(session, data);

    for (const auto &spawnIt : r.get_tagged_entities("spawners")) {
        auto &spawn = r.get_components<Spawner>()[spawnIt];
        if (spawn->timeElapsed < 0.01) {
            r.emplace_component<Uuid>(loader.getLastSpawn().second, id);
            spawnMob(session, id, r, loader.getLastSpawn().first);
            id++;
            spawn->timeElapsed = 0.01;
        }
    }

    elapsed += deltaTime;
    if (elapsed >= 0.25) {
        auto lastPlayer = r.get_tagged_entities("players").end();
        auto itePlayers = r.get_tagged_entities("players").begin();

        [[maybe_unused]]auto lastEnemies = r.get_tagged_entities("enemies").end();
        [[maybe_unused]]auto iteEnemies = r.get_tagged_entities("enemies").begin();
        elapsed = 0;

        for (; itePlayers != lastPlayer; ++itePlayers) {
//            if (!pos[*itePlayers].has_value() || !uuid[*itePlayers].has_value()) {
                sendNewPositions(pos[*itePlayers]->x, pos[*itePlayers]->y, uuid[*itePlayers]->uuid, session);
                usleep(50);
//            }
        }/*
        for (const auto &enemies : r.get_tagged_entities("enemies")) {
            sendNewPositions(pos[enemies]->x, pos[enemies]->y, uuid[enemies]->uuid, session);
            usleep(50);
        }*/
    }
}