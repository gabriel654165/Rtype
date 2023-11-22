#include "Registry.hpp"
#include "Graphic/Graphics.hpp"
#include "sparseArray.hpp"

void draw_system([[maybe_unused]] Registry &r, Resource::Graphics &resources, sparseArray<Sprite> &ctrlDraw, sparseArray<IU> &crtlIU)
{
    // auto itePos = ctrlPos.begin();
    sf::RenderWindow &window = resources.getWindow();

    window.clear();
    window.setView(resources.getCamera());

    //draw obj
    for (auto id: r.getScene().entities) {
        if (ctrlDraw[id].has_value())
            window.draw(ctrlDraw[id]->sprite);
        if (crtlIU[id].has_value())
            window.draw(crtlIU[id]->text);
    }
    window.display();
}