#include "Registry.hpp"
#include "Graphic/Graphics.hpp"
#include "sparseArray.hpp"
#include "EntityLoad.hpp"

#include "NtwClient.hpp"

///////////// SEND //////////////
void network_system_send([[maybe_unused]]Registry &r, [[maybe_unused]] Resource::Graphics &rsc, [[maybe_unused]]std::list<std::pair<Network::Protocol::UnpackDataUdp, bool>> &inputHistory, Resource::NtwClient &client, sparseArray<Position> &pos, [[maybe_unused]] sparseArray<Velocity> &vel, sparseArray<Spawner> &spawnArr, sparseArray<Uuid> &uuid, sparseArray<Controllable> &ctrlArr)
{
    auto player = *r.get_tagged_entities("player").begin();
    const auto &keys = ctrlArr[player]->keysPressed;

    auto uuidEntity = uuid[player]->uuid;

    Network::Protocol::Manager protocol;
    Network::Protocol::UnpackDataUdp dataToSend;
    if (keys[sf::Keyboard::Space] && !spawnArr[player]->active) {
        //auto bulletIdx = *r.get_tagged_entities("bullet").end();//le dernier bullet tiré
        dataToSend.typeHeader = Network::Protocol::CodeUdp::SpawnEntity;
        dataToSend.lenList = sizeof(std::size_t) * 4;
        dataToSend.list = new std::size_t[4];
        dataToSend.list[0] = 1;//Spawn bullet               //FAIRE UN ENUM DANS MANAGER.HPP
        dataToSend.list[1] = pos[player]->x;//pos player
        dataToSend.list[2] = pos[player]->y;//pos player
        dataToSend.list[3] = uuidEntity;//player qui a send
        //std::cout << "CLient send msg !!! pos bullet : x -> " << dataToSend.list[0] << " y -> " << dataToSend.list[1] << " msg spawn type = " << dataToSend.list[0] << " uuid player = " << dataToSend.list[3] << std::endl << std::endl;
        protocol.pack("dlu", dataToSend.typeHeader, dataToSend.lenList, dataToSend.list);
        client.send(protocol.p, client.getEnpoint());
        delete [] dataToSend.list;
    } else if (spawnArr[player]->active) {
        //auto bulletIdx = *r.get_tagged_entities("bullet").end();//le dernier bullet tiré
        dataToSend.typeHeader = Network::Protocol::CodeUdp::SpawnEntity;
        dataToSend.lenList = sizeof(std::size_t) * 4;
        dataToSend.list = new std::size_t[4];
        dataToSend.list[0] = 2;//Spawn bullet               //FAIRE UN ENUM DANS MANAGER.HPP
        dataToSend.list[1] = pos[player]->x;//pos player
        dataToSend.list[2] = pos[player]->y;//pos player
        dataToSend.list[3] = uuidEntity;//player qui a send
        //std::cout << "CLient send msg !!! msg spawn type = " << dataToSend.list[0] << " uuid player = " << dataToSend.list[3] << std::endl << std::endl;
        protocol.pack("dlu", dataToSend.typeHeader, dataToSend.lenList, dataToSend.list);
        client.send(protocol.p, client.getEnpoint());
        delete[] dataToSend.list;
    }
        //if le bullet touche send aux gens que tel uuid est mort (monstrer)
}

/////////////// RECEIVE ////////////
static void 
spawnAnimDeath(Registry &r, Resource::Graphics &gr, std::size_t posX, std::size_t posY)
{
    auto animDeath = r.spawn_entity();
    r.add_entity_to_scene(animDeath, "game");
    r.tag_entity(animDeath, "death");
    r.emplace_component<Velocity>(animDeath, 0, 0, 0);
    r.emplace_component<Animations>(animDeath);
    r.emplace_component<Position>(animDeath, posX, posY);
    r.emplace_component<Scale>(animDeath, 2, 2);
    auto &deathTexture = gr.getTexture(Resource::Graphics::Textures::EFFECTS4);
    auto &deathDrawable = r.emplace_component<Sprite>(animDeath);
    deathDrawable->sprite.setTexture(deathTexture);
    deathDrawable->sprite.setTextureRect(sf::IntRect(0, 54, 32, 31));
    auto &deathAnimation = r.get_components<Animations>()[animDeath];
    deathAnimation->animations.emplace("ExplodeAnim", AnimationData{
            .texture = deathTexture,
            .nextFrameOffset = {32, 0},
            .firstFramePosition = sf::Vector2f(0, 54),
            .frames = 7,
            .timeStep = 0.1
    });
    deathAnimation->currentAnimation = deathAnimation->animations.begin();
    deathAnimation->active = true;
    deathAnimation->currentAnimation->second.currentFrame = 0;
    deathAnimation->loop = false;
    
    auto audioDeath = r.spawn_entity();
    auto &audioSource = r.emplace_component<AudioSource>(audioDeath);
    audioSource->sound.setBuffer(gr.getSound(Resource::Graphics::Sounds::BOOM));
    audioSource->sound.play();

    //active false
    //une fois l'animation finie enlever le composant sprite & aniamtion du bail
    //apres x temps que le player soit mort
}

bool searchByUuid(Registry::entity_t &res, Network::Protocol::UnpackDataUdp &data, std::size_t posUuid, std::size_t sizeToCheck, sparseArray<Uuid> &uuidArr)
{
    auto iteUuid = uuidArr.begin();

    for (; iteUuid != uuidArr.end(); ++iteUuid) {
        if (!iteUuid->has_value())
            continue;
        if (!(data.lenList >= (std::size_t)(sizeof(size_t) * 2)))
            return false;
        if ((*iteUuid)->uuid == (std::size_t)(data.list[posUuid]))
            break;
    }
    if ((*iteUuid)->uuid == 0)//uuid start at 1
        return false;
    
    res = std::distance(uuidArr.begin(), iteUuid);
    return true;
}

static void posEntity(Network::Protocol::UnpackDataUdp &data, sparseArray<Position> &pos, sparseArray<Scale> &scale, sparseArray<Sprite> &sprite, sparseArray<Uuid> &uuid)
{
    std::size_t entityIdx = 0;

    if (!searchByUuid(entityIdx, data, 2, (sizeof(size_t) * 2), uuid))//c sizeof * 3 normalement
            return;
    sprite[entityIdx]->sprite.setScale(scale[entityIdx]->x, scale[entityIdx]->y);
    pos[entityIdx]->x = (float)data.list[0];
    pos[entityIdx]->y = (float)data.list[1];
    //std::cout << "Client move to x : " << data.list[0] << " y : " << data.list[1] << " from uuid : " << data.list[2] << " from entityIdx : " << entityIdx << std::endl << std::endl;
    sprite[entityIdx]->sprite.setPosition(pos[entityIdx]->x , pos[entityIdx]->y);
}

static void spawnEntity(Registry &r, Resource::Graphics &gr, Network::Protocol::UnpackDataUdp &data, sparseArray<Spawner> &spawn, sparseArray<Uuid> &uuid, sparseArray<Controllable> &ctrlArr, EntityLoad &loader, std::string buff)
{
    std::size_t entityIdx = 0;

    if (data.list[0] == 1) {
        if (!searchByUuid(entityIdx, data, 3, (sizeof(size_t) * 4), uuid))
            return;
        spawn[entityIdx]->active = true;
        //std::cout << "Client receive msg !!! UUID player who shoot = " << data.list[3] << " and enity IDX = " << entityIdx << std::endl;
    } else if (data.list[0] == 2) {
        if (!searchByUuid(entityIdx, data, 3, (sizeof(size_t) * 4), uuid))
            return;
        spawn[entityIdx]->active = false;
        //std::cout << "Client receive msg !!! UUID player who shoot = " << data.list[3] << " and enity IDX = " << entityIdx << std::endl;
    }
    if (data.list[0] == 3) {
        auto id = loader.spawnEntity(buff, r, gr);
        r.emplace_component<Uuid>(id, data.list[1]);
    } else if (data.list[0] == 4) {
        if (!searchByUuid(entityIdx, data, 3, (sizeof(size_t) * 4), uuid))
            return;
        spawnAnimDeath(r, gr, data.list[1], data.list[2]);
        //std::cout << "-----------Client receive msg !!! x = " << data.list[1] << " y = " << data.list[2] << " uuid = " << data.list[3] << std::endl;
        
        //MUTEX ?
        r.kill_entity(entityIdx);
    }
}//FAIRE UN ENUM DANS MANAGER.HPP

static void directionGameEvent(Registry &r, Network::Protocol::UnpackDataUdp &data, sparseArray<Velocity> &vel, sparseArray<Uuid> &uuid)
{
    //chercher l'entity_t par rapport a l'uuid
    [[unusued]]auto player = *(r.get_tagged_entities("player").begin());
    auto iteUuid = uuid.begin();

    if (!(data.lenList >= (std::size_t)(sizeof(size_t) * 3)))
        return;
    //if (uuid[player]->uuid == (std::size_t)(data.list[3]))
    //    return;
    for (; iteUuid != uuid.end(); ++iteUuid) {
        if (!iteUuid->has_value())
            continue;
        if ((*iteUuid)->uuid == (std::size_t)(data.list[3]))            //mettre un enum pour le '3' truc::UdpUuid MANAGER.HPP
            break;
    }
    if ((*iteUuid)->uuid == 0)//uuid start at 1
        return;

    auto entityIdx = std::distance(uuid.begin(), iteUuid);

    if (data.list[0] == 1) {
        if (data.list[1] == 0)
            vel[entityIdx]->x = 0;
        else if (data.list[1] == 1)
            vel[entityIdx]->x = vel[entityIdx]->speed;
        else if (data.list[1] == 2)
            vel[entityIdx]->x = (-vel[entityIdx]->speed);
        // update y - by client GameEvent
        if (data.list[2] == 0)
            vel[entityIdx]->y = 0;
        else if (data.list[2] == 1)
            vel[entityIdx]->y = vel[entityIdx]->speed;
        else if (data.list[2] == 2)
            vel[entityIdx]->y = (-vel[entityIdx]->speed);

        //std::cout << "NewVelX : " << vel[entityIdx]->x << " NewVelY : " << vel[entityIdx]->y << " from : " << data.list[3] << std::endl << std::endl;
    }
}

static Network::Protocol::CodeUdp checkTypeMsg(unsigned char * dataReceived)
{
    Network::Protocol::CodeUdp type{0};
    Network::Protocol::Manager::unPack(dataReceived, "d", &type);
    //std::cout << "---typeMs = " << type << std::endl;
    return type;
}

void network_system_receive([[maybe_unused]]Registry &r, Resource::Graphics &gr, Network::LockedQueue<unsigned char *> &safeQueue, Resource::NtwClient &client, sparseArray<Position> &pos, sparseArray<Velocity> &vel, sparseArray<Scale> &scale, sparseArray<Sprite> &sprite, sparseArray<Spawner> &spawn, sparseArray<Uuid> &uuid, sparseArray<Controllable> &ctrl, EntityLoad &loader)
{
    if (!client.getClient().IsUdpClientStart())
        return;
    while (!safeQueue.empty()) {
        unsigned char *dataReceived = safeQueue.pop();
        Network::Protocol::CodeUdp typeHeader = checkTypeMsg(dataReceived);
        Network::Protocol::Manager protocol;
        Network::Protocol::UnpackDataUdp data;
        std::size_t size;
        char buff[16] = {};

        //std::cout << "Client receive msg !!!" << std::endl;
        if (typeHeader == Network::Protocol::CodeUdp::MovePlayer) {
            data.list = new std::size_t[3];
            protocol.unPack(dataReceived, "dlu", &data.typeHeader, &data.lenList, data.list);
            posEntity(data, pos, scale, sprite, uuid);
            delete [] data.list;
        } else if (typeHeader == Network::Protocol::CodeUdp::SpawnEntity) {
            data.list = new std::size_t[4];
            protocol.unPack(dataReceived, "dluls", &data.typeHeader, &data.lenList, data.list, &size, buff);
            //spawn bullet || .so monster || kill enity_t
            spawnEntity(r, gr, data, spawn, uuid, ctrl, loader, buff);
            delete [] data.list;
        } else if (typeHeader == Network::Protocol::CodeUdp::GameEvent) {
            data.list = new std::size_t[4];
            protocol.unPack(dataReceived, "dlu", &data.typeHeader, &data.lenList, data.list);
            //if entity x go < > ^ v
            directionGameEvent(r, data, vel, uuid);
            delete [] data.list;
            //entity_t took X bonus / ...
        }
    }
}
