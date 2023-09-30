#include "global.h"
//
// *form a class for the objects and a vector to hold them
// * apply gravity
// * image gets cycled through and makes a silouette into another image based on "non-transparent" pixels (alpha<128)
// * make pixel collision
// * mask the objects
// 
//

sf::Image GameClass::makeSilouette(sf::Image& original)
{
    sf::Image ret;
    ret.create(original.getSize().x, original.getSize().y, sf::Color::White);
    for (auto y = 0; y < original.getSize().y; y++)
    {
        for (auto x = 0; x < original.getSize().x; x++)
        {
            if (original.getPixel(x, y).a < 128U)
                ret.setPixel(x, y, sf::Color::Black);
        }
    }
    return ret;
}

void GameClass::handleCollision(clutter A, clutter B) {
    sf::FloatRect thisBounds = A.sprite.getGlobalBounds();
    sf::FloatRect otherBounds = B.sprite.getGlobalBounds();

    if (thisBounds.intersects(otherBounds)) {
        // Simple elastic collision response
        sf::Vector2f relativeVelocity = A.velocity - B.velocity;
        sf::Vector2f collisionNormal = ic::normalVector(sf::Vector2f(
            thisBounds.left + thisBounds.width / 2.0f - (otherBounds.left + otherBounds.width / 2.0f),
            thisBounds.top + thisBounds.height / 2.0f - (otherBounds.top + otherBounds.height / 2.0f)
        ));
        float dotProduct = relativeVelocity.x * collisionNormal.x + relativeVelocity.y * collisionNormal.y;

        // Check if objects are moving towards each other
        if (dotProduct < 0) {
            // Calculate new velocities
            float combinedMass = A.weight + B.weight;
            sf::Vector2f impulse = (-(1.0f + A.bounciness) * dotProduct) / combinedMass * collisionNormal;
            A.velocity += impulse;
            B.velocity -= impulse;
        }
    }
}

void GameClass::applyGravity(clutter A)
{
    sf::Vector2f grav = { 0.0f, GRAVITY };
    A.velocity += grav;
}


GameClass::GameClass(const ic::gameScreen _screen)
{
    playerImg.loadFromFile("assets/sampleplayer.png");
    playerTex.loadFromImage(playerImg);
    playerSprite.setTexture(playerTex, true);
    playerSprite.setOrigin(16.0f, 16.0f);
    playerSprite.setPosition(32.0f, 32.0f);
    playerVelocity = { 30.0f, 30.0f };
    objectImg.loadFromFile("assets/samplebox.png");
    objectTex.loadFromImage(objectImg);
    objectSprite.setTexture(objectTex, true);
    objectSprite.setOrigin(16.0f, 16.0f);
    objectSprite.setPosition(32.0f, 32.0f);
    objectVelocity = { -60.0f, 30.0f };
    for (auto ii = 0; ii < 20; ii++)
    {
        clutter cl;
        cl.velocity = sf::Vector2f((float)(ic::random(randomCount++, 12345)%60)+20.0f, (float)(ic::random(randomCount++, 12345)%60)+20.0f);
        cl.image.loadFromFile("assets/samplebox.png");
        cl.texture.loadFromImage(cl.image);
        cl.sprite.setTexture(objectTex, true);
        cl.centerPoint = sf::Vector2f(16.0f, 16.0f);
        cl.sprite.setOrigin(cl.centerPoint);
        cl.sprite.setPosition(sf::Vector2f((float)(ic::random(randomCount++, 12345) % 216) + 20.0f, (float)(ic::random(randomCount++, 12345) % 152) + 20.0f));
        cl.radius = 16.0f;
        cl.accel = { 0.0f, 0.0f };
        cl.isActive = true;
        cl.silo = makeSilouette(cl.image);
        cl.weight = 1.0f;
        clutterPile.push_back(cl);
    }
    rectangle = { sf::Vector2f(60, 60), 1.0f, sf::Vector2f(50, 30) , 0.5f};
    junk.setColor(sf::Color::Magenta);

	structureImg.loadFromFile("assets/background_sms.png");
	structureTex.loadFromImage(structureImg);
	structureSprite.setTexture(structureTex, true);

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
            //junk.velocity = {5.1f, -2.0f};
            //junk.position = { 60.0f, 110.0f };
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
    //update routine
    deltaTime = dtClock.restart().asSeconds();

    for (size_t iterk=0; iterk<clutterPile.size(); iterk++)
    {
        clutter K = clutterPile[iterk];
        applyGravity(K);
        K.velocity += K.accel;

        for (size_t iterl = iterk + 1; iterl < clutterPile.size(); iterl++)
        {
            handleCollision(K, clutterPile[iterl]);
        }
        
        K.sprite.move(K.velocity);
    }

 //rendering routine
    applicationWindow.app.clear();
    applicationWindow.app.draw(structureSprite);
    //applicationWindow.app.draw(playerSprite);
    for (auto& k : clutterPile)
    {
        applicationWindow.app.draw(k.sprite);
    }
    applicationWindow.app.display();

    return true;

}
