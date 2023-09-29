#include "global.h"


GameClass::GameClass(const ic::gameScreen _screen)
{
	playerImg.loadFromFile("assets/player32x.png");
	playerTex.loadFromImage(playerImg);
	playerSprite.setTexture(playerTex, true);
    playerSprite.setOrigin(16.0f, 16.0f);
    playerSprite.setPosition(32.0f, 32.0f);
    playerVelocity = { 30.0f, 30.0f };

	structureImg.loadFromFile("assets/background_sms.png");
	structureTex.loadFromImage(structureImg);
	structureSprite.setTexture(structureTex, true);
    structureSprite.setColor(sf::Color(78, 78, 78, 255));

    screenRect = applicationWindow.canvas.canvasToRect();
}

GameClass::~GameClass()
{

}

bool GameClass::gameLoop()
{
    sf::Event event;
    while (applicationWindow.app.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            applicationWindow.app.close();
            return false;
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            isClicking = true;
            sf::Vector2f mousePos = applicationWindow.transformMouse(applicationWindow.app);
        }
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::F11)
            {
                if (applicationWindow.isFullScreen)
                {
                    applicationWindow.launchWindow("SFML Barebones (feyleaf.com)");
                }
                else
                {
                    applicationWindow.launchFullScreen("SFML Barebones Fullscreen (feyleaf.com)");
                }
            }
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {
            isClicking = false;
        }
        if (event.type == sf::Event::Resized)
        {
            applicationWindow.renewView(event.size.width, event.size.height);
            screenRect = applicationWindow.canvas.canvasToRect();
        }
    }
    deltaTime = dtClock.restart().asSeconds();
    
        //update routine
    if (playerSprite.getPosition().x+(0.5f*playerSprite.getGlobalBounds().width) > screenRect.width || playerSprite.getPosition().x - (0.5f * playerSprite.getGlobalBounds().width) < screenRect.left)
    {
        playerVelocity.x *= -1.0f;
        if (playerVelocity.x < 0.0f)
            playerSprite.setScale(-1.0f, 1.0f);
        else
            playerSprite.setScale(1.0f, 1.0f);
    }
    if (playerSprite.getPosition().y + (0.5f * playerSprite.getGlobalBounds().height) > screenRect.height || playerSprite.getPosition().y - (0.5f * playerSprite.getGlobalBounds().height) < screenRect.top)
    {
        playerVelocity.y *= -1.0f;
    }
    playerSprite.move(playerVelocity * deltaTime);


        //rendering routine
        applicationWindow.app.clear();
        applicationWindow.app.draw(structureSprite);
        applicationWindow.app.draw(playerSprite);
        applicationWindow.app.display();
    
    return true;
}
