/*
** EPITECH PROJECT, 2021
** CPP-RType
** File description:
** PlayerShips
*/

#include "Registry.hpp"
#include "MyClock.hpp"
#ifdef CLIENT
#else
#include "UdpSession.hpp"
#include "Manager.hpp"
#endif
#include <mutex>

#ifdef CLIENT
#include "Graphics.hpp"

static void animate_ship(
        Velocity &vel,
        Animations &anim
) {
    auto &activeAnim = anim.active;
    auto &animData = anim.currentAnimation->second;

    if (vel.y) {
        bool isGoingUp = (vel.y < 0);
        bool lastFrame = animData.currentFrame == ((animData.frames - 1) * isGoingUp);

        if (!lastFrame)
            animData.nextFrameOffset.x = (isGoingUp ? 1 : -1) * std::abs(animData.nextFrameOffset.x);
        activeAnim = !lastFrame;
    } else if (animData.currentFrame != 2) {
        int beforeIdle = animData.currentFrame < 2;

        activeAnim = true;
        if ((animData.nextFrameOffset.x > 0 && !beforeIdle) ||
            (animData.nextFrameOffset.x < 0 && beforeIdle))
            animData.nextFrameOffset.x = -animData.nextFrameOffset.x;
    } else
        activeAnim = false;

}
#endif

void collision_two_entities(
   Registry::entity_t entity1,
   Registry::entity_t entity2,
   sparseArray<Velocity> &velArr,
   sparseArray<Position> &posArr,
   sparseArray<Collider> &colArr,
   sparseArray<Scale> &scaleArr
)
{
    auto &objVel = velArr[entity1];
    auto objCol = std::any_cast<RectangleCollider>(colArr[entity1]->infos);
    auto &objPos = posArr[entity1];
    Scale objScale = scaleArr[entity1].has_value() ? *scaleArr[entity1] : Scale{1, 1};

    auto otherObjCol = std::any_cast<RectangleCollider>(colArr[entity2]->infos);
    auto &otherObjPos = posArr[entity2];
    Scale otherObjScale = scaleArr[entity2].has_value() ? *scaleArr[entity2] : Scale{1, 1};

    if (objVel->x > 0 
#ifdef CLIENT
     && std::abs((objPos->x + objCol.width * objScale.x )- otherObjPos->x) < 1
#endif
     )
     
        objPos->x = otherObjPos->x - objCol.width * objScale.x;
    else if (objVel->x < 0 
#ifdef CLIENT
    && std::abs(objPos->x -  (otherObjPos->x + otherObjCol.width * otherObjScale.x)) < 1
#endif
    )
        objPos->x = otherObjPos->x + otherObjCol.width * otherObjScale.x;
    if (objVel->y > 0 
#ifdef CLIENT
    && std::abs((objPos->y + objCol.height * objScale.y) - otherObjPos->y) < 1
#endif
    )
        objPos->y = otherObjPos->y - objCol.height * objScale.y;
    else if (objVel->y < 0 
#ifdef CLIENT
    && std::abs(objPos->y - (otherObjPos->y + otherObjCol.height * otherObjScale.y)) < 1
#endif
    )
        objPos->y = otherObjPos->y + otherObjCol.height * otherObjScale.y;

    objVel->y = 0;
    objVel->x = 0;
}

void manageBulletCollisions(
   Registry::entity_t player,
   Registry::entity_t enemy,
   Spawner &enemySpawner,
   std::set<Registry::entity_t> &enemies
)
{
    if (enemies.count(enemy))
        return;
    auto infos = std::any_cast<BulletInfo>(enemySpawner.infos);
    int damage = -infos.damage;
    // faire prendre des dégats et tout
    std::cout << "binks player " << player << "s'est pris une balle (" << damage << ") par " << enemy << std::endl;
}

bool manageWallDeath(
    Registry &r,
    Registry::entity_t player, 
#ifdef CLIENT
    Resource::Graphics &gr,
    sparseArray<Animations> &animArr,
    sparseArray<Sprite> &spriteArr,
    sparseArray<Velocity> &velArr,
#else
    std::mutex &mutexKillEntity,
    std::unique_ptr<NetworkServer::UdpSession> &session,
#endif
    sparseArray<Position> &posArr,
    sparseArray<Spawner> &spawnArr,
    sparseArray<Uuid> &uuid
    )
{
    std::cout << "-----------------------------Collions avec le mur ca va exploser !!!" << std::endl;
#ifdef CLIENT
    velArr[player]->x = 0;
    velArr[player]->y = 0;
    velArr[player]->speed = 0;
#else
    std::size_t posX = (posArr[player]->x <= 0) ? 0 : posArr[player]->x;
    std::size_t posY = (posArr[player]->y <= 0) ? 0 : posArr[player]->y;
    
    Network::Protocol::Manager protocol;
    Network::Protocol::UnpackDataUdp newData;
    newData.typeHeader = Network::Protocol::CodeUdp::SpawnEntity;
    newData.list = new std::size_t[4];
    newData.list[0] = 4;//-> death
    newData.list[1] = posX;
    newData.list[2] = posY;
    newData.list[3] = uuid[player]->uuid;//entity_t est die
    newData.lenList = sizeof(size_t) * 4;
    protocol.pack("dlu", newData.typeHeader, newData.lenList, newData.list);
    std::cout << "------------Server send a msg !!!" << std::endl << "PosX = " << newData.list[1] << " PosY = " << newData.list[2] << " DIED uuid : " << newData.list[3] << std::endl << std::endl;
    session->SendToAll(*reinterpret_cast<Network::Protocol::Packet *>(&protocol.p));
    delete [] newData.list;

    std::unique_lock<std::mutex> lock(mutexKillEntity);
    r.kill_entity(player);
    //send un msg aux client en mode oueil est mort
    //kill entity idx avec l'uuid
#endif
    return true;
}

void players_ship_system(
        [[maybe_unused]] Registry &r,
        sparseArray<Velocity> &velArr,
#ifdef CLIENT
        sparseArray<Animations> &animArr,
        sparseArray<Sprite> &spriteArr,
        Resource::Graphics &rsc,
#else
        MyClock &clock,
        std::mutex &mutex,
        std::unique_ptr<NetworkServer::UdpSession> &session,
#endif
        sparseArray<Spawner> &spawnArr,
        sparseArray<Position> &posArr,
        sparseArray<Collider> &colArr,
        sparseArray<Scale> &scaleArr,
        sparseArray<Parent> &parArr,
        sparseArray<Follow> &followArr,
        sparseArray<Uuid> &uuid
//        sparseArray<BulletInfo> &bonusArr
) {
    auto players = r.get_tagged_entities("players");
    auto &bullets = r.get_tagged_entities("bullets");
    auto enemies = r.get_tagged_entities("enemies");
    auto wall = r.get_tagged_entities("limit");
    auto wallLevel = r.get_tagged_entities("level");
    auto &bonuses = r.get_components<BeamBonusInfo>();
    bool hasDie = false;

    for (auto &player: players) {
        //if (!posArr[player].has_value() || !velArr[player].has_value() || !colArr[player].has_value() || !scaleArr[player].has_value())
        //    continue;
        auto &spawner = spawnArr[player];

#ifdef CLIENT
        animate_ship(velArr[player].value(),
                     animArr[player].value());
#endif
        spawner->position = spawner->offset + sf::Vector2f{posArr[player]->x, posArr[player]->y};

        if (!colArr[player]->hits.empty()) {
            for (auto entity: colArr[player]->hits) {
                if (bullets.count(entity)) {
                    manageBulletCollisions(player, parArr[entity]->parent, *spawnArr[parArr[entity]->parent], enemies);
                } else if (wall.count(entity) || wallLevel.count(entity)) {
                    if (manageWallDeath(
#ifdef CLIENT
                        r, player, rsc, animArr, spriteArr, velArr, posArr, spawnArr, uuid)) {
                            r.untag_entity(player, "players");
                            //r.untag_entity(player, "player");
                            return;
                        }
#else
                        r, player, mutex, session, posArr, spawnArr, uuid)) {
                            players.erase(player);
                            return;
                        }
#endif
                
                } else if (bonuses[entity].has_value() && followArr[entity].has_value()) {
                    if (followArr[entity]->active)
                        continue;
                    followArr[entity]->following = player;
                    followArr[entity]->active = true;
                } else // normal collisions
                    collision_two_entities(player, entity, velArr, posArr, colArr, scaleArr);
            }
        }
        [[maybe_unused]]auto pos = posArr.begin();
        [[maybe_unused]]auto vel = velArr.begin();
#ifdef CLIENT
        auto deltaTime = rsc.getDeltaTime();
#else
        auto deltaTime = clock.getElapsedTime();
#endif

        //pour qu'il dépasse pas les bord x de la map
        for (auto &id: players) {
            if (!posArr[id].has_value() || !velArr[id].has_value() || !colArr[id].has_value() || !scaleArr[id].has_value())
                continue;
            if ((posArr[id]->x + velArr[id]->x * deltaTime) < 0) {
                posArr[id]->x = 0;
                velArr[id]->x = 0;
            }
            if (!posArr[player].has_value() || !velArr[player].has_value() || !colArr[player].has_value() || !scaleArr[player].has_value())
                continue;
            if (((posArr[player]->x + velArr[player]->x * deltaTime) + 
                (std::any_cast<RectangleCollider>(colArr[player]->infos).width * scaleArr[player]->x )) 
                > 1920) {
                posArr[id]->x = 1920 - (std::any_cast<RectangleCollider>(colArr[id]->infos).width * scaleArr[id]->x);
                velArr[id]->x = 0;
            }
        }

    }

}
