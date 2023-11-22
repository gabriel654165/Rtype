#include "Registry.hpp"
#include "sparseArray.hpp"
#include "Graphic/Graphics.hpp"

void camera_system([[maybe_unused]] Registry &r, [[maybe_unused]]Resource::Graphics &resources)
{
    // move x by time
    auto newCenter = resources.getCamera().getCenter().x;
    float speedCamera = 100;

    if (newCenter == 0) {
        newCenter += resources.getDeltaTime() * speedCamera;
    } else {
        newCenter += resources.getDeltaTime() * speedCamera;
    }
    resources.getCamera().setCenter(newCenter, resources.getCamera().getCenter().y);

    //auto player = *r.get_tagged_entities("player").begin();
    //velArr[player]->x += 150;//le serveur lui donne ses pos
}