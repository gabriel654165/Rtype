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

static void compute_join_button(Registry &r, Resource::NtwClient &client, std::optional<Uuid> &uuid)
{
    Network::Protocol::Manager protocol;
    std::cout << " J'APPUIE SUR LE BOUTON LOBBY N°" << uuid->uuid << std::endl;
    protocol.pack("dd", Network::Protocol::CodeTcp::ReqConnectToLobby, uuid->uuid);
    client.getClient().GetTcpClient()->Send(protocol.p);
}

static void set_join_button_state(Registry &r, std::optional<Controllable> &iteCtrl, std::optional<Animations> &iteAnim, unsigned long distance,
    Resource::NtwClient &client, std::optional<Uuid> &uuid)
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
        compute_join_button(r, client, uuid);
        iteCtrl->isClicked = false;
    }
}

void lobby_list_button_system([[maybe_unused]]Registry &r, Resource::NtwClient &client,
    sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl,
    sparseArray<Animations> &anim, sparseArray<Uuid> &uuid)
{
    auto mouse = *r.get_tagged_entities("mouse2").begin();
    auto &controller = ctrl[mouse];
    for (auto &id : r.getScene().entities) {
        if (!pos[id].has_value() || !cldr[id].has_value() || !uuid[id].has_value())
            continue;
        if (cldr[id]->type == Collider::RECTANGLE) {
            auto &colliderInfo = std::any_cast<RectangleCollider &>(cldr[id]->infos);
            if (controller->mousePosition.x >= pos[id]->x + colliderInfo.xoff
                && controller->mousePosition.x <= pos[id]->x + colliderInfo.xoff + colliderInfo.width) {
                if (controller->mousePosition.y >= pos[id]->y + colliderInfo.yoff
                    && controller->mousePosition.y <= pos[id]->y + colliderInfo.yoff + colliderInfo.height) {
                    set_join_button_state(r, controller, anim[id], id, client, uuid[id]);
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