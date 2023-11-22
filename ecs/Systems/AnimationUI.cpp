#include "Registry.hpp"
#include "sparseArray.hpp"
#include "Graphic/Graphics.hpp"

void animation_ui_system([[maybe_unused]] Registry &r, [[maybe_unused]]Resource::Graphics &resources, sparseArray<Animations> &animArray, sparseArray<Sprite>& spriteArray)
{
    auto itAnim = animArray.begin();
    auto itSprite = spriteArray.begin();

    for (; itAnim != animArray.end() && itSprite != spriteArray.end() ; ++itSprite, ++itAnim) {
        if (!itSprite->has_value() || (!itAnim->has_value()))
            continue;

        auto &spriteData= itSprite->value();
        auto &animationData = itAnim->value().currentAnimation->second;
        auto textureRect = spriteData.sprite.getTextureRect();

        textureRect.left = animationData.nextFrameOffset.x * animationData.currentFrame;
        textureRect.top = animationData.nextFrameOffset.y * animationData.currentFrame;

        spriteData.sprite.setTextureRect(textureRect);
    }}