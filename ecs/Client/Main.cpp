#include <iostream>
#include "Registry.hpp"
#include "sparseArray.hpp"
#include "Graphics.hpp"
#include "NtwClient.hpp"
#include "EntityLoad.hpp"

void registerComponents(Registry &);
void addGameSystemsClient(Registry &, Resource::Graphics&, Resource::NtwClient &, EntityLoad &loader);
void addMainMenuSystems(Registry &, Resource::Graphics&, Resource::NtwClient &);
void addMenuJoinLobbySystems(Registry &, Resource::Graphics &, Resource::NtwClient &);
void addMenuCreateLobbySystems(Registry &, Resource::Graphics &, Resource::NtwClient &);
void addMenuLobbySystems(Registry &, Resource::Graphics &, Resource::NtwClient &);

void initLobbyMenu(Registry &game, Resource::Graphics &gr)
{
    //    Mouse Component -> juste la sourie
    auto mouse = game.spawn_entity();
    game.add_entity_to_scene(mouse, "menuLobby");
    game.tag_entity(mouse, "mouse_lobby");
    game.emplace_component<Controllable>(mouse);

    auto mouse_ready = game.spawn_entity();
    game.add_entity_to_scene(mouse_ready, "menuLobby");
    game.tag_entity(mouse_ready, "mouse_lobby_ready");
    game.emplace_component<Controllable>(mouse_ready);

    // Button
    auto buttonTest = game.spawn_entity();
    game.add_entity_to_scene(buttonTest, "menuLobby");
    game.tag_entity(buttonTest, "LinkToJoinLobby");
    game.emplace_component<Position>(buttonTest, 50, 20);
    game.emplace_component<Collider>(buttonTest, RectangleCollider{.width = 160, .height = 80});
    game.emplace_component<Animations>(buttonTest);
    game.emplace_component<Scale>(buttonTest, 20, 10);

    // set texture to button sprite
    auto &buttonTexture = gr.getTexture(Resource::Graphics::Textures::BUTTON_EMPTY); // Should be done elsewhere
    auto &buttonDrawable = game.emplace_component<Sprite>(buttonTest);
    buttonDrawable->sprite.setTexture(buttonTexture);
    buttonDrawable->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8)); // Should maybe be from systems

    // animation button
    auto &animButton = game.get_components<Animations>()[buttonTest];
    animButton->active = true;
    animButton->animations.emplace("buttonEmpty", AnimationData{.texture = buttonTexture, .nextFrameOffset = {8, 0}, .firstFramePosition = {0, 0}, .frames = 3, .timeStep = 1000});
    animButton->currentAnimation = animButton->animations.begin();

    // Simple yellow text
    auto &buttonIU = game.emplace_component<IU>(buttonTest);
    buttonIU->offset = {50, 20};
    buttonIU->color = sf::Color::Yellow;
    buttonIU->text.setString("Back");
    buttonIU->text.setFont(gr.getFont(Resource::Graphics::FONT1));
    buttonIU->text.setCharacterSize(30);

    // SECOND BUTTON
    auto buttonCreate = game.spawn_entity();
    game.add_entity_to_scene(buttonCreate, "menuLobby");
    game.tag_entity(buttonCreate, "ReadyToPlay");
    game.emplace_component<Position>(buttonCreate, 560, 100);
    game.emplace_component<Collider>(buttonCreate, RectangleCollider{.width = 800, .height = 240});
    game.emplace_component<Animations>(buttonCreate);
    game.emplace_component<Scale>(buttonCreate, 100,  30);
    game.emplace_component<Container<bool>>(buttonCreate, true);

    // set texture to button sprite
    auto &buttonTxt = gr.getTexture(Resource::Graphics::Textures::BUTTON_EMPTY); // Should be done elsewhere
    auto &buttonDraw = game.emplace_component<Sprite>(buttonCreate);
    buttonDraw->sprite.setTexture(buttonTxt);
    buttonDraw->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8)); // Should maybe be from systems

    // animation button
    auto &animBut = game.get_components<Animations>()[buttonCreate];
    animBut->active = true;
    animBut->animations.emplace("buttonEmpty", AnimationData{.texture = buttonTxt, .nextFrameOffset = {8, 0}, .firstFramePosition = {0, 0}, .frames = 3, .timeStep = 1000});
    animBut->currentAnimation = animBut->animations.begin();

    // Simple yellow button
    auto &buttonUI = game.emplace_component<IU>(buttonCreate);
    buttonUI->offset = {110, 40};
    buttonUI->color = sf::Color::Green;
    buttonUI->text.setString("READY TO PLAY");
    buttonUI->text.setFont(gr.getFont(Resource::Graphics::FONT1));
    buttonUI->text.setCharacterSize(70);
}

void initMenuJoinLobby(Registry &game, Resource::Graphics &gr)
{
    // mouse
    auto mouse = game.spawn_entity();
    game.add_entity_to_scene(mouse, "menuJoinLobby");
    game.tag_entity(mouse, "mouse_join_lobby");
    game.emplace_component<Controllable>(mouse);

    // RETURN Button
    auto buttonTest = game.spawn_entity();
    game.add_entity_to_scene(buttonTest, "menuJoinLobby");
    game.tag_entity(buttonTest, "LinkToReturnMainMenu");
    game.emplace_component<Position>(buttonTest, 50, 20);
    game.emplace_component<Collider>(buttonTest, RectangleCollider{.width = 160, .height = 80});
    game.emplace_component<Animations>(buttonTest);
    game.emplace_component<Scale>(buttonTest, 20, 10);

    // set texture to button sprite
    auto &buttonTexture = gr.getTexture(Resource::Graphics::Textures::BUTTON_EMPTY); // Should be done elsewhere
    auto &buttonDrawable = game.emplace_component<Sprite>(buttonTest);
    buttonDrawable->sprite.setTexture(buttonTexture);
    buttonDrawable->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8)); // Should maybe be from systems

    // animation button
    auto &animButton = game.get_components<Animations>()[buttonTest];
    animButton->active = true;
    animButton->animations.emplace("buttonEmpty", AnimationData{.texture = buttonTexture, .nextFrameOffset = {8, 0}, .firstFramePosition = {0, 0}, .frames = 3, .timeStep = 1000});
    animButton->currentAnimation = animButton->animations.begin();

    // Simple yellow button
    auto &buttonIU = game.emplace_component<IU>(buttonTest);
    buttonIU->offset = {40, 15};
    buttonIU->color = sf::Color::Blue;
    buttonIU->text.setString("BACK");
    buttonIU->text.setFont(gr.getFont(Resource::Graphics::FONT1));
    buttonIU->text.setCharacterSize(30);

    //
}

void initMenuCreateLobby(Registry &game, Resource::Graphics &gr)
{
//    Mouse Component -> juste la sourie
    auto mouse = game.spawn_entity();
    game.add_entity_to_scene(mouse, "menuCreateLobby");
    game.tag_entity(mouse, "mouse_create_menu");
    game.emplace_component<Controllable>(mouse);

    // RETURN Button
    auto buttonTest = game.spawn_entity();
    game.add_entity_to_scene(buttonTest, "menuCreateLobby");
    game.tag_entity(buttonTest, "LinkToReturnMainMenu");
    game.emplace_component<Position>(buttonTest, 50, 20);
    game.emplace_component<Collider>(buttonTest, RectangleCollider{.width = 160, .height = 80});
    game.emplace_component<Animations>(buttonTest);
    game.emplace_component<Scale>(buttonTest, 20, 10);

    // set texture to button sprite
    auto &buttonTexture = gr.getTexture(Resource::Graphics::Textures::BUTTON_EMPTY); // Should be done elsewhere
    auto &buttonDrawable = game.emplace_component<Sprite>(buttonTest);
    buttonDrawable->sprite.setTexture(buttonTexture);
    buttonDrawable->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8)); // Should maybe be from systems

    // animation button
    auto &animButton = game.get_components<Animations>()[buttonTest];
    animButton->active = true;
    animButton->animations.emplace("buttonEmpty", AnimationData{.texture = buttonTexture, .nextFrameOffset = {8, 0}, .firstFramePosition = {0, 0}, .frames = 3, .timeStep = 1000});
    animButton->currentAnimation = animButton->animations.begin();

    // Simple yellow button
    auto &buttonIU = game.emplace_component<IU>(buttonTest);
    buttonIU->offset = {8, 10};
    buttonIU->color = sf::Color::Green;
    buttonIU->text.setString("     EXIT");
    buttonIU->text.setFont(gr.getFont(Resource::Graphics::FONT1));
    buttonIU->text.setCharacterSize(30);

    // CREATE BUTTON
    auto createButton = game.spawn_entity();
    game.add_entity_to_scene(createButton, "menuCreateLobby");
    game.tag_entity(createButton, "LinkToCreateNewLobby");
    game.emplace_component<Position>(createButton, 640, 700);
    game.emplace_component<Collider>(createButton, RectangleCollider{.width = 640, .height = 200});
    game.emplace_component<Animations>(createButton);
    game.emplace_component<Scale>(createButton, 80, 25);

    // set texture to button sprite
    auto &buttonTxt = gr.getTexture(Resource::Graphics::Textures::BUTTON_EMPTY); // Should be done elsewhere
    auto &buttonDra = game.emplace_component<Sprite>(createButton);
    buttonDra->sprite.setTexture(buttonTxt);
    buttonDra->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8)); // Should maybe be from systems

    // animation button
    auto &animBut = game.get_components<Animations>()[createButton];
    animBut->active = true;
    animBut->animations.emplace("buttonEmpty", AnimationData{.texture = buttonTxt, .nextFrameOffset = {8, 0}, .firstFramePosition = {0, 0}, .frames = 3, .timeStep = 1000});
    animBut->currentAnimation = animBut->animations.begin();

    // Simple yellow button
    auto &buttonUI = game.emplace_component<IU>(createButton);
    buttonUI->offset = {115, 50};
    buttonUI->color = sf::Color::Black;
    buttonUI->text.setString("CREATE NEW LOBBY");
    buttonUI->text.setFont(gr.getFont(Resource::Graphics::FONT1));
    buttonUI->text.setCharacterSize(40);
    buttonUI->text.setOutlineThickness(2);
}

void initMainMenu(Registry &game, Resource::Graphics &gr)
{
//    Mouse Component -> juste la sourie
    auto mouse = game.spawn_entity();
    game.add_entity_to_scene(mouse, "menu");
    game.tag_entity(mouse, "mouse_menu");
    game.emplace_component<Controllable>(mouse);

    // background logo Rtype
    auto logoBackground = game.spawn_entity();
    game.add_entity_to_scene(logoBackground, "menu");
    game.emplace_component<Position>(logoBackground, 660, 30);
    game.emplace_component<Scale>(logoBackground, 2.6, 2);

    auto &logoBgTexture = gr.getTexture(Resource::Graphics::Textures::LOGO_BACKGROUND);
    auto &logoBgSprite = game.emplace_component<Sprite>(logoBackground);
    logoBgSprite->sprite.setTexture(logoBgTexture);
    logoBgSprite->sprite.setTextureRect(sf::IntRect(0, 0, 231, 115));

    // Logo SUPER Rtype
    auto rtypeLogo = game.spawn_entity();
    game.add_entity_to_scene(rtypeLogo, "menu");
    game.emplace_component<Position>(rtypeLogo, 660, 30);
    game.emplace_component<Scale>(rtypeLogo, 1.5, 1.5);

    auto &rtypeLogoTexture = gr.getTexture(Resource::Graphics::Textures::RTYPE_LOGO);
    auto &rtypeLogoSprite = game.emplace_component<Sprite>(rtypeLogo);
    rtypeLogoSprite->sprite.setTexture(rtypeLogoTexture);
    rtypeLogoSprite->sprite.setTextureRect(sf::IntRect(0, 0, 400, 148));

    // Button
    auto buttonTest = game.spawn_entity();
    game.add_entity_to_scene(buttonTest, "menu");
    game.tag_entity(buttonTest, "LinkToCreateLobby");
    game.emplace_component<Position>(buttonTest,  300, 700);
    game.emplace_component<Collider>(buttonTest, RectangleCollider{.width = 320, .height = 120});
    game.emplace_component<Animations>(buttonTest);
    game.emplace_component<Scale>(buttonTest, 40, 15);

    // set texture to button sprite
    auto &buttonTexture = gr.getTexture(Resource::Graphics::Textures::BUTTON_EMPTY); // Should be done elsewhere
    auto &buttonDrawable = game.emplace_component<Sprite>(buttonTest);
    buttonDrawable->sprite.setTexture(buttonTexture);
    buttonDrawable->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8)); // Should maybe be from systems

    // animation button
    auto &animButton = game.get_components<Animations>()[buttonTest];
    animButton->active = true;
    animButton->animations.emplace("buttonEmpty", AnimationData{.texture = buttonTexture, .nextFrameOffset = {8, 0}, .firstFramePosition = {0, 0}, .frames = 3, .timeStep = 1000});
    animButton->currentAnimation = animButton->animations.begin();

    // Simple yellow text
    auto &buttonIU = game.emplace_component<IU>(buttonTest);
    buttonIU->offset = {-25, 25};
    buttonIU->color = sf::Color::Yellow;
    buttonIU->text.setString("         CREATE");
    buttonIU->text.setFont(gr.getFont(Resource::Graphics::FONT1));
    buttonIU->text.setCharacterSize(40);

    // SECOND BUTTON
    auto buttonCreate = game.spawn_entity();
    game.add_entity_to_scene(buttonCreate, "menu");
    game.tag_entity(buttonCreate, "LinkToJoinLobby");
    game.emplace_component<Position>(buttonCreate, 1300, 700);
    game.emplace_component<Collider>(buttonCreate, RectangleCollider{.width = 320, .height = 120});
    game.emplace_component<Animations>(buttonCreate);
    game.emplace_component<Scale>(buttonCreate, 40, 15);

    // set texture to button sprite
    auto &buttonTxt = gr.getTexture(Resource::Graphics::Textures::BUTTON_EMPTY); // Should be done elsewhere
    auto &buttonDraw = game.emplace_component<Sprite>(buttonCreate);
    buttonDraw->sprite.setTexture(buttonTxt);
    buttonDraw->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8)); // Should maybe be from systems

    // animation button
    auto &animBut = game.get_components<Animations>()[buttonCreate];
    animBut->active = true;
    animBut->animations.emplace("buttonEmpty", AnimationData{
        .texture = buttonTxt, 
        .nextFrameOffset = {8, 0}, 
        .firstFramePosition = {0, 0}, 
        .frames = 3, 
        .timeStep = 1000
    });

    animBut->currentAnimation = animBut->animations.begin();

    // Simple yellow button
    auto &buttonUI = game.emplace_component<IU>(buttonCreate);
    buttonUI->offset = {0, 25};
    buttonUI->color = sf::Color::Green;
    buttonUI->text.setString("          JOIN");
    buttonUI->text.setFont(gr.getFont(Resource::Graphics::FONT1));
    buttonUI->text.setCharacterSize(40);
}

void initParalaxes(Registry &game, Resource::Graphics &gr)
{
    auto paralaxe = game.spawn_entity();

    game.add_entity_to_scene(paralaxe, "game");
    game.tag_entity(paralaxe, "paralaxe");
    game.emplace_component<Position>(paralaxe, 0, -30);
    game.emplace_component<Velocity>(paralaxe, -350, 0, 0);
    game.emplace_component<Scale>(paralaxe, 1, 1);
    auto &testSprite = game.emplace_component<Sprite>(paralaxe);
    auto &testTexture = gr.addTexture("./assets/textures/paralaxes/ParalaxeElarged.png");
    testSprite->sprite.setTexture(testTexture);
}

void createLevelBlock(Registry &game, int posX, int posY, int scaleX, int scaleY, int wdth, int hght)
{
    auto block = game.spawn_entity();
    game.add_entity_to_scene(block, "game");
    game.tag_entity(block, "level");
    game.emplace_component<Position>(block, posX, posY);
    game.emplace_component<Velocity>(block, 0, 0, 0);
    game.emplace_component<Scale>(block, scaleX, scaleY);
    game.emplace_component<Collider>(block, RectangleCollider{.width = wdth, .height = hght});
}

void initLevels([[maybe_unused]]Registry &game, [[maybe_unused]]Resource::Graphics &gr)
{
    auto &lvlTexture = gr.addTexture("./assets/textures/levels/gamemap0.png");

    //level map
    auto level = game.spawn_entity();
    game.add_entity_to_scene(level, "game");
    game.tag_entity(level, "level");
    game.emplace_component<Position>(level, 0, -30);
    game.emplace_component<Velocity>(level, 0, 0, 0);
    game.emplace_component<Scale>(level, 5.5, 5.6);
    auto &lvlSprite = game.emplace_component<Sprite>(level);
    lvlSprite->sprite.setTexture(lvlTexture);

    //screen limit 
    auto lvlDownLimit = game.spawn_entity();
    game.add_entity_to_scene(lvlDownLimit, "game");
    game.tag_entity(lvlDownLimit, "limit");
    game.emplace_component<Position>(lvlDownLimit, ((1920 / 2) - ((225 / 2) * 8.3)), (1080 - 120));
    game.emplace_component<Scale>(lvlDownLimit, 8.3, 0.5);
    game.emplace_component<Collider>(lvlDownLimit, RectangleCollider{.width = 225, .height = 100});
    
    auto lvlUpLimit = game.spawn_entity();
    game.add_entity_to_scene(lvlUpLimit, "game");
    game.tag_entity(lvlUpLimit, "limit");
    game.emplace_component<Position>(lvlUpLimit, ((1920 / 2) - ((225 / 2) * 8.3)), -100);
    game.emplace_component<Scale>(lvlUpLimit, 8.3, 1);
    game.emplace_component<Collider>(lvlUpLimit, RectangleCollider{.width = 225, .height = 100});

    //level design boxes
    createLevelBlock(game, 750, (1080 - 250), 1.3, 1, 100, 225);
    createLevelBlock(game, 2100, (1080 - 250), 3, 2, 120, 225);
    createLevelBlock(game, 4650, (1080 - 250), 1.3, 1, 100, 225);
    createLevelBlock(game, 5300, (1080 - 250), 1.3, 1, 100, 225);
    createLevelBlock(game, 5300, (1080 - 250), 1.3, 1, 100, 225);
    createLevelBlock(game, 8100, (1080 - 250), 1.3, 2.5, 100, 225);
    createLevelBlock(game, 9000, ((225 * 2.5) + 50), 2, 2.1, 100, 225);
    createLevelBlock(game, 9000, (((225 * 2.5) / 2 ) - 350), 2, 2.1, 100, 225);
    createLevelBlock(game, 10700, ((225 * 2.5) + 100), 2.5, 1.7, 100, 225);
    createLevelBlock(game, 10700, ((225 * 2.5) / 2 ), 2.5, 1.7, 100, 225);
    createLevelBlock(game, 16700, ((225 * 2.5) + 100), 3.9, 2.2, 100, 225);
    createLevelBlock(game, 16700, (((225 * 2.5) / 2 ) - 350), 3.9, 2.2, 100, 225);
    createLevelBlock(game, 21500, ((225 * 2.5) + 100),  2, 1.5, 100, 225);
    createLevelBlock(game, 21500, (((225 * 2.5) / 2 ) - 350),  2, 1.5, 100, 225);

    //bonuses
    auto bonus = game.spawn_entity();
    game.add_entity_to_scene(bonus, "game");
    game.emplace_component<Position>(bonus, 900, 500);//la pos a pas bougé wsh
    game.emplace_component<Collider>(bonus, RectangleCollider{.width = 20, .height = 16});

    auto &bonusInfo = game.emplace_component<BeamBonusInfo>(bonus);
    bonusInfo->animationId = 0;
    bonusInfo->beamIndice = 10;
    game.emplace_component<Scale>(bonus, 2, 2);
    game.emplace_component<Velocity>(bonus, 0, 0, 200);
    auto &bonusDrawable = game.emplace_component<Sprite>(bonus);
    auto &bonusAnimations = game.emplace_component<Animations>(bonus);
    auto &bonusTexture = gr.getTexture(Resource::Graphics::EFFECTS2); // Should be done elsewhere
    bonusDrawable->sprite.setTexture(bonusTexture);
    bonusDrawable->sprite.setTextureRect(sf::IntRect(159, 35, 20, 16)); // Should maybe be from systems

    bonusAnimations->animations.emplace("BasicAnim", AnimationData{
            .texture = bonusTexture,
            .nextFrameOffset = {20 + 4, 0},
            .firstFramePosition = sf::Vector2f(159, 35),
            .frames = 5,
            .timeStep = 0.1
    });
    bonusAnimations->currentAnimation = bonusAnimations->animations.begin();
    bonusAnimations->loop = true;
    bonusAnimations->active = true;

    auto &bonusFollow = game.emplace_component<Follow>(bonus);
    bonusFollow->type = Follow::SMOOTH;
    bonusFollow->offset = {33 * 2, 8};
}

int main([[maybe_unused]] int ac, char **av)
{
    Registry game;

    Resource::Graphics resources;
    Resource::NtwClient client(av[1], std::atoi(av[2]));
    SignalHandler signalHandler;
    signalHandler.setupSignalHandlers();
    EntityLoad loader;

    registerComponents(game);

    resources.setMusic(Resource::Graphics::MENU_MUSIC);

    // SCENE MAIN MENU
    game.add_scene("menu", Registry::Scene());
    game.change_scene("menu");
    game.handle_scenes();
    addMainMenuSystems(game, resources, client);

    initMainMenu(game, resources);

    // SCENE JOIN LOBBY
    game.add_scene("menuJoinLobby", Registry::Scene());
    game.change_scene("menuJoinLobby");
    game.handle_scenes();
    addMenuJoinLobbySystems(game, resources, client);

    initMenuJoinLobby(game, resources);

    // SCENE CREATE LOBBY
    game.add_scene("menuCreateLobby", Registry::Scene());
    game.change_scene("menuCreateLobby");
    game.handle_scenes();
    addMenuCreateLobbySystems(game, resources, client);

    initMenuCreateLobby(game, resources);

    // SCENE GAME
    game.add_scene("game", Registry::Scene());
    game.change_scene("game");
    game.handle_scenes();
    loader.loadEntity(game);
    addGameSystemsClient(game, resources, client, loader);

    initParalaxes(game, resources);
    initLevels(game, resources);

    // SCENE LOBBY
    game.add_scene("menuLobby", Registry::Scene());
    game.change_scene("menuLobby");
    game.handle_scenes();
    addMenuLobbySystems(game, resources, client);

    initLobbyMenu(game, resources);


    // REMET LA SCENE DE DÉPART
    game.change_scene("menu");
    game.handle_scenes();

    // systems will run in order of adding
    resources.resetClock();
    while (resources.getWindow().isOpen()) {
        game.run_scene_systems();
        game.handle_scenes();
    }
    client.getClient().ReqDisconnectionBySignal((client.getClient().GetTcpClient()->getInLobby().first
        ? client.getClient().GetTcpClient()->getInLobby().second : 0));
    return 0;
}