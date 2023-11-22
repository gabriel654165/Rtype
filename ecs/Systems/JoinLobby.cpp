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

static void compute_join_session_button([[maybe_unused]] Registry &r, Resource::NtwClient &client,
    std::optional<Container<std::size_t>> &container, [[maybe_unused]] unsigned long id)
{
    Network::Protocol::Manager protocol;
    protocol.pack("dd", Network::Protocol::CodeTcp::ReqConnectToLobby, container->value);
    client.getClient().GetTcpClient()->Send(protocol.p);
}

static void set_join_button_state(Registry &r, std::optional<Controllable> &iteCtrl,
    std::optional<Animations> &iteAnim, unsigned long id, Resource::NtwClient &client,
    std::optional<Container<std::size_t>> &container)
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
        compute_join_session_button(r, client, container, id);
        iteCtrl->isClicked = false;
    }
}

void join_lobby_system([[maybe_unused]]Registry &r, Resource::NtwClient &client, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl,
    sparseArray<Animations> &anim, sparseArray<Container<std::size_t>> &container)
{
    auto mouse = *r.get_tagged_entities("mouse2").begin();
    auto &controller = ctrl[mouse];
    for (auto &id : r.getScene().entities) {
        if (!pos[id].has_value() || !cldr[id].has_value() || !container[id].has_value()
            || !anim[id].has_value())
            continue;
        if (cldr[id]->type == Collider::RECTANGLE) {
            auto &colliderInfo = std::any_cast<RectangleCollider &>(cldr[id]->infos);
            if (controller->mousePosition.x >= pos[id]->x + colliderInfo.xoff
                && controller->mousePosition.x <= pos[id]->x + colliderInfo.xoff + colliderInfo.width) {
                if (controller->mousePosition.y >= pos[id]->y + colliderInfo.yoff
                    && controller->mousePosition.y <= pos[id]->y + colliderInfo.yoff + colliderInfo.height) {
                    set_join_button_state(r, controller, anim[id], id, client, container[id]);
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