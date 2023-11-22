#include <iostream>

#include "Registry.hpp"
#include "sparseArray.hpp"
#include "Graphics.hpp"
#include "NtwClient.hpp"

#include <SFML/Window.hpp>

void registerComponents(Registry &);
void addGameSystems(Registry &, Resource::Graphics&, Resource::NtwClient &);
void addMainMenuSystems(Registry &, Resource::Graphics&, Resource::NtwClient &);
void addMenuLobbySystems(Registry &, Resource::Graphics &, Resource::NtwClient &);

void initMenuLobby(Registry &game, Resource::Graphics &gr)
{
    // mouse
    auto mouse = game.spawn_entity();
    game.add_entity_to_scene(mouse, "menuLobby");
    game.tag_entity(mouse, "mouse");
    game.emplace_component<Controllable>(mouse);

    // Logo SUPER Rtype
    auto rtypeLogo = game.spawn_entity();
    game.add_entity_to_scene(rtypeLogo, "menuLobby");
    game.emplace_component<Position>(rtypeLogo, 660, 400);
    game.emplace_component<Scale>(rtypeLogo, 1.5, 1.5);

    auto &rtypeLogoTexture = gr.getTexture(Resource::Graphics::Textures::RTYPE_LOGO);
    auto &rtypeLogoSprite = game.emplace_component<Sprite>(rtypeLogo);
    rtypeLogoSprite->sprite.setTexture(rtypeLogoTexture);
    rtypeLogoSprite->sprite.setTextureRect(sf::IntRect(0, 0, 400, 148));
}

void initMainMenu(Registry &game, Resource::Graphics &gr)
{
//    Mouse Component -> juste la sourie
    auto mouse = game.spawn_entity();
    game.add_entity_to_scene(mouse, "menu");
    game.tag_entity(mouse, "mouse");
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
    game.tag_entity(buttonTest, "linkToChooseLobby");
    game.emplace_component<Position>(buttonTest, 100, 100);
    game.emplace_component<Collider>(buttonTest, RectangleCollider{.width = 80, .height = 32});
    game.emplace_component<Animations>(buttonTest);
    game.emplace_component<Scale>(buttonTest, 10, 4);

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
    buttonIU->color = sf::Color::Yellow;
    buttonIU->text.setString("Test");
    buttonIU->text.setFont(gr.getFont(Resource::Graphics::FONT1));
    buttonIU->text.setCharacterSize(20);
}

void initPlayer(Registry &game, Resource::Graphics &gr)
{
    auto player = game.spawn_entity();

    game.add_entity_to_scene(player, "default");
    game.tag_entity(player, "player");
    game.emplace_component<Velocity>(player, 0, 0, 200);
    game.emplace_component<Animations>(player);
    game.emplace_component<Position>(player, 100, 100);
    game.emplace_component<Scale>(player, 2, 2);
    game.emplace_component<Controllable>(player);
    game.emplace_component<Collider>(player, RectangleCollider{.width = 33, .height = 17});

    // Add texture to player
    auto &playerTexture = gr.getTexture(Resource::Graphics::Textures::PLAYERS); // Should be done elsewhere
    auto &playerDrawable = game.emplace_component<Sprite>(player);
    playerDrawable->sprite.setTexture(playerTexture);
    playerDrawable->sprite.setTextureRect(sf::IntRect(0, 0, 33, 17)); // Should maybe be from systems

    // Add BasicAnim to player
    auto &playerAnimations = game.get_components<Animations>()[player];
    playerAnimations->animations.emplace("BasicAnim", AnimationData{.texture = playerTexture, .nextFrameOffset = {33, 0}, .firstFramePosition = {66, 0}, .frames = 5, .timeStep = 0.1});
    playerAnimations->currentAnimation = playerAnimations->animations.begin();
}

void initObject(Registry &game, Resource::Graphics &gr)
{
    auto object = game.spawn_entity();

    game.add_entity_to_scene(object, "default");
    game.emplace_component<Position>(object, 200, 200);
    game.emplace_component<Scale>(object, 0.5, 0.5);

    // Add texture to object
    auto &testSprite = game.emplace_component<Sprite>(object);
    auto &testTexture = gr.addTexture("./assets/textures/mdr.jpg");
    testSprite->sprite.setTexture(testTexture);
    game.emplace_component<Collider>(object, RectangleCollider{.width = 225, .height = 225});

    // sf::Font testFont;
    // testFont.loadFromFile("./assets/fonts/LiberationSans.ttf");
    // // Add text to an entity
    // auto &iuDrawable = game.emplace_component<IU>(object);
    // iuDrawable->text.setFont(testFont);
    // iuDrawable->text.setFillColor(sf::Color::White);
    // iuDrawable->text.setString("RTYPE");
}

int main([[maybe_unused]] int ac, char **av)
{
    Registry game;
    Resource::Graphics resources;
    Resource::NtwClient client(av[1], atoi(av[2]));
    registerComponents(game);
    game.add_scene("menu", Registry::Scene());
    game.change_scene("menu");
    game.handle_scenes();
    addMainMenuSystems(game, resources, client);

    initMainMenu(game, resources);

    game.add_scene("menuLobby", Registry::Scene());
    game.change_scene("menuLobby");
    game.handle_scenes();
    addMenuLobbySystems(game, resources, client);

    initMenuLobby(game, resources);

    game.change_scene("menu");
    game.handle_scenes();

//    game.change_scene("default");

//    addGameSystems(game, resources, client);

//    initPlayer(game, resources);
//    initObject(game, resources);

//    game.change_scene("menu");
    // systems will run in order of adding

    while (resources.getWindow().isOpen()) {
        game.run_scene_systems();
        game.handle_scenes();
    }
    return 0;
}