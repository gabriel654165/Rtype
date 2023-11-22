#include "Registry.hpp"
#include "Graphic/Graphics.hpp"

void event_system([[maybe_unused]] Registry &r, sf::Event events, sf::RenderWindow &window, sparseArray<Controllable> &pContr) 
{
    //auto player = *r.get_tagged_entities("player").begin();

    while (window.pollEvent(events)) {
        Registry::entity_t entity = 0;

        for (auto &n: pContr) {
            if (!n.has_value())
                continue;

            if (events.type == sf::Event::Closed)
                window.close();

            auto &controller = n.value();
            if (events.type == sf::Event::KeyPressed && !controller.keysPressed[events.key.code]) {
                if (events.key.code == sf::Keyboard::Escape)
                    window.close();
                controller.keysPressed[events.key.code] = true;
            } else if (events.type == sf::Event::KeyReleased) {
                controller.keysPressed[events.key.code] = false;
            }
//            if (events.type == sf::Event::Mouse)
            if (events.type == sf::Event::MouseButtonPressed) {//&& !controller.isClicked) {
                controller.isMousePressed = true;
                controller.isClicked = false;
            } else if (events.type == sf::Event::MouseButtonReleased) {
                controller.isMousePressed = false;
                controller.isClicked = true;
            }
            controller.mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            entity++;
        }
    }
}

void reset_events_system([[maybe_unused]] Registry &r, sparseArray<Uuid> &uuid, sparseArray<Spawner> &spawnArr)
{
    /*auto spawn = spawnArr.begin();
    for (int i = 0; spawn != spawnArr.end(); ++spawn, ++i) {
        if (!spawn->has_value())
            continue;
        if (spawn[i]->active) {
            std::cout << "-------setActive before " << spawn[i]->active << std::endl;
            spawn[i]->active = false;
            std::cout << "-------setActive after " << spawn[i]->active << " enetity id : " << i << std::endl;
        }
    }*/
}