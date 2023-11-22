
#include "Registry.hpp"
#include "NtwClient.hpp"

void lobby_button_anim_pressed(std::optional<Animations> &iteAnim)
{
    iteAnim->currentAnimation->second.currentFrame = 2;
}

void lobby_button_anim_fly_over(std::optional<Animations> &iteAnim)
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

void compute_lobby_button(Registry &r, unsigned long distance, Resource::NtwClient &client,
    std::optional<Container<bool>> &container)
{
    auto tagList = r.get_tagged_entities("ReadyToPlay");
    Network::Protocol::Manager protocol;
    for (auto &it : tagList) {
        if (distance == it) {
            if (container->value) {
                container->value = false;
                protocol.pack("dd", Network::Protocol::CodeTcp::ReqReadyToPlay,
                    client.getClient().GetTcpClient()->getInLobby().second);
                client.getClient().GetTcpClient()->Send(protocol.p);
            } else {
                container->value = true;
                protocol.pack("dd", Network::Protocol::CodeTcp::ReqNotReadyToPlay,
                              client.getClient().GetTcpClient()->getInLobby().second);
                client.getClient().GetTcpClient()->Send(protocol.p);
            }
            return;
        }
    }
}

void set_lobby_button_state(Registry &r, std::optional<Controllable> &iteCtrl, std::optional<Animations> &iteAnim, unsigned long distance, Resource::NtwClient &client,
    std::optional<Container<bool>> &container)
{
    if (iteCtrl->isMousePressed) {
        if (iteAnim.has_value()) {
            lobby_button_anim_pressed(iteAnim);
        } else {
      //      button_color_change_pressed();
        }
    } else {
        if (iteAnim.has_value()) {
            lobby_button_anim_fly_over(iteAnim);
        } else {
       //     button_color_change_fly_over();
        }
    }
    if (iteCtrl->isClicked) {
        compute_lobby_button(r, distance, client, container);
        iteCtrl->isClicked = false;
    }
}

void lobby_button_system([[maybe_unused]]Registry &r, Resource::NtwClient &client, sparseArray<Position> &pos, sparseArray<Collider> &cldr, sparseArray<Controllable> &ctrl, sparseArray<Animations> &anim,
    sparseArray<Container<bool>> &container)
{
    auto mouse = *r.get_tagged_entities("mouse_lobby").begin();
    auto &controller = ctrl[mouse];
    for (auto &id : r.getScene().entities) {
        if (!pos[id].has_value() || !cldr[id].has_value() || !container[id].has_value() || !anim[id].has_value())
            continue;
        if (cldr[id]->type == Collider::RECTANGLE) {
            auto &colliderInfo = std::any_cast<RectangleCollider &>(cldr[id]->infos);
            if (controller->mousePosition.x >= pos[id]->x + colliderInfo.xoff
                && controller->mousePosition.x <= pos[id]->x + colliderInfo.xoff + colliderInfo.width) {
                if (controller->mousePosition.y >= pos[id]->y + colliderInfo.yoff
                    && controller->mousePosition.y <= pos[id]->y + colliderInfo.yoff + colliderInfo.height) {
                    set_lobby_button_state(r, controller, anim[id], id, client, container[id]);
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