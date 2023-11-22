#include "Registry.hpp"
#include "Graphic/Graphics.hpp"
#include "sparseArray.hpp"
#include "NtwClient.hpp"


void resAskForInfo(Network::Buffer packet, [[maybe_unused]]Registry &r, [[maybe_unused]]Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::Manager protocol;
    Network::Protocol::UnpackDataUdp data;

    std::cout << "[TCP CLIENT][RECEIVE] : server send udp info" << std::endl;
    data.list = new std::size_t[3];
    Network::Protocol::Manager::unPack(packet, "dlu", &data.typeHeader, &data.lenList, data.list);
    std::cout << "INFO : len list = " << data.lenList << std::endl;
    for (std::size_t i = 0; i < (data.lenList / sizeof(std::size_t)); i += 3) {
        client.getClient().GetLobbys().insert(std::make_pair(data.list[i], std::make_unique<NetworkClient::Lobby>(data.list[i], 5, data.list[i + 1])));
        client.getClient().GetLobbys()[data.list[i]]->setReadyPlayer(data.list[i + 2]);
        std::cout << "INFO : " << std::endl <<
                     "       - lobby : " << data.list[i] << std::endl <<
                     "       - nb player : " << data.list[i + 1] << std::endl <<
                     "       - nb player ready : " << data.list[i + 2] << std::endl;

        auto buttonLobby = r.spawn_entity();
        r.add_entity_to_scene(buttonLobby, "menuJoinLobby");
        r.tag_entity(buttonLobby, "lobbyId_" + std::to_string(data.list[i]));
        r.emplace_component<Position>(buttonLobby, 1000, 100 * client.getClient().GetLobbys().size());
        r.emplace_component<Collider>(buttonLobby, RectangleCollider{.width = 400, .height = 80});
        r.emplace_component<Animations>(buttonLobby);
        r.emplace_component<Scale>(buttonLobby, 50, 10);
        r.emplace_component<Container<std::size_t>>(buttonLobby, data.list[i]);

        // set texture to button sprite
        auto &buttonTexture = rsc.getTexture(Resource::Graphics::Textures::BUTTON_EMPTY); // Should be done elsewhere
        auto &buttonDrawable = r.emplace_component<Sprite>(buttonLobby);
        buttonDrawable->sprite.setTexture(buttonTexture);
        buttonDrawable->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8)); // Should maybe be from systems

        // animation button
        auto &animButton = r.get_components<Animations>()[buttonLobby];
        animButton->active = true;
        animButton->animations.emplace("buttonEmpty", AnimationData{.texture = buttonTexture, .nextFrameOffset = {8, 0}, .firstFramePosition = {0, 0}, .frames = 3, .timeStep = 1000});
        animButton->currentAnimation = animButton->animations.begin();

        // Simple yellow button
        auto &buttonIU = r.emplace_component<IU>(buttonLobby);
        buttonIU->offset = {60, 10};
        buttonIU->text.setString("Lobby " + std::to_string(data.list[i]) + "   " +
                                 std::to_string(client.getClient().GetLobbys()[data.list[i]]->getNmberPlayer()) +
                                 "/" + std::to_string(client.getClient().GetLobbys()[data.list[i]]->getMaxPlayer()));
        buttonIU->text.setFont(rsc.getFont(Resource::Graphics::FONT1));
        buttonIU->text.setCharacterSize(40);
    }
    delete [] data.list;
}

void notifyReadyToPlay(Network::Buffer packet, [[maybe_unused]]Registry &r, [[maybe_unused]]Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;

    try {
        Network::Protocol::Manager::unPack(packet, "dd", &t.typeHeader, &t.lobbyId);
        std::cout << "[TCP CLIENT][RECEIVE] : notify that client is ready to play in lobby " << t.lobbyId << "." << std::endl;

        client.getClient().GetLobbys()[t.lobbyId]->setReadyPlayer(client.getClient().GetLobbys()[t.lobbyId]->getReadyPlayer() + 1);

        if (client.getClient().GetTcpClient()->getInLobby().second == t.lobbyId) {
            r.get_tagged_entities("lobbyCountReady").begin();

        }
        std::cout << std::endl;
    } catch (std::exception &error) {
        std::cerr << "Error notify ready to play" << "\n";
    }
}

void resConnection([[maybe_unused]]Network::Buffer packet, [[maybe_unused]]Registry &r, [[maybe_unused]]Resource::Graphics &rsc, Resource::NtwClient &client)
{
    std::cout << "[TCP CLIENT][RECEIVE] : connection approved." << std::endl;
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;

    char title[16] = {0};
    //std::string title(16, 0);
    //std::cout <<
    Network::Protocol::Manager::unPack(packet, "dls", &t.typeHeader, &t.nbPlayer, title);
    std::string temp(title);
//    client.getClient()._udpClient->SetEndpoint(temp);
    client.getClient().SetRemoteEndPoint(temp);

    protocol.pack("d", Network::Protocol::CodeTcp::ReqAskForInfo);
    client.getClient().GetTcpClient()->Send(protocol.p);

    std::cout << "[TCP CLIENT][SEND] : Ask for data server." << std::endl;
}

void notifyNotReadyToPlay(Network::Buffer packet, [[maybe_unused]]Registry &r, [[maybe_unused]]Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;

    try {
        Network::Protocol::Manager::unPack(packet, "dd", &t.typeHeader, &t.lobbyId);
        std::cout << "[TCP CLIENT][RECEIVE] : notify that client is not ready to play in lobby " << t.lobbyId << "."
            << std::endl;

        client.getClient().GetLobbys()[t.lobbyId]->setReadyPlayer(client.getClient().GetLobbys()[t.lobbyId]->getReadyPlayer() - 1);
        client.getClient().GetLobbys()[t.lobbyId]->setStart(false);
        client.getClient().SetStartGame(false);
        if (client.getClient().GetTcpClient()->getInLobby().second == t.lobbyId) {
            r.get_tagged_entities("lobbyCountReady").begin();
            // TODO -> dire que le client est pas prêt
        }
        std::cout << std::endl;
    } catch (std::exception &error) {
        std::cerr << "Error notify ready to play" << "\n";
    }
}

void resErrorConnectionToLobby(Network::Buffer packet, [[maybe_unused]]Registry &r, [[maybe_unused]]Resource::Graphics &rsc, [[maybe_unused]]Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;

    try {
        Network::Protocol::Manager::unPack(packet, "d", &t.typeHeader);
        std::cout << "[TCP CLIENT][RECEIVE] : Error connection to lobby " << t.lobbyId << "." << std::endl;
    }
    catch (std::exception &error) {
        std::cerr << "res error lobby creation " << error.what() << "\n";
    }
}

//////////////////NOTIFY////////////////////

void notifyLobbyCreated(Network::Buffer packet, Registry &r, Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;


    try {
        Network::Protocol::Manager::unPack(packet, "ddd", &t.typeHeader, &t.lobbyId, &t.maxPlayer);
        std::cout << "[TCP CLIENT][RECEIVE] : notify that lobby " << t.lobbyId << " is created." << std::endl;
        std::cout << "                        Max Player : " << t.maxPlayer << std::endl;
        std::cout << "[TCP CLIENT][ACTION] : add a new lobby." << std::endl;
        client.getClient().GetLobbys().insert(std::make_pair(t.lobbyId, std::make_unique<NetworkClient::Lobby>(t.lobbyId, t.maxPlayer, 1)));
        client.getClient().GetLobbys()[t.lobbyId]->setNumberPlayer(1);

        auto buttonLobby = r.spawn_entity();
        r.add_entity_to_scene(buttonLobby, "menuJoinLobby");
        r.tag_entity(buttonLobby, "lobbyId_" + std::to_string(t.lobbyId));
        r.emplace_component<Position>(buttonLobby, 1000, 100 * client.getClient().GetLobbys().size());
        r.emplace_component<Collider>(buttonLobby, RectangleCollider{.width = 400, .height = 80});
        r.emplace_component<Animations>(buttonLobby);
        r.emplace_component<Scale>(buttonLobby, 50, 10);
        r.emplace_component<Container<std::size_t>>(buttonLobby, t.lobbyId);

        // set texture to button sprite
        auto &buttonTexture = rsc.getTexture(Resource::Graphics::Textures::BUTTON_EMPTY); // Should be done elsewhere
        auto &buttonDrawable = r.emplace_component<Sprite>(buttonLobby);
        buttonDrawable->sprite.setTexture(buttonTexture);
        buttonDrawable->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8)); // Should maybe be from systems

        // animation button
        auto &animButton = r.get_components<Animations>()[buttonLobby];
        animButton->active = true;
        animButton->animations.emplace("buttonEmpty", AnimationData{.texture = buttonTexture, .nextFrameOffset = {8, 0}, .firstFramePosition = {0, 0}, .frames = 3, .timeStep = 1000});
        animButton->currentAnimation = animButton->animations.begin();

        // Simple yellow button
        auto &buttonIU = r.emplace_component<IU>(buttonLobby);
        buttonIU->offset = {60, 10};
        buttonIU->text.setString("Lobby " + std::to_string(t.lobbyId) + "   " +
            std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer()) +
            "/" + std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getMaxPlayer()));
        buttonIU->text.setFont(rsc.getFont(Resource::Graphics::FONT1));
        buttonIU->text.setCharacterSize(40);
        std::cout << std::endl;
    } catch (std::exception &error) {
        std::cerr << "Error in lobby creation notification" << error.what() << "\n";
    }
}

void notifyConnectionInLobby(Network::Buffer packet, Registry &r, [[maybe_unused]]Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::Manager protocol;
    Network::Protocol::UnpackData t;

    try {
        Network::Protocol::Manager::unPack(packet, "dd", &t.typeHeader, &t.lobbyId);
        std::cout << "[TCP CLIENT][RECEIVE] : notify that someone connected to " << t.lobbyId << "." << std::endl;
        client.getClient().GetLobbys()[t.lobbyId]->setNumberPlayer(client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer() + 1);
        std::cout << client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer() << std::endl;
        auto lobbyId = *r.get_tagged_entities("lobbyId_" + std::to_string(t.lobbyId)).begin();
        auto &buttonLobbyText = r.get_components<IU>();
        if (buttonLobbyText[lobbyId].has_value()) {
            std::cout << "[TCP CLIENT][ACTION] : add a client from lobby " << t.lobbyId << "." << std::endl;
            buttonLobbyText[lobbyId]->text.setString("Lobby " + std::to_string(t.lobbyId) + "   " +
                std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer()) +
                "/" + std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getMaxPlayer()));
            std::cout << "NUMBER OF CLIENT WHEN NOTIFY = " << client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer() << std::endl;
        }
        if (client.getClient().GetTcpClient()->getInLobby().second == t.lobbyId) {
            auto astro = r.spawn_entity();
            r.add_entity_to_scene(astro, "menuLobby");
            r.tag_entity(astro, "astroPopup");
            r.emplace_component<Position>(astro, 300, 100 * client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer());
            r.emplace_component<Scale>(astro, 1, 1);
            auto &astroTexture = rsc.getTexture(Resource::Graphics::Textures::ASTRONAUT); // Should be done elsewhere
            auto &astroSprite = r.emplace_component<Sprite>(astro);
            astroSprite->sprite.setTexture(astroTexture);
            astroSprite->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8)); // Should maybe be from systems
        }
        std::cout << std::endl;
    } catch (std::exception &error) {
        std::cerr << "Error in lobby connection notification " << error.what() << "\n";
    }
}

void notifyDisconnectedFromLobby(Network::Buffer packet, Registry &r, [[maybe_unused]]Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;

    try {
        Network::Protocol::Manager::unPack(packet, "ddd", &t.typeHeader, &t.lobbyId, &t.spaceShipId);
        std::cout << "[TCP CLIENT][RECEIVE] : notify that someone has disconnected from lobby " << t.lobbyId <<  "." << std::endl;

        if (client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer() > 1) {
            std::cout << "[TCP CLIENT][ACTION] : remove a client from lobby " << t.lobbyId << "." << std::endl;
            client.getClient().GetLobbys()[t.lobbyId]->setNumberPlayer(client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer() - 1);
            auto lobbyId = *r.get_tagged_entities("lobbyId_" + std::to_string(t.lobbyId)).begin();
            auto &buttonLobbyText = r.get_components<IU>();
            if (buttonLobbyText[lobbyId].has_value()) {
                buttonLobbyText[lobbyId]->text.setString("Lobby " + std::to_string(t.lobbyId) + "   " +
                    std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer()) +
                    "/" + std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getMaxPlayer()));
            }
        } else {
            std::cout << "[TCP CLIENT][ACTION] : delete lobby " << t.lobbyId << "." << std::endl;
            auto lobbyId = *r.get_tagged_entities("lobbyId_" + std::to_string(t.lobbyId)).begin();
            client.getClient().GetLobbys().erase(t.lobbyId);
            r.kill_entity(lobbyId);
        }
        if (client.getClient().GetTcpClient()->getInLobby().second == t.lobbyId) {
            //std::cout << "JE DOIS DÉTRUIRE LE VAISSEAU : " << t.spaceShipId << std::endl;
            auto &uuidList = r.get_components<Uuid>();
            for (auto ite = uuidList.begin(); ite != uuidList.end(); ite++) {
                if (!ite->has_value())
                    continue;
                if ((*ite)->uuid == t.spaceShipId) {
                    std::unique_lock<std::mutex> lock(client._mutexKillEntity);
                    r.kill_entity(std::distance(uuidList.begin(), ite));
//                    std::unique_lock<std::mutex> unlock(client._mutexKillEntity);
                    break;
                }
            }
        }
        std::cout << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void notifyLobbyDeleted(Network::Buffer packet, Registry &r, [[maybe_unused]]Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;

    try {
        Network::Protocol::Manager::unPack(packet, "dd", &t.typeHeader, &t.lobbyId);

        std::cout << "[TCP CLIENT][RECEIVE] : notify lobby " << t.lobbyId << " get deleted." << std::endl;

        auto lobbyId = *r.get_tagged_entities("lobbyId_" + std::to_string(t.lobbyId)).begin();
        client.getClient().GetLobbys().erase(t.lobbyId);
        r.kill_entity(lobbyId);
        std::cout << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void initPlayerByUuid(Registry &game, Resource::Graphics &gr, std::size_t &newIdPlayer, bool isYourPlayer)
{
    game.handle_scenes();
    auto newPlayer = game.spawn_entity();

    game.add_entity_to_scene(newPlayer, "game");
    game.tag_entity(newPlayer, "players");
    game.emplace_component<Velocity>(newPlayer, 0, 0, 400);
    game.emplace_component<Animations>(newPlayer);
    game.emplace_component<Position>(newPlayer, 100, (100 * newIdPlayer));
    game.emplace_component<Scale>(newPlayer, 2, 2);
    if (isYourPlayer) {
        game.tag_entity(newPlayer, "player");
        game.emplace_component<Controllable>(newPlayer);
    }
    game.emplace_component<Uuid>(newPlayer, newIdPlayer);
    auto &spawner = game.emplace_component<Spawner>(newPlayer, BulletInfo{
            .path = BulletInfo::STRAIGHT,
            .velocity = {700, 0},
            .speed = 600,
            .damage = 15
    });
    spawner->timeToWait = 0.1;
    spawner->offset = sf::Vector2f(25, 17/2 - 2);
    game.emplace_component<Collider>(newPlayer, RectangleCollider{.width = 33, .height = 17});
    // Add texture to newPlayer
    auto &playerTexture = gr.getTexture(Resource::Graphics::Textures::PLAYERS);
    auto &playerDrawable = game.emplace_component<Sprite>(newPlayer);
    playerDrawable->sprite.setTexture(playerTexture);
    playerDrawable->sprite.setTextureRect(sf::IntRect(0, 17, 33, 17));
    
    auto &playerAnimations = game.get_components<Animations>()[newPlayer];
    playerAnimations->animations.emplace("BasicAnim", AnimationData{
            .texture = playerTexture,
            .nextFrameOffset = {33, 0},
            .firstFramePosition = sf::Vector2f(0, (17 * (newIdPlayer - 1))),
            .frames = 5,
            .timeStep = 0.1
    });
    //set loop true and current anim
    playerAnimations->currentAnimation = playerAnimations->animations.begin();
    playerAnimations->loop = true;

    /*if (!(yourUuid == newIdPlayer))
        std::cout << "---------------------CLIENT CREATED SHIP : id = " << newIdPlayer << " posY = " << (100 * newIdPlayer) << std::endl;
    else
        std::cout << "---------------------CLIENT CREATED OWN SHIP : id = " << newIdPlayer << " posY = " << (100 * newIdPlayer) << std::endl;
    */
}

void addOwnedSpaceship(Network::Buffer packet, Registry &r, Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;
    try {
        Network::Protocol::Manager::unPack(packet, "ddd", &t.typeHeader, &t.lobbyId, &t.spaceShipId);
        if (client.getClient().GetTcpClient()->getInLobby().first
            && client.getClient().GetTcpClient()->getInLobby().second == t.lobbyId) {
            initPlayerByUuid(r, rsc, t.spaceShipId, true);
            std::cout << "[CLIENT TCP][RECEIVE] : server ask add your spaceship n°" << t.spaceShipId << std::endl;
        }
    } catch (std::exception &error) {
        std::cerr << "Error in nbPlayer" << std::endl;
    }
}

void addPlayerSpaceship(Network::Buffer packet, Registry &r, Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;
    try {
        Network::Protocol::Manager::unPack(packet, "ddd", &t.typeHeader, &t.lobbyId, &t.spaceShipId);
        if (client.getClient().GetTcpClient()->getInLobby().first
            && client.getClient().GetTcpClient()->getInLobby().second == t.lobbyId) {
            initPlayerByUuid(r, rsc, t.spaceShipId, false);
            std::cout << "[CLIENT TCP][RECEIVE] : server ask add spaceship n°" << t.spaceShipId << std::endl;
        }
    } catch (std::exception &error) {
        std::cerr << "Error in nbPlayer" << std::endl;
    }
}

void notifyStartGame(Network::Buffer packet, Registry &r, [[maybe_unused]]Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;
    Network::Protocol::Manager::unPack(packet, "ddd", &t.typeHeader, &t.lobbyId, &t.nbPlayer);
    try {
        if (client.getClient().GetTcpClient()->getInLobby().first
            && client.getClient().GetTcpClient()->getInLobby().second == t.lobbyId) {
            std::cout << "[TCP CLIENT][RECEIVE] : notify game started in lobby " << t.lobbyId << "." << std::endl;
            client.setNbPlayers(t.nbPlayer);
            client.getClient().CreateNewUdpClient();
            client.getClient().SetUdpState(true);
            client.getClient().GetLobbys()[t.lobbyId]->setStart(true);
            r.change_scene("game");
            rsc.setMusic(Resource::Graphics::GAME_MUSIC);
            r.handle_scenes();
        }
        std::cout << std::endl;
    } catch (std::exception &error) {
        std::cerr << "Error notify ready to play" << "\n";
    }
}


void resConnectLobby(Network::Buffer packet, Registry &r, Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;

    try {
        Network::Protocol::Manager::unPack(packet, "ddddd", &t.typeHeader, &t.lobbyId, &t.clientRole, &t.lobbyPort, &t.maxPlayer);
        std::cout << "[TCP CLIENT][RECEIVE] : connection to lobby " << t.lobbyId << "." << std::endl;
        std::cout << "                        Role : " << (t.clientRole == Network::UdpSessionRole::ADMIN ? "Admin" : "guest") << "." << std::endl;
        std::cout << "                        UDP port : " << t.lobbyPort << "." << std::endl;

        client.getClient().SetUdpPort(t.lobbyPort);
        client.getClient().GetTcpClient()->setInLobby(true, t.lobbyId);
        auto it = client.getClient().GetLobbys().find(t.lobbyId);
        if (it == client.getClient().GetLobbys().end()) {
            client.getClient().GetLobbys().insert(std::make_pair(t.lobbyId, std::make_unique<NetworkClient::Lobby>(t.lobbyId, t.maxPlayer, 1)));

            auto buttonLobby = r.spawn_entity();
            r.add_entity_to_scene(buttonLobby, "menuJoinLobby");
            r.tag_entity(buttonLobby, "lobbyId_" + std::to_string(t.lobbyId));
//        r.tag_entity(buttonLobby, "LinkToReturnMainMenu");
            r.emplace_component<Position>(buttonLobby, 1000, 100 * client.getClient().GetLobbys().size());
            r.emplace_component<Collider>(buttonLobby, RectangleCollider{.width = 400, .height = 80});
            r.emplace_component<Animations>(buttonLobby);
            r.emplace_component<Scale>(buttonLobby, 50, 10);
            r.emplace_component<Container<std::size_t>>(buttonLobby, t.lobbyId);

            // set texture to button sprite
            auto &buttonTexture = rsc.getTexture(Resource::Graphics::Textures::BUTTON_EMPTY); // Should be done elsewhere
            auto &buttonDrawable = r.emplace_component<Sprite>(buttonLobby);
            buttonDrawable->sprite.setTexture(buttonTexture);
            buttonDrawable->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8)); // Should maybe be from systems

            // animation button
            auto &animButton = r.get_components<Animations>()[buttonLobby];
            animButton->active = true;
            animButton->animations.emplace("buttonEmpty", AnimationData{.texture = buttonTexture, .nextFrameOffset = {8, 0}, .firstFramePosition = {0, 0}, .frames = 3, .timeStep = 1000});
            animButton->currentAnimation = animButton->animations.begin();

            // Simple yellow text
            auto &buttonIU = r.emplace_component<IU>(buttonLobby);
            buttonIU->text.setString("Lobby [ " + std::to_string(t.lobbyId) + " ]   " +
                std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer()) +
                "/" + std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getMaxPlayer()));
            buttonIU->text.setFont(rsc.getFont(Resource::Graphics::FONT1));
            buttonIU->text.setCharacterSize(20);
        } else {
            client.getClient().GetLobbys()[t.lobbyId]->setNumberPlayer(client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer() + 1);
            auto lobbyId = *r.get_tagged_entities("lobbyId_" + std::to_string(t.lobbyId)).begin();
            auto &buttonLobbyText = r.get_components<IU>();
            if (buttonLobbyText[lobbyId].has_value()) {
                buttonLobbyText[lobbyId]->text.setString("Lobby n [ " + std::to_string(t.lobbyId) + " ]   " +
                    std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer()) +
                    "/" + std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getMaxPlayer()));
            }

        }
        r.change_scene("menuLobby");
        std::cout << std::endl;
    } catch (std::exception &error)
    {
        std::cerr << error.what() << "\n";
    }
}

void resDisconnection(Network::Buffer packet, [[maybe_unused]]Registry &r, [[maybe_unused]]Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;
    std::cout << "[TCP CLIENT][RECEIVE] : disconnection approved." << std::endl;

    try {
        Network::Protocol::Manager::unPack(packet, "d", &t.typeHeader);
        client.getClient().GetTcpClient()->Stop();
        std::cout << std::endl;
    } catch (std::exception &error) {
        std::cerr << "Error in disconnection response" << error.what() << "\n";
    }
}

void resDisconnectLobby(Network::Buffer packet, Registry &r, [[maybe_unused]]Resource::Graphics &rsc, Resource::NtwClient &client)
{
    Network::Protocol::UnpackData t;
    Network::Protocol::Manager protocol;

    try {
        Network::Protocol::Manager::unPack(packet, "dd", &t.typeHeader, &t.lobbyId);
        std::cout << "[TCP CLIENT][RECEIVE] : disconnection to lobby " << t.lobbyId << "." << std::endl;

        client.getClient().GetTcpClient()->setInLobby(false, t.lobbyId);
        if (client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer() > 1) {
            std::cout << "[TCP CLIENT][ACTION] : remove a client from lobby " << t.lobbyId << "." << std::endl;
            client.getClient().GetLobbys()[t.lobbyId]->setNumberPlayer(
                    client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer() - 1);
            auto lobbyId = *r.get_tagged_entities("lobbyId_" + std::to_string(t.lobbyId)).begin();
            auto &buttonLobbyText = r.get_components<IU>();
            if (buttonLobbyText[lobbyId].has_value()) {
                buttonLobbyText[lobbyId]->text.setString("Lobby " + std::to_string(t.lobbyId) + "   " +
                    std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getNmberPlayer()) +
                    "/" + std::to_string(client.getClient().GetLobbys()[t.lobbyId]->getMaxPlayer()));
            }
        } else {
            std::cout << "[TCP CLIENT][ACTION] : delete lobby " << t.lobbyId << "." << std::endl;
            auto lobbyId = *r.get_tagged_entities("lobbyId_" + std::to_string(t.lobbyId)).begin();
            client.getClient().GetLobbys().erase(t.lobbyId);
            r.kill_entity(lobbyId);
        }
        r.change_scene("menu");
        std::cout << std::endl;
    } catch (std::exception &error) {
        std::cerr << "Error in response disconnect from lobby" << error.what() << "\n";
    }
}

void tcp_client_receive_system([[maybe_unused]]Registry &r, Resource::Graphics &rsc, Network::LockedQueue<unsigned char *> &safeQueue, Resource::NtwClient &client)
{
    if (!safeQueue.empty()) {
        static const std::map<Network::Digit, std::function<void(unsigned char *, Registry &, Resource::Graphics &,
            Resource::NtwClient &, const boost::system::error_code &)>> proto = {
                {Network::Protocol::CodeTcp::ResConnectionEstablished,
                    []([[maybe_unused]]unsigned char *packet,
                       [[maybe_unused]]Registry &r, [[maybe_unused]]Resource::Graphics &rsc,
                       Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        resConnection(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::ResAskForInfo,
                        []([[maybe_unused]]unsigned char *packet,
                           [[maybe_unused]]Registry &r, [[maybe_unused]]Resource::Graphics &rsc,
                           [[maybe_unused]]Resource::NtwClient &client,
                           const boost::system::error_code &er) {
                            if (!er)
                                resAskForInfo(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::ResDisconnected,
                    [](unsigned char *packet, [[maybe_unused]]Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        resDisconnection(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::ResConnectedToLobby,
                    [](unsigned char *packet, Registry &r,
                    Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        resConnectLobby(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::NotifyConnectedToLobby,
                    [](unsigned char *packet, Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        notifyConnectionInLobby(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::ResErrorConnectionToLobby,
                    [](unsigned char *packet, [[maybe_unused]]Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    [[maybe_unused]]Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        resErrorConnectionToLobby(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::ResDisconnectedFromLobby,
                    [](unsigned char *packet, Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        resDisconnectLobby(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::NotifyDisconnectedFromLobby,
                    [](unsigned char *packet, Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        notifyDisconnectedFromLobby(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::NotifyLobbyCreated,
                    [](unsigned char *packet, Registry &r,
                    Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        notifyLobbyCreated(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::NotifyReadyToPlay,
                    [](unsigned char *packet, [[maybe_unused]]Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        notifyReadyToPlay(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::ResReadyToPlay,
                    [](unsigned char *packet, [[maybe_unused]]Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        notifyReadyToPlay(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::NotifyNotReadyToPlay,
                    [](unsigned char *packet, [[maybe_unused]]Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        notifyNotReadyToPlay(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::NotifyStartGame,
                    [](unsigned char *packet, Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        notifyStartGame(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::ResAddYourSpaceship,
                    [](unsigned char *packet, Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        addOwnedSpaceship(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::ResAddASpaceship,
                    [](unsigned char *packet, Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        addPlayerSpaceship(packet, r, rsc, client);
                }},
                {Network::Protocol::CodeTcp::NotifyLobbyDeleted,
                    [](unsigned char *packet, Registry &r,
                    [[maybe_unused]]Resource::Graphics &rsc,
                    Resource::NtwClient &client,
                    const boost::system::error_code &er) {
                    if (!er)
                        notifyLobbyDeleted(packet, r, rsc, client);
                }}};
        auto packet = safeQueue.pop();
        boost::system::error_code error;
        Network::Digit type = 0;
        Network::Protocol::Manager::unPack(packet, "d", &type);
        std::cout << "UNPACK REQUEST N°" << type << std::endl;
        try {
            proto.at(type)(packet, r, rsc, client, error);
        }
        catch (const std::out_of_range &e) {
            std::cout << "no callback found, verify Header Type" << std::endl;
        }
    }
}