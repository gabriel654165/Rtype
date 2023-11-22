#include "Registry.hpp"
#include "sparseArray.hpp"
#include "Graphic/Graphics.hpp"

void animation_system([[maybe_unused]] Registry &r, Resource::Graphics &resources, sparseArray<Animations> &animArray, sparseArray<Sprite>& spriteArray)
{
    auto itAnim = animArray.begin();
    auto itSprite = spriteArray.begin();
    auto deltaTime = resources.getDeltaTime();

    for (; itSprite != spriteArray.end() && itAnim != animArray.end() ; ++itSprite, ++itAnim) {
        if (!itSprite->has_value() || (!itAnim->has_value()))
            continue;
        

        auto &spriteData= itSprite->value();
        auto textureRect = spriteData.sprite.getTextureRect();
        auto &animationData = itAnim->value().currentAnimation->second;        


        if (animationData.currentFrame == 0) { // not really nice, should be with the other if
            textureRect.left = animationData.firstFramePosition.x;
            textureRect.top = animationData.firstFramePosition.y;
            if (!(*itAnim)->active)
                spriteData.sprite.setTextureRect(textureRect);
        }
        if (!(*itAnim).has_value() || !(*itAnim)->active || (!itAnim->value().loop && (animationData.currentFrame == animationData.frames - 1)))
            continue;

        animationData.elapsedTime += deltaTime;
        
        if (animationData.elapsedTime < animationData.timeStep)
            continue;
        else
            animationData.elapsedTime = 0;

        if (animationData.nextFrameOffset.x > 0)
            animationData.currentFrame = (animationData.currentFrame + 1) % animationData.frames;
        else
            animationData.currentFrame = animationData.currentFrame ? (animationData.currentFrame - 1) : animationData.frames - 1;

        if (animationData.currentFrame != 0)  {
            textureRect.left += animationData.nextFrameOffset.x;
            textureRect.top += animationData.nextFrameOffset.y;
        }
        spriteData.sprite.setTextureRect(textureRect);
    }

}