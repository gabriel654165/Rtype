#include "Registry.hpp"
#include "sparseArray.hpp"
#include "NtwServer.hpp"

//add velocity by inputs of players
void event_system_server([[maybe_unused]]Registry &r, Network::Protocol::UnpackDataUdp &data, sparseArray<Velocity> &vel)
{
    if (data.typeHeader == Network::Protocol::CodeUdp::GameEvent) {
        if (data.list[0] == 1) {// 0 -> input event
            if (data.list[1] == 0) {
//                std::cout << "BOUGE PAS EN X" << std::endl;
                vel[data.list[3]]->x = 0;
            } else if (data.list[1] == 1) {
//                std::cout << "DROITE" << std::endl;
                vel[data.list[3]]->x = vel[data.list[3]]->speed;
            } else if (data.list[1] == 2) {
//                std::cout << "GAUCHE" << std::endl;
                vel[data.list[3]]->x = (-vel[data.list[3]]->speed);
            }
            if (data.list[2] == 0) {
                vel[data.list[3]]->y = 0;
//                std::cout << "BOUGE PAS EN Y" << std::endl;
            } else if (data.list[2] == 1) {
//                std::cout << "HAUT" << std::endl;
                vel[data.list[3]]->y = vel[data.list[3]]->speed;
            } else if (data.list[2] == 2) {
//                std::cout << "BAS" << std::endl;
                vel[data.list[3]]->y = (-vel[data.list[3]]->speed);
            }
        }
    }
}

//move objects by velocity
void movement_system_server([[maybe_unused]]Registry &r, double deltaTime, sparseArray<Position> &ctrlPos, sparseArray<Velocity> &ctrlVel)
{
//    std::cout << "DELTA TIME = " << std::endl;
    auto itePos = ctrlPos.begin();
    auto iteVel = ctrlVel.begin();

    for (; itePos != ctrlPos.end() && iteVel != ctrlVel.end() ; ++iteVel, ++itePos) {
        if (!(*itePos).has_value())
            continue;
        if ((*iteVel).has_value()) {
            (*itePos)->x += (*iteVel)->x * 0.5f;//deltaTime;
            (*itePos)->y += (*iteVel)->y * 0.5f;//deltaTime;
            //if ((*iteVel)->x != 0 && (*iteVel)->y != 0)
             //   std::cout << "VELOCITY WHEN CALCULATING : X -> " << (*iteVel)->x << " Y -> " << (*iteVel)->y << std::endl;
            //std::cout << "POS AFTER CALCUL : x -> " << (*itePos)->x << " y -> " << (*itePos)->y << std::endl;
        }
    }
}