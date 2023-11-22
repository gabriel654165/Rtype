#include "Registry.hpp"
#include "Graphic/Graphics.hpp"
#include "NtwClient.hpp"

//void send_event_system([[maybe_unused]] Registry &r, udp::endpoint endPoint, std::list<std::pair<Network::Protocol::UnpackDataUdp, bool>> &inputHistory, Resource::NtwClient &client, sf::Event events, sf::RenderWindow &window, Resource::Graphics &resources, sparseArray<Controllable> &pContr, sparseArray<Velocity> &velArr)
void send_event_system([[maybe_unused]] Registry &r, Resource::NtwClient &client, Resource::Graphics &resources, sparseArray<Controllable> &pContr, sparseArray<Uuid> &uuid)
{
    auto player = *r.get_tagged_entities("player").begin();
    auto uuidPlayer = uuid[player]->uuid;
    double deltaTime = resources.getDeltaTime();
    static float elapsed = 0;

    elapsed += deltaTime;
    //send events of player as udp doc (Readme.md)
    if (elapsed > 0.25 && client.getClient().IsUdpClientStart() && pContr[player].has_value()) {
        elapsed = 0;
        Network::Protocol::Manager protocol;
        Network::Protocol::UnpackDataUdp dataToSend;
        dataToSend.typeHeader = Network::Protocol::CodeUdp::GameEvent;
        dataToSend.lenList = (sizeof(size_t) * 4);
        dataToSend.list = new std::size_t[4];
        dataToSend.list[0] = 1;

        dataToSend.list[1] = 0;
        dataToSend.list[2] = 0;
        if (pContr[player]->keysPressed[sf::Keyboard::Right])
            dataToSend.list[1] = 1;
        else if (pContr[player]->keysPressed[sf::Keyboard::Left])
            dataToSend.list[1] = 2;
        if (pContr[player]->keysPressed[sf::Keyboard::Down])
            dataToSend.list[2] = 1;
        else if (pContr[player]->keysPressed[sf::Keyboard::Up])
            dataToSend.list[2] = 2;
        dataToSend.list[3] = uuidPlayer;
        //std::cout << " Client send message !!! NewVelX = " << dataToSend.list[1] << " NewVelY = " << dataToSend.list[2] << " from : " << dataToSend.list[3] << std::endl << std::endl;
        protocol.pack("dlu", dataToSend.typeHeader, dataToSend.lenList, dataToSend.list);
        client.send(protocol.p, client.getEnpoint());
        delete [] dataToSend.list;

        /*
        std::pair<Network::Protocol::UnpackDataUdp, bool> pair;
        pair.first = dataToSend;
        pair.second = false;
        inputHistory.push_back(pair);
        */
    }
}