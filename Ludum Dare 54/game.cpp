#include "global.h"
//
// *form a class for the objects and a vector to hold them
// * apply gravity
// * image gets cycled through and makes a silouette into another image based on "non-transparent" pixels (alpha<128)
// * make pixel collision
// * mask the objects
// 
//

#include "global.h"

// Constants
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 192;
//const int GRAVITY = 9.8; // Replace with your desired gravity value

sf::Image GameClass::makeSilouette(sf::Image& original)
{
    sf::Image ret;
    ret.create(original.getSize().x, original.getSize().y, sf::Color::White);

    for (int y = 0; y < original.getSize().y; y++) {
        for (int x = 0; x < original.getSize().x; x++) {
            if (original.getPixel(x, y).a < 128U) {
                ret.setPixel(x, y, sf::Color::Black);
            }
        }
    }

    randomCount++;
    ret.saveToFile("assets/silo-" + sf::String(std::to_string(randomCount)) + ".png");
    return ret;
}

void GameClass::handleCollision(clutter& A, clutter& B) {
    sf::FloatRect thisBounds = A.sprite.getGlobalBounds();
    sf::FloatRect otherBounds = B.sprite.getGlobalBounds();

    // Check if the bounding boxes of the two objects intersect
    if (thisBounds.intersects(otherBounds)) {
        // Calculate the collision rectangle between the two objects
        sf::FloatRect collisionRect;
        if (thisBounds.left < otherBounds.left) {
            collisionRect.left = otherBounds.left;
            collisionRect.width = std::min(thisBounds.left + thisBounds.width, otherBounds.left + otherBounds.width) - collisionRect.left;
        }
        else {
            collisionRect.left = thisBounds.left;
            collisionRect.width = std::min(thisBounds.left + thisBounds.width, otherBounds.left + otherBounds.width) - collisionRect.left;
        }
        if (thisBounds.top < otherBounds.top) {
            collisionRect.top = otherBounds.top;
            collisionRect.height = std::min(thisBounds.top + thisBounds.height, otherBounds.top + otherBounds.height) - collisionRect.top;
        }
        else {
            collisionRect.top = thisBounds.top;
            collisionRect.height = std::min(thisBounds.top + thisBounds.height, otherBounds.top + otherBounds.height) - collisionRect.top;
        }
        A.box.setPosition(A.sprite.getPosition()-A.sprite.getOrigin());
        A.box.setSize(A.sprite.getGlobalBounds().getSize());
        A.box.setFillColor(sf::Color(255, 255, 255, 0));
        A.box.setOutlineColor(sf::Color::Cyan);
        A.box.setOutlineThickness(1.0f);
        B.box.setPosition(B.sprite.getPosition());
        B.box.setSize(collisionRect.getSize());
        B.box.setFillColor(sf::Color(255, 255, 255, 0));
        B.box.setOutlineColor(sf::Color::Magenta);
        B.box.setOutlineThickness(1.0f);


        // Check pixel-perfect collision within the collision rectangle
        bool isPPCollide = false;
        sf::Image asilo = A.silo;
        sf::Image bsilo = B.silo;
        //sf::Vector2i apos = { static_cast<int>(A.sprite.getPosition().x-A.radius), static_cast<int>(A.sprite.getPosition().y-A.radius) };
        //sf::Vector2i bpos = { static_cast<int>(B.sprite.getPosition().x-B.radius), static_cast<int>(B.sprite.getPosition().y-B.radius) };

        sf::Vector2i apos = { static_cast<int>(A.sprite.getPosition().x), static_cast<int>(A.sprite.getPosition().y) };
        sf::Vector2i bpos = { static_cast<int>(B.sprite.getPosition().x), static_cast<int>(B.sprite.getPosition().y) };

        for (int x = static_cast<int>(collisionRect.left); x < static_cast<int>(collisionRect.left + collisionRect.width); x++) {
            for (int y = static_cast<int>(collisionRect.top); y < static_cast<int>(collisionRect.top + collisionRect.height); y++) {
                sf::Vector2i aOffset = { x - apos.x, y - apos.y };
                sf::Vector2i bOffset = { x - bpos.x, y - bpos.y };

                // Check pixel-perfect collision for both A and B using their on-screen positions
                if (aOffset.x >= 0 && aOffset.x < asilo.getSize().x && aOffset.y >= 0 && aOffset.y < asilo.getSize().y &&
                    bOffset.x >= 0 && bOffset.x < bsilo.getSize().x && bOffset.y >= 0 && bOffset.y < bsilo.getSize().y) {
                    if (asilo.getPixel(aOffset.x, aOffset.y) == sf::Color::White && bsilo.getPixel(bOffset.x, bOffset.y) == sf::Color::White) {
                        isPPCollide = true;
                        A.sprite.setColor(sf::Color::Yellow);
                        break;
                    }
                }
            }
            if (isPPCollide) {
                break;
            }
        }


        //for (int x = static_cast<int>(collisionRect.left); x < static_cast<int>(collisionRect.left + collisionRect.width); x++) {
        //    for (int y = static_cast<int>(collisionRect.top); y < static_cast<int>(collisionRect.top + collisionRect.height); y++) {
        //        sf::Vector2i offset = { x - apos.x + static_cast<int>(collisionRect.left) - bpos.x + static_cast<int>(collisionRect.left), y - apos.y + static_cast<int>(collisionRect.top) - bpos.y + static_cast<int>(collisionRect.top) };

        //        //sf::Vector2i offset = { x - apos.x + static_cast<int>(collisionRect.left), y - bpos.y + static_cast<int>(collisionRect.top) };
        //        if (offset.x >= 0 && offset.x < asilo.getSize().x && offset.y >= 0 && offset.y < asilo.getSize().y) {
        //            if (asilo.getPixel(offset.x, offset.y) == sf::Color::White && bsilo.getPixel(offset.x, offset.y) == sf::Color::White) {
        //                isPPCollide = true;
        //                break;
        //            }
        //        }
        //    }
        //    if (isPPCollide) {

        //        break;
        //    }
        //}

        if (isPPCollide) {
            handleCollisionResponse(A, B);
        }
    }
    else {
        handleOutOfScreen(A);
        handleOutOfScreen(B);
    }
}

void GameClass::handleCollisionResponse(clutter& A, clutter& B) {
    // Calculate relative velocity
    sf::Vector2f relativeVelocity = A.velocity - B.velocity;
    sf::FloatRect thisBounds = A.sprite.getGlobalBounds();
    sf::FloatRect otherBounds = B.sprite.getGlobalBounds();


    // Calculate collision normal
    sf::Vector2f collisionNormal = ic::normalVector(sf::Vector2f(
        thisBounds.left + thisBounds.width / 2.0f - (otherBounds.left + otherBounds.width / 2.0f),
        thisBounds.top + thisBounds.height / 2.0f - (otherBounds.top + otherBounds.height / 2.0f)
    ));

    // Calculate dot product
    float dotProduct = relativeVelocity.x * collisionNormal.x + relativeVelocity.y * collisionNormal.y;
    // Calculate the minimum translation distance to separate the objects
    float overlap = std::min(thisBounds.width + otherBounds.width - std::abs(thisBounds.left - otherBounds.left),
        thisBounds.height + otherBounds.height - std::abs(thisBounds.top - otherBounds.top));

    // Calculate the separation vector along the collision normal
    sf::Vector2f separation = ic::normalVector(collisionNormal * (overlap / 2.0f));

    // Move the objects apart
    A.velocity += separation;
    B.velocity -= separation;

    // Calculate new velocities (similar to your existing code)
    float combinedMass = A.weight + B.weight;
    sf::Vector2f impulse = (-(1.0f + A.bounciness) * dotProduct) / combinedMass * collisionNormal;
    A.velocity += ic::normalVector(impulse);
    B.velocity -= ic::normalVector(impulse);

    // Check if objects are moving towards each other
    if (dotProduct < 0) {
        // Calculate new velocities
        float combinedMass = A.weight + B.weight;
        sf::Vector2f impulse = (-(1.0f + A.bounciness) * dotProduct) / combinedMass * collisionNormal;
        A.velocity += ic::normalVector(impulse);
        B.velocity -= ic::normalVector(impulse);
    }
}
//void GameClass::handleCollisionResponse(clutter& A, clutter& B) {
//    // Calculate relative velocity
//    sf::Vector2f relativeVelocity = A.velocity - B.velocity;
//    sf::FloatRect thisBounds = A.sprite.getGlobalBounds();
//    sf::FloatRect otherBounds = B.sprite.getGlobalBounds();
//
//    // Calculate collision normal (reverse it for one of the objects)
//    sf::Vector2f collisionNormal = ic::normalVector(sf::Vector2f(
//        otherBounds.left + otherBounds.width / 2.0f - (thisBounds.left + thisBounds.width / 2.0f),
//        otherBounds.top + otherBounds.height / 2.0f - (thisBounds.top + thisBounds.height / 2.0f)
//    ));
//
//    // Calculate dot product
//    float dotProduct = relativeVelocity.x * collisionNormal.x + relativeVelocity.y * collisionNormal.y;
//
//    // Check if objects are moving towards each other
//    if (dotProduct < 0) {
//        // Calculate new velocities with adjusted impulse
//        float combinedMass = A.weight + B.weight;
//        float impulseValue = (-(1.0f + A.bounciness) * (1.0f + B.bounciness) * dotProduct) / combinedMass;
//        sf::Vector2f impulse = impulseValue * collisionNormal;
//
//        // Apply the impulse
//        A.velocity += impulse;
//        B.velocity -= impulse;
//
//        // Apply damping to reduce excessive bouncing
//        float damping = 0.5f; // Adjust as needed
//        A.velocity *= damping;
//        B.velocity *= damping;
//    }
//}

void GameClass::handleOutOfScreen(clutter& obj) {
    const float screenWidth = static_cast<float>(SCREEN_WIDTH);
    const float screenHeight = static_cast<float>(SCREEN_HEIGHT);

    if (obj.sprite.getPosition().x + obj.sprite.getGlobalBounds().width >= screenWidth || obj.sprite.getPosition().x <= 0.0f) {
        obj.velocity.x = 0.0f;
        if (obj.sprite.getPosition().x + obj.sprite.getGlobalBounds().width >= screenWidth) {
            obj.velocity.x = -1.0f;
        }
        if (obj.sprite.getPosition().x <= 0.0f) {
            obj.velocity.x = 1.0f;
        }
    }
    if (obj.sprite.getPosition().y + obj.sprite.getGlobalBounds().height >= screenHeight || obj.sprite.getPosition().y <= 0.0f) {
        obj.friction = 0.1f;
        obj.velocity.y = 0.0f;
        if (obj.sprite.getPosition().y + obj.sprite.getGlobalBounds().height >= screenHeight) {
            obj.velocity.y = -1.0f;
        }
        if (obj.sprite.getPosition().y <= 0.0f) {
            obj.velocity.y = 1.0f;
        }
    }
}
void GameClass::applyGravity(clutter& A, float _dt)
{
    sf::Vector2f grav = { 0.0f, GRAVITY * _dt };
    A.accel += grav;
}

// Other member functions and constructor/destructor...
/*
bool GameClass::gameLoop()
{
    sf::Event event;
    while (applicationWindow.app.pollEvent(event))
    {
        handleEvents();
    }

    // Update routine...
    deltaTime = dtClock.restart().asSeconds();
    // Collision detection and handling...

    // Rendering routine...
    applicationWindow.app.clear();
    applicationWindow.app.draw(structureSprite);
    for (auto& k : clutterPile) {
        applicationWindow.app.draw(k.sprite);
    }
    applicationWindow.app.draw(player.sprite);
    applicationWindow.app.display();

    return true;
}
*/
bool GameClass::handleEvents() {
    sf::Event event;
    while (applicationWindow.app.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            applicationWindow.app.close();
            return false;
        case sf::Event::MouseButtonPressed:
            handleMouseButtonPressed(event.mouseButton);
            break;
        case sf::Event::MouseButtonReleased:
            handleMouseButtonReleased(event.mouseButton);
            break;
        case sf::Event::KeyPressed:
            handleKeyPressed(event.key);
            break;
        case sf::Event::Resized:
            handleWindowResized(event.size.width, event.size.height);
            break;
        default:
            break;
        }
    }
    return true;
}

void GameClass::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton) {
    isClicking = true;
    sf::Vector2f mousePos = applicationWindow.transformMouse(applicationWindow.app);

    if (mouseButton.button == sf::Mouse::Left) {
        for (auto& k : clutterPile) {
            k.sprite.setPosition(sf::Vector2f((float)(ic::random(randomCount++, 12345) % 216) + 20.0f, (float)(ic::random(randomCount++, 12345) % 152) + 20.0f));
        }
    }
    else if (mouseButton.button == sf::Mouse::Right) {
        handleRightMouseButtonPressed(mousePos);
    }
    // Handle other mouse button events if needed
}

void GameClass::handleRightMouseButtonPressed(const sf::Vector2f& mousePos) {
    clutter cl;
    cl.velocity = { 4.0f, -16.0f };
    cl.image.loadFromFile("assets/samplebox.png");
    cl.texture.loadFromImage(cl.image);
    cl.sprite.setTexture(objectTex, true);
    cl.centerPoint = sf::Vector2f(16.0f, 16.0f);
    cl.sprite.setOrigin(0.0f,0.0f);
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

void GameClass::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton) {
    isClicking = false;
    // Handle mouse button released events if needed
}

void GameClass::handleKeyPressed(const sf::Event::KeyEvent& keyEvent) {
    if (keyEvent.code == sf::Keyboard::F11) {
        if (applicationWindow.isFullScreen) {
            applicationWindow.launchWindow("My Limit (feyleaf.com)");
        }
        else {
            applicationWindow.launchFullScreen("My Limit Fullscreen (feyleaf.com)");
        }
    }
    // Handle other key pressed events if needed
}

void GameClass::handleWindowResized(int width, int height) {
    applicationWindow.renewView(width, height);
    screenRect = applicationWindow.canvas.canvasToRect();
    // Handle window resized events if needed
}

/*sf::Image GameClass::makeSilouette(sf::Image& original)
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
*/

GameClass::GameClass(const ic::gameScreen _screen)
{
    playerImg.loadFromFile("assets/sampleplayer.png");
    playerTex.loadFromImage(playerImg);
    playerSprite.setTexture(playerTex, true);
    playerSprite.setOrigin(0.0f, 0.0f);
    playerSprite.setPosition(32.0f, 32.0f);
    playerVelocity = { 30.0f, 30.0f };
    objectImg.loadFromFile("assets/samplebox.png");
    objectTex.loadFromImage(objectImg);
    bearTexture.loadFromFile("assets/teddy.png");
    burritoTexture.loadFromFile("assets/burrito.png");
    couchTex.loadFromFile("assets/couch.png");
    tableTex.loadFromFile("assets/table.png");
    objectSprite.setTexture(objectTex, true);
    objectSprite.setOrigin(0.0f, 0.0f);
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
        cl.sprite.setOrigin(0.0f, 0.0f);
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
    player.sprite.setOrigin(0.0f, 0.0f);
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

void GameClass::handlePlayerCollision() {
    sf::FloatRect playerBounds = player.sprite.getGlobalBounds();

    for (auto& k : clutterPile) {
        sf::FloatRect clutterBounds = k.sprite.getGlobalBounds();

        if (playerBounds.intersects(clutterBounds)) {
            // Handle player and clutter collision here
            // You can apply custom logic or modify player/clutter properties as needed
            // For example, you can check if the player should bounce off or be affected in some way
            // Update player's velocity or position accordingly
        }
    }
}

GameClass::~GameClass()
{

}

void GameClass::renderGame()
{
    applicationWindow.app.clear();
    applicationWindow.app.draw(structureSprite);
    for (auto& k : clutterPile)
    {
        applicationWindow.app.draw(k.sprite);
        applicationWindow.app.draw(k.box);
    }
    applicationWindow.app.draw(player.sprite);
    applicationWindow.app.display();
}

bool GameClass::gameLoop() {
    if (!handleEvents()) {
        return false; // Exit the game if the window is closed
    }

    deltaTime = dtClock.restart().asSeconds();

    for (size_t iterk = 0; iterk < clutterPile.size(); iterk++) {
        for (size_t iterl = iterk + 1; iterl < clutterPile.size(); iterl++) {
            handleCollision(clutterPile[iterk], clutterPile[iterl]);
        }
    }

    handlePlayerCollision(); // Handle collisions with the player

    for (auto& k : clutterPile) {
        k.velocity.x *= k.friction;
        applyGravity(k, deltaTime);
        k.velocity += k.accel * deltaTime;
        k.sprite.move(k.velocity * deltaTime);
    }

    applyGravity(player, deltaTime);
    player.velocity.x *= player.friction;
    player.sprite.move(player.velocity * deltaTime);

    handleOutOfScreen(player);

    // Render the game
    renderGame();

    return true;
}
/*
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
*/