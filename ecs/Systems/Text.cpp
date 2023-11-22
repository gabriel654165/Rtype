#include "Registry.hpp"
#include "sparseArray.hpp"

void ui_system([[maybe_unused]] Registry &r, [[maybe_unused]] sf::RenderWindow &window, sparseArray<IU> &ctrlIu, sparseArray<Position> &ctrlPos)
{
    auto itePos = ctrlPos.begin();
    auto iteIu = ctrlIu.begin();

    for (; itePos != ctrlPos.end() && iteIu != ctrlIu.end() ; iteIu++, itePos++) {
        if (!(iteIu)->has_value()) {
            continue;
        } else {
            (*iteIu)->text.setFillColor((*iteIu)->color);
        }
        if (itePos->has_value()) {
            (*iteIu)->text.setPosition((*itePos)->x + (*iteIu)->offset.x, (*itePos)->y + (*iteIu)->offset.y);
        }
    }
}
