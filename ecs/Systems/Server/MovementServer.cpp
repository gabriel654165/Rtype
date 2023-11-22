#include "Registry.hpp"
#include "sparseArray.hpp"
#include "NtwServer.hpp"

//add velocity by inputs of players
void event_system_server([[maybe_unused]]Registry &r, Network::Protocol::UnpackDataUdp &data, sparseArray<Velocity> &vel, sparseArray<Uuid> &uuid)
{

    if (data.lenList == 0)
        return;

    auto iteUuid = uuid.begin();
    for (; iteUuid != uuid.end(); ++iteUuid) {
        if (!iteUuid->has_value())
            continue;
        //std::cout << std::endl << "!(data.lenList = " << data.lenList << " >= " << (std::size_t)(sizeof(size_t) * 3) << ")" << std::endl << std::endl;
        if (!(data.lenList >= (std::size_t)(sizeof(size_t) * 3)))
            return;
        //std::cout << "-------------DATA IN = " << data.list[3] << std::endl;// c ca
        if ((*iteUuid)->uuid == (std::size_t)(data.list[3]))//mettre un enum pour le GameEvent 3
            break;
    }

    auto entityIdx = std::distance(uuid.begin(), iteUuid);

    if (data.typeHeader == Network::Protocol::CodeUdp::GameEvent) {
        if (data.list[0] == 1) {// 0 -> input event
            if (data.list[1] == 0) {
                vel[entityIdx]->x = 0;
            } else if (data.list[1] == 1) {
                vel[entityIdx]->x = vel[entityIdx]->speed;
            } else if (data.list[1] == 2) {
                vel[entityIdx]->x = (-vel[entityIdx]->speed);
            }
            // update y - by client GameEvent
            if (data.list[2] == 0) {
                vel[entityIdx]->y = 0;
            } else if (data.list[2] == 1) {
                vel[entityIdx]->y = vel[entityIdx]->speed;
            } else if (data.list[2] == 2) {
                vel[entityIdx]->y = (-vel[entityIdx]->speed);
            }
        }
    }
}

//move objects by velocity
void movement_system_server([[maybe_unused]]Registry &r, MyClock &clock, sparseArray<Position> &ctrlPos, sparseArray<Velocity> &ctrlVel)
{
    auto itePos = ctrlPos.begin();
    auto iteVel = ctrlVel.begin();
    auto deltaTime = clock.getElapsedTime();
    //auto speedCamera = 150;

    //std::cout << "DeltaTime movement=" << deltaTime << std::endl;

    for (; itePos != ctrlPos.end() && iteVel != ctrlVel.end() ; ++iteVel, ++itePos) {
        if (!(*itePos).has_value())
            continue;
        if ((*iteVel).has_value()) {
            //(*itePos)->x += ((*iteVel)->x + speedCamera) * deltaTime;
            (*itePos)->x += (*iteVel)->x * deltaTime;
            (*itePos)->y += (*iteVel)->y * deltaTime;
        }
    }
}