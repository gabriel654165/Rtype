#include "Registry.hpp"
#include "NtwClient.hpp"

static void join_button_anim_pressed(std::optional<Animations> &iteAnim)
{
    iteAnim->currentAnimation->second.currentFrame = 2;
}

static void join_button_anim_fly_over(std::optional<Animations> &iteAnim)
{
    iteAnim->currentAnimation->second.currentFrame = 1;
}
/*
void button_color_change_pressed()
{

}

void button_color_change_fly_over()
{

}*/

static void compute_join_button(Registry &r, unsigned long distance, Resource::NtwClient &client)
{
    auto tagList = r.get_tagged_entities("LinkToReturnMainMenu");
    for (auto &it : tagList) {
        if (distance == it) {
            r.change_scene("menu");
            r.handle_scenes();
            return;
        }
    }
}

static void set_join_button_state(Registry &r, std::optional<Controllable> &iteCtrl, std::optional<Animations> &iteAnim, unsigned long distance, Resource::NtwClient &client)
{
    if (iteCtrl->isMousePressed) {
        if (iteAnim.has_value()) {
            join_button_anim_pressed(iteAnim);
        } else {
   //         button_color_change_pressed();
        }
    } else {
        if (iteAnim.has_value()) {
            join_button_anim_fly_over(iteAnim);
        } else {
    //        button_color_change_fly_over();
        }
    }
    if (iteCtrl->isClicked) {
        compute_join_button(r, distance, client);
        iteCtrl->isClicked = false;
    }
}

void join_lobby_button_system([[maybe_unused]]Registry &r, Resource::NtwClient &client, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim)
{
    auto mouse = *r.get_tagged_entities("mouse_join_lobby").begin();
    auto &controller = ctrl[mouse];
    for (auto &id : r.getScene().entities) {
        if (!pos[id].has_value() || !cldr[id].has_value() || !anim[id].has_value())
            continue;
        if (cldr[id]->type == Collider::RECTANGLE) {
            auto &colliderInfo = std::any_cast<RectangleCollider &>(cldr[id]->infos);
            if (controller->mousePosition.x >= pos[id]->x + colliderInfo.xoff
                && controller->mousePosition.x <= pos[id]->x + colliderInfo.xoff + colliderInfo.width) {
                if (controller->mousePosition.y >= pos[id]->y + colliderInfo.yoff
                    && controller->mousePosition.y <= pos[id]->y + colliderInfo.yoff + colliderInfo.height) {
                    set_join_button_state(r, controller, anim[id], id, client);
                } else {
                    anim[id]->currentAnimation->second.currentFrame = 0;
                }
            } else {
                anim[id]->currentAnimation->second.currentFrame = 0;
            }
        }
    }
    controller->isClicked = false;
}