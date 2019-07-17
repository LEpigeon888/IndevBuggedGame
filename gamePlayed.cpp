#include "gamePlayed.hpp"
#include "messageManager.hpp"
#include "playState.hpp"
#include "screenTransitionState.hpp"
#include "global.hpp"

gamePlayedClass::gamePlayedClass(std::string nameOfLevel)
{
    view.setSize(WIDTH_SCREEN, HEIGHT_SCREEN);
    view.setCenter(WIDTH_SCREEN / 2, HEIGHT_SCREEN / 2);

    currentLevelName = nameOfLevel;
    levelManagerClass::loadLevelFromFile(infoForLevel, nameOfLevel);
    global::versionOfGame = infoForLevel.initialGameVersion;
    playerPlay.setPosition(infoForLevel.playerStartPosition);
    updateGameVersion();
    playerPlay.update();
}

void gamePlayedClass::update()
{
    bool continueMove = false;

    do
    {
        continueMove = playerPlay.applyMove();
        checkCharacterInBorder(playerPlay);
        checkCharacterCollideWithBlock(playerPlay, playerPlay.getDirection(), true);
        setCameraToCharacter(playerPlay);
    }
    while(continueMove);

    do
    {
        continueMove = playerPlay.applyVerticalMove();
        checkCharacterInBorder(playerPlay);
        checkCharacterCollideWithBlock(playerPlay, playerPlay.getVerticalDirection(), true);
        setCameraToCharacter(playerPlay);
    }
    while(continueMove);

    checkCharacterInBorder(playerPlay);
    if(checkCharacterCollideWithBlock(playerPlay, playerPlay.getDirection()))
    {
        setCameraToCharacter(playerPlay);
        return;
    }
    setCameraToCharacter(playerPlay);
    checkCharacterCollideWithEvent(playerPlay);

    playerPlay.attractTo(sf::Vector2i(0, GRAVITY));
    playerPlay.update();

    listOfForegroundBlock.clear();
    for(int x = (view.getCenter().x - (WIDTH_SCREEN / 2)) / SIZE_BLOCK; x < (view.getCenter().x + (WIDTH_SCREEN / 2)) / SIZE_BLOCK; ++x)
    {
        for(int y = (view.getCenter().y - (HEIGHT_SCREEN / 2)) / SIZE_BLOCK; y < (view.getCenter().y + (HEIGHT_SCREEN / 2)) / SIZE_BLOCK; ++y)
        {
            auto block = infoForLevel.mapOfGame.find(point(x, y));
            if(block != infoForLevel.mapOfGame.end())
            {
                block->second->update();
                if(block->second->getBlockInfo().isForeGroundBlock)
                {
                    listOfForegroundBlock.push_back(block->second.get());
                }
            }
        }
    }

    if(playerPlay.getIsDead())
    {
        global::activeGameStateStack->add(new screenTransitionStateClass(new playStateClass(currentLevelName), sf::Color::Black, 25));
    }
}

void gamePlayedClass::draw(sf::RenderWindow& window)
{
    window.setView(view);

    for(int x = (view.getCenter().x - (WIDTH_SCREEN / 2)) / SIZE_BLOCK; x < (view.getCenter().x + (WIDTH_SCREEN / 2)) / SIZE_BLOCK; ++x)
    {
        for(int y = (view.getCenter().y - (HEIGHT_SCREEN / 2)) / SIZE_BLOCK; y < (view.getCenter().y + (HEIGHT_SCREEN / 2)) / SIZE_BLOCK; ++y)
        {
            auto block = infoForLevel.mapOfGame.find(point(x, y));
            if(block != infoForLevel.mapOfGame.end())
            {
                block->second->draw(window);
            }
        }
    }

    playerPlay.draw(window);

    for(blockClass* thisBlock : listOfForegroundBlock)
    {
        thisBlock->draw(window);
    }
}

void gamePlayedClass::movePlayerTo(direction dir)
{
    playerPlay.setMoveTo(dir);
}

void gamePlayedClass::jumpPlayer()
{
    playerPlay.startJump();
}

void gamePlayedClass::checkCharacterInBorder(characterClass& character)
{
    if(character.getPosition().x < infoForLevel.limitOfGame.left)
    {
        character.hasEnterInCollide(LEFT);
        character.setPosition(infoForLevel.limitOfGame.left, character.getPosition().y);
    }
    else if(character.getPosition().x + character.getCollideBox().width > infoForLevel.limitOfGame.left + infoForLevel.limitOfGame.width)
    {
        character.hasEnterInCollide(RIGHT);
        character.setPosition(infoForLevel.limitOfGame.left + infoForLevel.limitOfGame.width - character.getCollideBox().width, character.getPosition().y);
    }

    if(character.getPosition().y < infoForLevel.limitOfGame.top)
    {
        character.hasEnterInCollide(UP);
        character.setPosition(character.getPosition().x, infoForLevel.limitOfGame.top);
    }
    else if(character.getPosition().y + character.getCollideBox().height > infoForLevel.limitOfGame.top + infoForLevel.limitOfGame.height)
    {
        character.hasEnterInCollide(DOWN);
        character.setPosition(character.getPosition().x, infoForLevel.limitOfGame.top + infoForLevel.limitOfGame.height - character.getCollideBox().height);
    }
}

bool gamePlayedClass::checkCharacterCollideWithBlock(characterClass& character, direction dir, bool onlySolid)
{
    for(int x = character.getCollideBox().left / SIZE_BLOCK; x < (character.getCollideBox().left + character.getCollideBox().width) / SIZE_BLOCK; ++x)
    {
        for(int y = character.getCollideBox().top / SIZE_BLOCK; y < (character.getCollideBox().top + character.getCollideBox().height) / SIZE_BLOCK; ++y)
        {
            auto block = infoForLevel.mapOfGame.find(point(x, y));

            if(block != infoForLevel.mapOfGame.end())
            {
                if(block->second->getBlockInfo().isOnlyOneBlock && block->second->getOldCollide())
                {
                    block->second->isCollideWith(character.getCollideBox(), dir);
                }
                else
                {
                    if(onlySolid)
                    {
                        if(block->second->getBlockInfo().isSolidBlock && block->second->isCollideWith(character.getCollideBox(), dir))
                        {
                            character.hasEnterInCollide(dir);
                            character.setPosition(block->second->getNewPosOf(character.getCollideBox(), dir));
                        }
                    }
                    else if(block->second->isCollideWith(character.getCollideBox(), dir))
                    {
                        if(block->second->getBlockInfo().isKillPlayerBlock)
                        {
                            character.setIsDead(true);
                        }
                        if(block->second->getBlockInfo().isFinishBlock)
                        {
                            if(infoForLevel.nextLevelName.empty())
                            {
                                infoForLevel.nextLevelName = currentLevelName;
                            }
                            global::activeGameStateStack->add(new screenTransitionStateClass(new playStateClass(infoForLevel.nextLevelName), sf::Color::Black, 25));
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

void gamePlayedClass::checkCharacterCollideWithEvent(characterClass& character)
{
    for(auto eventIte = infoForLevel.listOfEvent.begin(); eventIte != infoForLevel.listOfEvent.end(); )
    {
        if((*eventIte)->isCollideWith(character.getCollideBox()))
        {
            if((*eventIte)->getEventInfo().isUpdateEvent)
            {
                global::versionOfGame = (*eventIte)->getEventInfo().newVersion;

                updateGameVersion();
            }
            if((*eventIte)->getEventInfo().isShowMessageEvent)
            {
                messageManagerClass::addMessageStateToStack("NORMAL_MESSAGE", (*eventIte)->getEventInfo().messageToShow);
                infoForLevel.listOfEvent.erase(eventIte++);
                break;
            }
            infoForLevel.listOfEvent.erase(eventIte++);
            continue;
        }
        ++eventIte;
    }
}

void gamePlayedClass::setCameraToCharacter(characterClass& character)
{
    view.setCenter(character.getSpriteBox().left + character.getSpriteBox().width / 2, character.getSpriteBox().top + character.getSpriteBox().height / 2);

    if(view.getCenter().x < infoForLevel.limitOfGame.left + (WIDTH_SCREEN / 2))
    {
        view.setCenter(infoForLevel.limitOfGame.left + (WIDTH_SCREEN / 2), view.getCenter().y);
    }
    else if(view.getCenter().x > infoForLevel.limitOfGame.left + infoForLevel.limitOfGame.width - (WIDTH_SCREEN / 2))
    {
        view.setCenter(infoForLevel.limitOfGame.left + infoForLevel.limitOfGame.width - (WIDTH_SCREEN / 2), view.getCenter().y);
    }

    if(view.getCenter().y < infoForLevel.limitOfGame.top + (HEIGHT_SCREEN / 2))
    {
        view.setCenter(view.getCenter().x, infoForLevel.limitOfGame.top + (HEIGHT_SCREEN / 2));
    }
    else if(view.getCenter().y > infoForLevel.limitOfGame.top + infoForLevel.limitOfGame.height - (HEIGHT_SCREEN / 2))
    {
        view.setCenter(view.getCenter().x, infoForLevel.limitOfGame.top + infoForLevel.limitOfGame.height - (HEIGHT_SCREEN / 2));
    }
}

void gamePlayedClass::updateGameVersion()
{
    for(auto& thisBlock : infoForLevel.mapOfGame)
    {
        thisBlock.second->setCollisionForVersion();
    }

    playerPlay.setMovementForVersion();
}
