#ifndef MESSAGESTATE_HPP
#define MESSAGESTATE_HPP

#include "gameState.hpp"

struct RectForMessage
{
    RectForMessage() : endOfIncrease(false), timeUntilStart(0), timeUntilHide(0)
    {
    }
    bool endOfIncrease;
    float timeUntilStart;
    float timeUntilHide;
    sf::Vector2i origin;
    sf::Vector2i trueSize;
    sf::Vector2i speedIncrease;
    sf::Vector2i speedDecrease;
    sf::RectangleShape rect;
};

class MessageState : public GameState
{
public:
    MessageState(std::vector<RectForMessage>& newListOfRect, sf::Text newMessageToShow);

private:
    void updateImpl(sf::RenderWindow& window) override;
    void drawImpl(sf::RenderWindow& window) const override;
    bool changeSizeOfRect(RectForMessage& rect, sf::Vector2i speed, sf::Vector2i endSize, int factor = 1);

private:
    sf::Clock time;
    bool timeToLeave;
    bool showMessage;
    unsigned int numberOfEndOfIncrease;
    std::vector<RectForMessage> listOfRect;
    sf::Text messageToShow;
};

#endif
