#include "block.hpp"
#include "global.hpp"

Block::Block(const BlockProperties& newProperties, const BlockSprite& newSpriteInfos, BlockId newId)
    : properties(newProperties), spriteInfos(newSpriteInfos), id(newId)
{
    sprite.setSize(sf::Vector2f(spriteInfos.size.x, spriteInfos.size.y));
    sprite.setFillColor(spriteInfos.color);
    setCollisionForVersion();
}

void Block::update()
{
    if (hasCheckedCollideLastFrame)
    {
        hasCheckedCollideLastFrame = false;
    }
    else
    {
        wasInCollideLastFrame = false;
    }
}

void Block::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

bool Block::applyCollision(Character& character, Direction movementDir, bool onlyPositionCheck)
{
    bool isColliding = collision.isCollidingBlock(character, *this, movementDir);

    if (onlyPositionCheck)
    {
        if (isColliding && properties.isSolid)
        {
            collision.replaceCharacterNearBlock(character, *this, movementDir);
            character.hasEnterInCollide(movementDir);
        }
    }
    else
    {
        if (isColliding && (properties.isTriggeredContinuously || !wasInCollideLastFrame))
        {
            if (properties.isDeadlyToPlayer)
            {
                character.setStatus(Character::Status::isDead, true);
            }
            if (properties.doStopPlayerFromMoving)
            {
                character.setStatus(Character::Status::canMoveIntentionally, false);
            }
            if (properties.affectCharacterMove != sf::Vector2i(0, 0))
            {
                character.addToListOfBlocksAffectingMove(id);
            }
            if (properties.isFinishTrigger)
            {
                character.setStatus(Character::Status::hasTriggeredFinishBlock, true);
            }
        }
        wasInCollideLastFrame = isColliding;
    }

    return isColliding;
}

sf::IntRect Block::getCollideBox() const
{
    return sf::IntRect(position.x + spriteInfos.margin.x, position.y + spriteInfos.margin.y, spriteInfos.size.x,
                       spriteInfos.size.y);
}

const BlockProperties Block::getBlockInfo()
{
    return properties;
}

bool Block::getWasInCollideLastFrame()
{
    return wasInCollideLastFrame;
}

void Block::setPosition(sf::Vector2i newPosition)
{
    position = newPosition;
    sprite.setPosition(position.x + spriteInfos.margin.x, position.y + spriteInfos.margin.y);
}

void Block::setPosition(int newX, int newY)
{
    position = sf::Vector2i(newX, newY);
    sprite.setPosition(newX + spriteInfos.margin.x, newY + spriteInfos.margin.y);
}

void Block::setCollisionForVersion()
{
    collision.setFuncsForGameVersion(Global::versionOfGame);
}
