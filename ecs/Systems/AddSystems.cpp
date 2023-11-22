#include "Registry.hpp"
#include "Graphic/Graphics.hpp"
#include "NtwClient.hpp"
#include "EntityLoad.hpp"

void network_system_send([[maybe_unused]]Registry &r, Resource::Graphics &rsc, [[maybe_unused]]std::list<std::pair<Network::Protocol::UnpackDataUdp, bool >>  &inputHistory, Resource::NtwClient &client, sparseArray<Position> &pos, sparseArray<Velocity> &vel, sparseArray<Spawner> &spawn, sparseArray<Uuid> &uuid, sparseArray<Controllable> &ctrlArr);
void network_system_receive([[maybe_unused]]Registry &r, Resource::Graphics &gr, Network::LockedQueue<unsigned char *> &safeQueue, Resource::NtwClient &client, sparseArray<Position> &pos, sparseArray<Velocity> &vel, sparseArray<Scale> &scale, sparseArray<Sprite> &sprite, sparseArray<Spawner> &spawn, sparseArray<Uuid> &uuid, sparseArray<Controllable> &ctrl, EntityLoad &loader);
void lobby_menu_return_button([[maybe_unused]]Registry &r, Resource::NtwClient &client, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim);
void join_lobby_system([[maybe_unused]]Registry &r, Resource::NtwClient &client, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim, sparseArray<Container<std::size_t>> &container);
void tcp_client_receive_system([[maybe_unused]]Registry &r, Resource::Graphics &rsc, Network::LockedQueue<unsigned char *> &safeQueue, Resource::NtwClient &client);
void animation_ui_system([[maybe_unused]] Registry &r, [[maybe_unused]]Resource::Graphics &resources, sparseArray<Animations> &animArray, sparseArray<Sprite>& spriteArray);
void main_menu_button_system([[maybe_unused]]Registry &r, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim);
void join_lobby_button_system([[maybe_unused]]Registry &r, Resource::NtwClient &client, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim);
void create_lobby_button_system([[maybe_unused]]Registry &r, Resource::NtwClient &client, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim);
void lobby_button_system([[maybe_unused]]Registry &r, Resource::NtwClient &client, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim, sparseArray<Container<bool>> &container);
void animation_system([[maybe_unused]] Registry &r, Resource::Graphics &resources, sparseArray<Animations> &animArray, sparseArray<Sprite>& spriteArray);
void camera_system([[maybe_unused]] Registry &r, [[maybe_unused]]Resource::Graphics &resources);
void paralaxe_system([[maybe_unused]] Registry &r, Resource::Graphics &resources, sparseArray<Position> &arrPos);
void manage_levels([[maybe_unused]] Registry &r, Resource::Graphics &resources, sparseArray<Position> &arrPos, sparseArray<Collider> &arrCol, sparseArray<Velocity> &arrVel);
void draw_system([[maybe_unused]] Registry &r, Resource::Graphics &resources, sparseArray<Sprite> &ctrlDraw, sparseArray<IU> &crtlIU);
void event_system([[maybe_unused]] Registry &r, sf::Event events, sf::RenderWindow &window, sparseArray<Controllable> &pContr);
//void send_event_system([[maybe_unused]] Registry &r, udp::endpoint endPoint, std::list<std::pair<Network::Protocol::UnpackDataUdp, bool>> &inputHistory, Resource::NtwClient &client, sf::Event events, sf::RenderWindow &window, Resource::Graphics &resources, sparseArray<Controllable> &pContr, sparseArray<Velocity> &velArr);
void send_event_system([[maybe_unused]] Registry &r, Resource::NtwClient &client, Resource::Graphics &resources, sparseArray<Controllable> &pContr, sparseArray<Uuid> &uuid);
void movement_system([[maybe_unused]] Registry &r, Resource::Graphics &rsc, sparseArray<Position> &ctrlPos, sparseArray<Velocity> &ctrlVel, sparseArray<Sprite> &spriteArr, sparseArray<Scale> &scaleArr);
void ui_system([[maybe_unused]] Registry &r, [[maybe_unused]] sf::RenderWindow &window, sparseArray<IU> &ctrlIu, sparseArray<Position> &ctrlPos);
void player_system([[maybe_unused]] Registry &r, sparseArray<Velocity> &velArr, sparseArray<Controllable> &ctrlArr, sparseArray<Animations> &animArr, sparseArray<Spawner> &spawnArr, sparseArray<Power> &power);

void players_ship_system(
    [[maybe_unused]] Registry &r,
    sparseArray<Velocity> &velArr,
    sparseArray<Animations> &animArr,
    sparseArray<Sprite> &spriteArr,
    Resource::Graphics &gr,
    sparseArray<Spawner> &spawnArr,
    sparseArray<Position> &posArr,
    sparseArray<Collider> &colArr,
    sparseArray<Scale> &,
    sparseArray<Parent> &,
    sparseArray<Follow> &,
    sparseArray<Uuid> &
);

void collision_system(
        [[maybe_unused]] Registry &r,
        Resource::Graphics &rsc,
        sparseArray<Position> &posArr,
        sparseArray<Collider> &colArr,
        sparseArray<Scale> &scaleArr,
        sparseArray<Velocity> &velArr
        );

void spawner_system(
    [[maybe_unused]] Registry &r,
    Resource::Graphics &rsc,
    sparseArray<Spawner> &spawnArr,
    EntityLoad &loader
);

void follow_system([[maybe_unused]] Registry &r, sparseArray<Follow> &followArr, sparseArray<Position> &posArr, sparseArray<Velocity> &velArr);

void reset_events_system([[maybe_unused]] Registry &r, sparseArray<Uuid> &uuid, sparseArray<Spawner> &spawn);

void addMenuLobbySystems(Registry &game, Resource::Graphics &resources, Resource::NtwClient &client)
{
    game.add_system_to_current_scene<>(
            [&resources, &safeQueue = client.getClient().GetTcpSafeQueue(), &client]
                    (Registry &r)
            { tcp_client_receive_system(r, resources, safeQueue, client); }
    );
    game.add_system_to_current_scene<Controllable>(
            [&resources]
                    (Registry &r, sparseArray<Controllable> &ctrlArr)
            { event_system(r, resources.getEvents(), resources.getWindow(), ctrlArr); }
    );
    game.add_system_to_current_scene<Position, Velocity, Scale, Sprite>(
            [&resources]
                    (Registry &r, sparseArray<Position> &ctrlPos, sparseArray<Velocity> &ctrlVel, sparseArray<Scale> &scaleArr, sparseArray<Sprite> &ctrlDraw)
            { movement_system(r, resources, ctrlPos, ctrlVel, ctrlDraw, scaleArr); }
    );
    game.add_system_to_current_scene<IU, Position>(
            [&window = resources.getWindow()]
            (Registry &r, sparseArray<IU> &ctrlIu, sparseArray<Position> &ctrlPos)
            { ui_system(r, window, ctrlIu, ctrlPos); }
    );
    game.add_system_to_current_scene<Position, Collider, Controllable, Animations, Container<bool>>(
            [&client]
                    (Registry &r, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim, sparseArray<Container<bool>> &container)
            { lobby_button_system(r, client, pos, cldr, ctrl, anim, container); }
    );
    game.add_system_to_current_scene<Position, Collider, Controllable, Animations>(
            [&client]
                    (Registry &r, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim)
            { lobby_menu_return_button(r, client, pos, cldr, ctrl, anim); }
    );
    game.add_system_to_current_scene<Animations, Sprite>(
            [&resources]
                    (Registry &r, sparseArray<Animations> &ctrlPos, sparseArray<Sprite> &drawDraw)
            { animation_ui_system(r, resources, ctrlPos, drawDraw); }
    );
    game.add_system_to_current_scene<Sprite, IU>(
            [&resources]
                    (Registry &r, sparseArray<Sprite> &drawDraw, sparseArray<IU> &crtlIu)
            { draw_system(r, resources, drawDraw, crtlIu); }
    );
}

void addMenuJoinLobbySystems(Registry &game, Resource::Graphics &resources, Resource::NtwClient &client)
{
    game.add_system_to_current_scene<>(
            [&resources, &safeQueue = client.getClient().GetTcpSafeQueue(), &client]
                    (Registry &r)
            { tcp_client_receive_system(r, resources, safeQueue, client); }
    );
    game.add_system_to_current_scene<Controllable>(
            [&resources]
                    (Registry &r, sparseArray<Controllable> &ctrlArr)
            { event_system(r, resources.getEvents(), resources.getWindow(), ctrlArr); }
    );
    game.add_system_to_current_scene<Position, Velocity, Scale, Sprite>(
            [&resources]
                    (Registry &r, sparseArray<Position> &ctrlPos, sparseArray<Velocity> &ctrlVel, sparseArray<Scale> &scaleArr, sparseArray<Sprite> &ctrlDraw)
            { movement_system(r, resources, ctrlPos, ctrlVel, ctrlDraw, scaleArr); }
    );
    game.add_system_to_current_scene<IU, Position>(
            [&window = resources.getWindow()]
                    (Registry &r, sparseArray<IU> &ctrlIu, sparseArray<Position> &ctrlPos)
            { ui_system(r, window, ctrlIu, ctrlPos); }
    );
    game.add_system_to_current_scene<Position, Collider, Controllable, Animations>(
            [&client]
                    (Registry &r, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim)
            { join_lobby_button_system(r, client, pos, cldr, ctrl, anim); }
    );
    game.add_system_to_current_scene<Position, Collider, Controllable, Animations, Container<std::size_t>>(
            [&client]
                    (Registry &r, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim, sparseArray<Container<std::size_t>> &container)
            { join_lobby_system(r, client, pos, cldr, ctrl, anim, container); }
    );
    game.add_system_to_current_scene<Animations, Sprite>(
            [&resources]
                    (Registry &r, sparseArray<Animations> &ctrlPos, sparseArray<Sprite> &drawDraw)
            { animation_ui_system(r, resources, ctrlPos, drawDraw); }
    );
    game.add_system_to_current_scene<Sprite, IU>(
            [&resources]
                    (Registry &r, sparseArray<Sprite> &drawDraw, sparseArray<IU> &crtlIu)
            { draw_system(r, resources, drawDraw, crtlIu); }
    );
}

void addMenuCreateLobbySystems(Registry &game, Resource::Graphics &resources, Resource::NtwClient &client)
{
    game.add_system_to_current_scene<>(
            [&resources, &safeQueue = client.getClient().GetTcpSafeQueue(), &client]
                    (Registry &r)
            { tcp_client_receive_system(r, resources, safeQueue, client); }
    );
    game.add_system_to_current_scene<Controllable>(
            [&resources]
                    (Registry &r, sparseArray<Controllable> &ctrlArr)
            { event_system(r, resources.getEvents(), resources.getWindow(), ctrlArr); }
    );
    game.add_system_to_current_scene<Position, Velocity, Scale, Sprite>(
            [&resources]
                    (Registry &r, sparseArray<Position> &ctrlPos, sparseArray<Velocity> &ctrlVel, sparseArray<Scale> &scaleArr, sparseArray<Sprite> &ctrlDraw)
            { movement_system(r, resources, ctrlPos, ctrlVel, ctrlDraw, scaleArr); }
    );
    game.add_system_to_current_scene<IU, Position>(
            [&window = resources.getWindow()]
                    (Registry &r, sparseArray<IU> &ctrlIu, sparseArray<Position> &ctrlPos)
            { ui_system(r, window, ctrlIu, ctrlPos); }
    );
    game.add_system_to_current_scene<Position, Collider, Controllable, Animations>(
            [&client]
                    (Registry &r, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim)
            { create_lobby_button_system(r, client, pos, cldr, ctrl, anim); }
    );
    game.add_system_to_current_scene<Animations, Sprite>(
            [&resources]
                    (Registry &r, sparseArray<Animations> &ctrlPos, sparseArray<Sprite> &drawDraw)
            { animation_ui_system(r, resources, ctrlPos, drawDraw); }
    );
    game.add_system_to_current_scene<Sprite, IU>(
            [&resources]
                    (Registry &r, sparseArray<Sprite> &drawDraw, sparseArray<IU> &crtlIu)
            { draw_system(r, resources, drawDraw, crtlIu); }
    );
}

void addMainMenuSystems(Registry &game, Resource::Graphics &resources, Resource::NtwClient &client)
{
    game.add_system_to_current_scene<>(
            [&resources, &safeQueue = client.getClient().GetTcpSafeQueue(), &client]
                    (Registry &r)
            { tcp_client_receive_system(r, resources, safeQueue, client); }
    );
    game.add_system_to_current_scene<Controllable>(
            [&resources]
                    (Registry &r, sparseArray<Controllable> &ctrlArr)
            { event_system(r, resources.getEvents(), resources.getWindow(), ctrlArr); }
    );
    game.add_system_to_current_scene<Position, Velocity, Scale, Sprite>(
            [&resources]
                    (Registry &r, sparseArray<Position> &ctrlPos, sparseArray<Velocity> &ctrlVel, sparseArray<Scale> &scaleArr, sparseArray<Sprite> &ctrlDraw)
            { movement_system(r, resources, ctrlPos, ctrlVel, ctrlDraw, scaleArr); }
    );
    game.add_system_to_current_scene<IU, Position>(
            [&window = resources.getWindow()]
                    (Registry &r, sparseArray<IU> &ctrlIu, sparseArray<Position> &ctrlPos)
            { ui_system(r, window, ctrlIu, ctrlPos); }
    );
    game.add_system_to_current_scene<Position, Collider, Controllable, Animations>(
            []
                    (Registry &r, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim)
            { main_menu_button_system(r, pos, cldr, ctrl, anim); }
    );
    game.add_system_to_current_scene<Animations, Sprite>(
            [&resources]
                    (Registry &r, sparseArray<Animations> &ctrlPos, sparseArray<Sprite> &drawDraw)
            { animation_ui_system(r, resources, ctrlPos, drawDraw); }
    );
    game.add_system_to_current_scene<Sprite, IU>(
            [&resources]
            (Registry &r, sparseArray<Sprite> &drawDraw, sparseArray<IU> &crtlIu)
            { draw_system(r, resources, drawDraw, crtlIu); }
    );
}

void addGameSystemsClient(Registry &game, Resource::Graphics &resources, Resource::NtwClient &client, EntityLoad &loader)
{
    game.add_system_to_current_scene<Position, Velocity, Scale, Sprite, Spawner, Uuid, Controllable>(
            [&resources, &safeQueue = client.getClient().GetUdpSafeQueue(), &client, &loader]
                    (Registry &r, sparseArray<Position> &pos, sparseArray<Velocity> &vel, sparseArray<Scale> &scale, sparseArray<Sprite> &sprite, sparseArray<Spawner> &spawn, sparseArray<Uuid> &uuid, sparseArray<Controllable> &ctrl)
            { network_system_receive(r, resources, safeQueue, client, pos, vel, scale, sprite, spawn, uuid, ctrl, loader); }
    ); //udpate pos de any entity //spawn any bullet

    game.add_system_to_current_scene<>(
            [&resources, &safeQueue = client.getClient().GetTcpSafeQueue(), &client]
            (Registry &r)
            { tcp_client_receive_system(r, resources, safeQueue, client); }
    );

    game.add_system_to_current_scene<Controllable>(
            [&resources]
            (Registry &r, sparseArray<Controllable> &ctrlArr)
            { event_system(r, resources.getEvents(), resources.getWindow(), ctrlArr); }
    );

    game.add_system_to_current_scene<Controllable, Uuid>(
            [&resources, &client]
            (Registry &r, sparseArray<Controllable> &ctrlArr, sparseArray<Uuid> &uuid)
            { send_event_system(r, client, resources, ctrlArr, uuid); }
    );//send direction < > ^ v

    game.add_system_to_current_scene<Velocity, Controllable, Animations, Spawner, Power>(
            []
            (Registry &r, sparseArray<Velocity> &vel, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim, sparseArray<Spawner> &spawn, sparseArray<Power> &power)
            { player_system(r, vel, ctrl, anim, spawn, power); }
    );

    game.add_system_to_current_scene<Spawner>(
            [&resources, &loader]
            (Registry &r, sparseArray<Spawner> &spawn)
            { spawner_system(r, resources, spawn, loader); }
    );

    game.add_system_to_current_scene<Position, Collider, Velocity>(
            [&resources]
            (Registry &r, sparseArray<Position> &arrPos, sparseArray<Collider> &arrCol, sparseArray<Velocity> &arrVel)
            { manage_levels(r, resources, arrPos, arrCol, arrVel); }
    );//peu etre il print pas prcq il est au dessus de collision_system

    game.add_system_to_current_scene<Animations, Sprite>(
            [&resources]
            (Registry &r, sparseArray<Animations> &ctrlPos, sparseArray<Sprite> &drawDraw)
            { animation_system(r, resources, ctrlPos, drawDraw); }
    );

    game.add_system_to_current_scene<Position, Collider, Scale, Velocity>(
            [&resources]
            (Registry &r, sparseArray<Position> &ctrlPos, sparseArray<Collider> &colArr, sparseArray<Scale> &scaleArr, sparseArray<Velocity> &velArr)
            { collision_system(r, resources, ctrlPos, colArr, scaleArr, velArr); }
    );

    game.add_system_to_current_scene<Velocity, Animations, Sprite, Spawner, Position, Collider, Scale, Parent, Follow, Uuid>(
            [&resources]
            (Registry &r, sparseArray<Velocity> &vel, sparseArray<Animations> &anim, sparseArray<Sprite> &sprite, sparseArray<Spawner> &spawn, sparseArray<Position> &pos, sparseArray<Collider> &col, sparseArray<Scale> &scale, sparseArray<Parent> &parent, sparseArray<Follow> &follow, sparseArray<Uuid> &uuid)
            { players_ship_system(r, vel, anim, sprite, resources, spawn, pos, col, scale, parent, follow, uuid); }
    );

    game.add_system_to_current_scene<Follow, Position, Velocity>(
            []
                    (Registry &r, sparseArray<Follow> &follow, sparseArray<Position> &pos, sparseArray<Velocity> &vel)
            { follow_system(r, follow, pos, vel); }
        );

    game.add_system_to_current_scene<Position, Velocity, Scale, Sprite>(
            [&resources]
                    (Registry &r, sparseArray<Position> &ctrlPos, sparseArray<Velocity> &ctrlVel, sparseArray<Scale> &scaleArr, sparseArray<Sprite> &ctrlDraw)
            { movement_system(r, resources, ctrlPos, ctrlVel, ctrlDraw, scaleArr); }
    );//prediction system

    game.add_system_to_current_scene<Position, Velocity, Spawner, Uuid, Controllable>(
            [&resources, &inputHist = client.getInputHistory(), &client]
             (Registry &r, sparseArray<Position> &pos, sparseArray<Velocity> &vel, sparseArray<Spawner> &spawn, sparseArray<Uuid> &uuid, sparseArray<Controllable> &ctrlArr)
            { network_system_send(r, resources, inputHist, client, pos, vel, spawn, uuid, ctrlArr); }
    );

    game.add_system_to_current_scene<IU, Position>(
            [&window = resources.getWindow()]
            (Registry &r, sparseArray<IU> &ctrlIu, sparseArray<Position> &ctrlPos)
            { ui_system(r, window, ctrlIu, ctrlPos); }
    );
    game.add_system_to_current_scene<Position>(
            [&resources]
            ([[maybe_unused]] Registry &r, sparseArray<Position> &arrPos)
            { paralaxe_system(r, resources, arrPos); }
    );
    
    game.add_system_to_current_scene<Uuid, Spawner>(
            []
            ([[maybe_unused]] Registry &r, sparseArray<Uuid> &uuid, sparseArray<Spawner> &spawn)
            { reset_events_system(r, uuid, spawn); }
    );

    game.add_system_to_current_scene<>(
            [&resources]
            ([[maybe_unused]] Registry &r)
            { resources.resetClock(); }
    );
    game.add_system_to_current_scene<Sprite, IU>(
            [&resources]
            (Registry &r, sparseArray<Sprite> &drawDraw, sparseArray<IU> &crtlIu)
            { draw_system(r, resources, drawDraw, crtlIu); }
    );
}