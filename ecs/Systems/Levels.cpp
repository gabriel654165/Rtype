#include "Registry.hpp"
#include "Graphics.hpp"
#include "sparseArray.hpp"

void resetMap(Registry &r, Resource::Graphics &resources, sparseArray<Position> &arrPos, sparseArray<Collider> &arrCol, sparseArray<Velocity> &arrVel)
{
    const std::size_t resetPosColliders [] = {0, 750, 2100, 4650, 5300, 
                                        8100, 9000, 9000, 10700, 10700, 
                                        16700, 16700, 21500, 21500};
    auto iteLevel = r.get_tagged_entities("level").begin();
    int speedMap = -200;

    for (int i = 0; i < 13 ; ++i, (*iteLevel++) ) {
        arrPos[*iteLevel]->x = resetPosColliders[i];
        arrVel[*iteLevel]->x = speedMap;
    }
    resources.resetClock();
}

void manage_levels([[maybe_unused]] Registry &r, Resource::Graphics &resources, sparseArray<Position> &arrPos, sparseArray<Collider> &arrCol, sparseArray<Velocity> &arrVel)
{
    auto levelMap = r.get_tagged_entities("level").begin();
    static bool reset = false;
    /*auto deltaTime = resources.getDeltaTime();
    static double displayTimeLvl1 = 0;
    static double displayTimeLvl2 = 0;
    static bool onceKilled1 = false;
    static bool onceKilled2 = false;
    static bool onceCreated1 = false;
    static bool onceCreated2 = false;
*/
    if (!reset) {
        resetMap(r, resources, arrPos, arrCol, arrVel);
        reset = true;
    }

    //std::cout << "POS MAP X = " << arrPos[*levelMap]->x << std::endl;
/*
    if (arrPos[*levelMap]->x <= 0 && !onceCreated1) {
        onceCreated1 = true;
        auto textLevel = r.spawn_entity();
        r.add_entity_to_scene(textLevel, "game");
        r.tag_entity(textLevel, "Textlvl1");
        auto atextPos = r.emplace_component<Position>(textLevel, (1920 / 2 - 125), (1080 / 2 - 100));
        auto &textUI = r.emplace_component<IU>(textLevel);
        textUI->offset = {-25, 25};
        textUI->color = sf::Color::White;
        textUI->text.setString("STAGE 1");
        textUI->text.setFont(resources.getFont(Resource::Graphics::FONT1));
        textUI->text.setCharacterSize(40);
    }
    if (arrPos[*levelMap]->x <= (-23125) && !onceCreated2) {
        onceCreated2 = true;
        auto textLevel = r.spawn_entity();
        r.add_entity_to_scene(textLevel, "game");
        r.tag_entity(textLevel, "Textlvl2");
        auto atextPos = r.emplace_component<Position>(textLevel, (1920 / 2 - 125), (1080 / 2 - 100));
        auto &textUI = r.emplace_component<IU>(textLevel);
        textUI->offset = {-25, 25};
        textUI->color = sf::Color::White;
        textUI->text.setString("STAGE 2");
        textUI->text.setFont(resources.getFont(Resource::Graphics::FONT1));
        textUI->text.setCharacterSize(40);
    }
    if (onceCreated1)
        displayTimeLvl1 += deltaTime;
    if (onceCreated2)
        displayTimeLvl2 += deltaTime;

    if (displayTimeLvl1 >= 5 && !onceKilled1) {
        auto itetext = r.get_tagged_entities("Textlvl1").begin();
        r.kill_entity(*itetext);
        onceKilled1 = true;
    }
    if (displayTimeLvl2 >= 5 && !onceKilled2) {
        auto itetext = r.get_tagged_entities("Textlvl2").begin();
        r.kill_entity(*itetext);
        onceKilled2 = true;
    }
*/
    if (arrPos[*levelMap]->x <= -47031.2) {
        resetMap(r, resources, arrPos, arrCol, arrVel);
        r.change_scene("menu");
    }
}