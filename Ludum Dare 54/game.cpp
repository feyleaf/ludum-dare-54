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
    randomCount++;
    ret.saveToFile("assets/silo-" + sf::String(randomCount) + ".png");
    return ret;
}

void GameClass::handleCollision(clutter& A, clutter& B) {
    sf::FloatRect thisBounds = A.sprite.getGlobalBounds();
    sf::FloatRect otherBounds = B.sprite.getGlobalBounds();
    sf::Image asilo = A.silo;
    sf::Image bsilo = B.silo;
    sf::Vector2i apos = { int(A.sprite.getPosition().x), int(A.sprite.getPosition().y) };
    sf::Vector2i bpos = { int(B.sprite.getPosition().x), int(B.sprite.getPosition().y) };
    sf::FloatRect screenBounds = { 0.0f, 0.0f, 256.0f, 192.0f };
    //if (screenBounds.contains(A.sprite.getPosition()) && screenBounds.contains(B.sprite.getPosition()))
    //{
    if (thisBounds.intersects(otherBounds))
    {
        //check pixel perfect collider
        bool isPPCollide = false;
        if (A.radius >= B.radius)
        {
            for (int x = 0; x < asilo.getSize().x; x++)
            {
                for (int y = 0; y < asilo.getSize().y; y++)
                {
                    if (asilo.getPixel(x, y) == sf::Color::White)
                    {
                        if (!isPPCollide)
                        {
                            /*
                            apos.x+x is the pixel on screen
                            bpos.x+x is the other pixel on screen
                            they are offset by
                            let's see
                            a=100,20 b=110,21
                            x=8
                            y=2
                            apix=108,22
                            bpix=118,23
                            we want bpix 108,22
                            so be subtract apix from bpix
                            10,1
                            offx=10
                            offy=1
                            */
                            sf::Vector2i offset = { (apos.x - bpos.x) + x, (apos.y - bpos.y) + y };
                            if (!(offset.x<0 || offset.x>bsilo.getSize().x || offset.y<0 || offset.y>bsilo.getSize().y))
                            {
                                if (bsilo.getPixel(offset.x, offset.y) == sf::Color::White)
                                {
                                    isPPCollide = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (A.radius < B.radius)
        {
            for (int x = 0; x < bsilo.getSize().x; x++)
            {
                for (int y = 0; y < bsilo.getSize().y; y++)
                {
                    if (bsilo.getPixel(x, y) == sf::Color::White)
                    {
                        if (!isPPCollide)
                        {
                            /*
                            apos.x+x is the pixel on screen
                            bpos.x+x is the other pixel on screen
                            they are offset by
                            let's see
                            a=100,20 b=110,21
                            x=8
                            y=2
                            apix=108,22
                            bpix=118,23
                            we want bpix 108,22
                            so be subtract apix from bpix
                            10,1
                            offx=10
                            offy=1
                            */
                            sf::Vector2i offset = { (bpos.x - apos.x) + x, (bpos.y - apos.y) + y };
                            if (!(offset.x<0 || offset.x>asilo.getSize().x || offset.y<0 || offset.y>asilo.getSize().y))
                            {
                                if (asilo.getPixel(offset.x, offset.y) == sf::Color::White)
                                {
                                    isPPCollide = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (isPPCollide)
        {
            if (ic::calcMagnitude(A.velocity) < 0.1f && ic::calcMagnitude(B.velocity) < 0.1f)
            {
                A.velocity = { 0.0f, 0.0f };
                B.velocity = { 0.0f, 0.0f };
            }
            else
            {

                // Simple elastic collision response
                sf::Vector2f relativeVelocity = A.velocity - B.velocity;
                sf::Vector2f collisionNormal = ic::normalVector(sf::Vector2f(
                    thisBounds.left + thisBounds.width / 2.0f - (otherBounds.left + otherBounds.width / 2.0f),
                    thisBounds.top + thisBounds.height / 2.0f - (otherBounds.top + otherBounds.height / 2.0f)
                ));
                float dotProduct = relativeVelocity.x * collisionNormal.x + relativeVelocity.y * collisionNormal.y;
                if (abs(dotProduct) < 0.1f) dotProduct = 0.0f;
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
    }
    //}
    //else
    if (A.sprite.getPosition().x + A.radius >= 256.0f || A.sprite.getPosition().x - A.radius <= 0.0f)
    {
        A.velocity.x = 0.0f;
        if (A.sprite.getPosition().x + A.radius >= 256.0f)
        {
            A.velocity.x = -1.0f;
        }
        if (A.sprite.getPosition().x - A.radius <=0.0f)
        {
            A.velocity.x = 1.0f;
        }
    }
    if (A.sprite.getPosition().y + A.radius >= 192.0f || A.sprite.getPosition().y - A.radius <= 0.0f)
    {
        A.friction = 0.1f;
        A.velocity.y = 0.0f;
        if (A.sprite.getPosition().y + A.radius >= 192.0f)
        {
            A.velocity.y = -1.0f;

        }
        if (A.sprite.getPosition().y - A.radius <= 0.0f)
        {
            A.velocity.y = 1.0f;

        }
    }
    if (B.sprite.getPosition().x + B.radius >= 256.0f || B.sprite.getPosition().x - B.radius <= 0.0f)
    {
        B.velocity.x = 0.0f;
        if (B.sprite.getPosition().x + B.radius >= 256.0f)
        {
            B.velocity.x = -1.0f;
        }
        if (B.sprite.getPosition().x - B.radius <= 0.0f)
        {
            B.velocity.x = 1.0f;
        }
    }
    if (B.sprite.getPosition().y + B.radius >= 192.0f || B.sprite.getPosition().y - B.radius <= 0.0f)
    {
        B.friction = 0.1f;
        B.velocity.y = 0.0f;
        if (B.sprite.getPosition().y + B.radius >= 192.0f)
        {
            B.velocity.y = -1.0f;
        }
        if (B.sprite.getPosition().y - B.radius <= 0.0f)
        {
            B.velocity.y = 1.0f;
        }
    }
}

void GameClass::applyGravity(clutter& A, float _dt)
{
    sf::Vector2f grav = { 0.0f, GRAVITY *_dt };
    A.accel += grav;
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
    bearTexture.loadFromFile("assets/teddy.png");
    burritoTexture.loadFromFile("assets/burrito.png");
    couchTex.loadFromFile("assets/couch.png");
    tableTex.loadFromFile("assets/table.png");
    objectSprite.setTexture(objectTex, true);
    objectSprite.setOrigin(16.0f, 16.0f);
    objectSprite.setPosition(32.0f, 32.0f);
    objectVelocity = { -60.0f, 30.0f };
    for (auto ii = 0; ii < 20; ii++)
    {
        clutter cl;
        cl.velocity = { 0.0f, -1.0f };
        switch(ic::random(randomCount++, 12345) % 10)
        {
        case 1:
            cl.centerPoint = sf::Vector2f(16.0f, 16.0f);
            cl.image.loadFromFile("assets/samplebox.png");
            cl.texture.loadFromImage(cl.image);
            cl.sprite.setTexture(objectTex, true);
            cl.weight = 1.0f;
            cl.radius = 16.0f;

            break;
        case 2:
            cl.centerPoint = sf::Vector2f(16.0f, 16.0f);
            cl.image.loadFromFile("assets/teddy.png");
            cl.texture.loadFromImage(cl.image);
            cl.sprite.setTexture(bearTexture, true);
            cl.weight = 1.0f;
            cl.radius = 16.0f;
            break;
        case 3:
            cl.centerPoint = sf::Vector2f(32.0f, 32.0f);
            cl.image.loadFromFile("assets/couch.png");
            cl.texture.loadFromImage(cl.image);
            cl.sprite.setTexture(couchTex, true);
            cl.weight = 50.0f;
            cl.radius = 32.0f;
            break;
        case 4:
            cl.centerPoint = sf::Vector2f(32.0f, 32.0f);
            cl.image.loadFromFile("assets/table.png");
            cl.texture.loadFromImage(cl.image);
            cl.sprite.setTexture(tableTex, true);
            cl.weight = 40.0f;
            cl.radius = 32.0f;
            break;
        default:
            cl.centerPoint = sf::Vector2f(16.0f, 16.0f);
            cl.image.loadFromFile("assets/burrito.png");
            cl.texture.loadFromFile("assets/burrito.png");
            cl.sprite.setTexture(burritoTexture, true);
            cl.weight = 0.4f;
            cl.radius = 16.0f;
            break;

        }
        cl.sprite.setOrigin(cl.centerPoint);
        cl.sprite.setPosition(sf::Vector2f((float)(ic::random(randomCount++, 12345) % 216) + 20.0f, (float)(ic::random(randomCount++, 12345) % 152) + 20.0f));
        cl.accel = { 0.0f, -1.0f };
        cl.isActive = true;
        cl.silo = makeSilouette(cl.image);
        cl.bounciness = 0.1f;
        cl.friction = 0.99f;
        clutterPile.push_back(cl);
    }
    player.image.loadFromFile("assets/sampleplayer.png");
    player.texture.loadFromImage(playerImg);
    player.sprite.setTexture(playerTex);
    player.velocity = { 0.0f, 0.0f };
    player.centerPoint = { 16.0f, 16.0f };
    player.sprite.setOrigin(player.centerPoint);
    player.sprite.setPosition(player.centerPoint * 2.0f);
    player.radius = 16.0f;
    player.accel = { 0.0f, 0.0f };
    player.isActive = true;
    player.silo = makeSilouette(player.image);
    player.weight = 10.0f;
    player.bounciness = 1.0f;

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
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                for (auto& k : clutterPile)
                {
                    k.sprite.setPosition(sf::Vector2f((float)(ic::random(randomCount++, 12345) % 216) + 20.0f, (float)(ic::random(randomCount++, 12345) % 152) + 20.0f));
                }
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                clutter cl;
                cl.velocity = { 4.0f, -16.0f };
                cl.image.loadFromFile("assets/samplebox.png");
                cl.texture.loadFromImage(cl.image);
                cl.sprite.setTexture(objectTex, true);
                cl.centerPoint = sf::Vector2f(16.0f, 16.0f);
                cl.sprite.setOrigin(cl.centerPoint);
                cl.sprite.setPosition(mousePos);
                cl.radius = 16.0f;
                cl.accel = { 0.0f, -0.0f };
                cl.isActive = true;
                cl.silo = makeSilouette(cl.image);
                cl.weight = 1.0f;
                cl.bounciness = 0.8f;
                cl.friction = 0.999f;
                clutterPile.push_back(cl);

            }
            //junk.velocity = {5.1f, -2.0f};
            //junk.position = { 60.0f, 110.0f };
        }
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::F11)
            {
                if (applicationWindow.isFullScreen)
                {
                    applicationWindow.launchWindow("My Limit (feyleaf.com)");
                }
                else
                {
                    applicationWindow.launchFullScreen("My Limit Fullscreen (feyleaf.com)");
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.velocity.x = 20.0f;
            player.sprite.setScale(sf::Vector2f(1.0f, 1.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            player.velocity.y = -10.0f;
            player.friction = 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player.velocity.x = -20.0f;
            player.sprite.setScale(sf::Vector2f(-1.0f, 1.0f));
        }

    }
    //update routine
    deltaTime = dtClock.restart().asSeconds();

    for (size_t iterk=0; iterk<clutterPile.size(); iterk++)
    {
        for (size_t iterl = iterk + 1; iterl < clutterPile.size(); iterl++)
        {
            handleCollision(clutterPile[iterk], clutterPile[iterl]);
        }
        
    }
    player.friction = 0.999f;
    
    int colliderCount = 0;
    for (auto& k : clutterPile)
    {
        k.velocity.x *= k.friction;
        applyGravity(k, deltaTime);
        k.velocity += k.accel * deltaTime;
        k.sprite.move(k.velocity * deltaTime);
        handleCollision(k, player);
        if (player.sprite.getGlobalBounds().intersects(k.sprite.getGlobalBounds()))
        {
            colliderCount++;
        }
    }
    if (colliderCount > 5 && player.sprite.getPosition().y>180)
    {
        player.sprite.setColor(sf::Color::Red);
    }
    applyGravity(player, deltaTime);
    player.velocity += player.accel * deltaTime;
    player.velocity.x *= player.friction;
    player.sprite.move(player.velocity * deltaTime);

 //rendering routine
    applicationWindow.app.clear();
    applicationWindow.app.draw(structureSprite);
    for (auto& k : clutterPile)
    {
        applicationWindow.app.draw(k.sprite);
    }
    applicationWindow.app.draw(player.sprite);
    applicationWindow.app.display();

    return true;

}
