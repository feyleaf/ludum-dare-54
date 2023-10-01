#include "global.h"

// Constants
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 192;

constexpr float PLAYER_SPEED = 25.0f;
constexpr float JUMP_FORCE = 20.0f;

void GameClass::initializeClutterObjects() {
    const int numObjects = 10;

    for (int i = 0; i < numObjects; i++) {
        clutter cl;
        cl.velocity = { 0.0f, 0.0f };

        float randomX = static_cast<float>(ic::random(randomCount++, randomSeed) % 180);
        float randomY = static_cast<float>(ic::random(randomCount++, randomSeed) % 107);

        cl.sprite.setPosition(randomX, randomY);

        cl.velocity = { 0.0f, -1.0f };
        switch (ic::random(randomCount++, randomSeed) % 5)
        {
        case 1:
            cl.image.loadFromFile("assets/samplebox.png");
            cl.texture.loadFromImage(cl.image);
            cl.sprite.setTexture(objectTex, true);
            cl.weight = 1.0f;

            break;
        case 2:
            cl.image.loadFromFile("assets/teddy.png");
            cl.texture.loadFromImage(cl.image);
            cl.sprite.setTexture(bearTexture, true);
            cl.weight = 1.0f;
            break;
        case 3:
            cl.image.loadFromFile("assets/couch.png");
            cl.texture.loadFromImage(cl.image);
            cl.sprite.setTexture(couchTex, true);
            cl.weight = 4.0f;
            break;
        case 4:
            cl.image.loadFromFile("assets/table.png");
            cl.texture.loadFromImage(cl.image);
            cl.sprite.setTexture(tableTex, true);
            cl.weight = 3.0f;
            break;
        default:
            cl.image.loadFromFile("assets/burrito.png");
            cl.texture.loadFromFile("assets/burrito.png");
            cl.sprite.setTexture(burritoTexture, true);
            cl.weight = 1.0f;
            break;

        }
        cl.boundingBox = cl.sprite.getLocalBounds();
        cl.sprite.setOrigin(0.0f, 0.0f);
        cl.accel = { 0.0f, -0.0f };
        cl.isActive = true;
        cl.bounciness = 0.8f;
        cl.friction = 0.99f;

        clutterPile.push_back(cl);
    }
}

void GameClass::updatePlayerPosition() {
    
    for (auto& k : clutterPile)
    {
        handlePlayerClutterCollision(player, k);
    }
    
    player.sprite.move(player.velocity * deltaTime);
    player.boundingBox = player.sprite.getGlobalBounds();
    for (auto& k : clutterPile) {
        if (player.boundingBox.intersects(k.boundingBox)) {
            handlePlayerCollisionResponse(player, k);
        }
    }

    handleOutOfScreen(player);
}

void GameClass::handleClutterCollision(clutter& A, clutter& B) {
    if (A.boundingBox.intersects(B.boundingBox)) {
        float overlapX = std::min(A.boundingBox.left + A.boundingBox.width, B.boundingBox.left + B.boundingBox.width) -
            std::max(A.boundingBox.left, B.boundingBox.left);
        float overlapY = std::min(A.boundingBox.top + A.boundingBox.height, B.boundingBox.top + B.boundingBox.height) -
            std::max(A.boundingBox.top, B.boundingBox.top);

        if (overlapX < overlapY) {
            float pushX = overlapX / 2.0f;
            A.sprite.move(-pushX, 0.0f);
            B.sprite.move(pushX, 0.0f);
            float totalWeight = A.weight + B.weight;
            float newVelocityA = (A.weight * A.velocity.x + B.weight * B.velocity.x) / totalWeight;
            float newVelocityB = (A.weight * A.velocity.x + B.weight * B.velocity.x) / totalWeight;
            A.velocity.x = newVelocityA;
            B.velocity.x = newVelocityB;
        }
        else {
            float pushY = overlapY / 2.0f;
            A.sprite.move(0.0f, -pushY);
            B.sprite.move(0.0f, pushY);
            float totalWeight = A.weight + B.weight;
            float newVelocityA = (A.weight * A.velocity.y + B.weight * B.velocity.y) / totalWeight;
            float newVelocityB = (A.weight * A.velocity.y + B.weight * B.velocity.y) / totalWeight;
            A.velocity.y = newVelocityA;
            B.velocity.y = newVelocityB;
        }

        A.boundingBox = A.sprite.getGlobalBounds();
        B.boundingBox = B.sprite.getGlobalBounds();
    }
}

void GameClass::handlePlayerInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.velocity.x = -PLAYER_SPEED; // Move left
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.velocity.x = PLAYER_SPEED; // Move right
    }
    else {
        player.velocity.x = 0.0f; // Stop horizontal movement if no key is pressed
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && playerIsOnGround()) {
        player.isJumping = true;
        player.velocity.y = -6.0f*JUMP_FORCE; // Apply an upward force for jumping
    }
}

bool GameClass::playerIsOnGround() {
    const float screenHeight = static_cast<float>(SCREEN_HEIGHT);
    sf::FloatRect playerBounds = player.sprite.getGlobalBounds();
    if (playerBounds.top + playerBounds.height >= screenHeight-62.0f) {
        player.isJumping = false;
        return true;
    }

    for (const auto& clutterObj : clutterPile) {
        if (playerBounds.intersects(clutterObj.boundingBox)) {
            // You can adjust this threshold value as needed to determine when the player is on top of a clutter object
            float verticalThreshold = 10.0f;
            if (playerBounds.top + playerBounds.height >= clutterObj.boundingBox.top - verticalThreshold) {
                return true;
            }
        }
    }

    return false;
}
void GameClass::updatePlayer() {
    handlePlayerInput();
    applyGravity(player, deltaTime);
    for (auto& k : clutterPile)
    {
        handlePlayerClutterCollision(player, k);
    }
    updatePlayerPosition();
}

void GameClass::handleCollisionResponse(clutter& A, clutter& B) {
    sf::Vector2f relativeVelocity = A.velocity - B.velocity;
    sf::Vector2f collisionNormal = ic::normalVector(B.sprite.getPosition() - A.sprite.getPosition());
    float dotProduct = relativeVelocity.x * collisionNormal.x + relativeVelocity.y * collisionNormal.y;
    float radiusA = 0.5f * (A.boundingBox.width);
    float radiusB = 0.5f * (B.boundingBox.width);
    float overlap = radiusA + radiusB - ic::calcMagnitude(A.sprite.getPosition() - B.sprite.getPosition());
    sf::Vector2f separation = collisionNormal * (overlap / 2.0f);
    A.sprite.move(separation);
    B.sprite.move(-separation);
    float combinedMass = A.weight + B.weight;
    float impulseMagnitude = (-(1.0f + A.bounciness) * dotProduct) / std::max(combinedMass, 1.0f);
    sf::Vector2f impulse = impulseMagnitude * collisionNormal;
    A.velocity += impulse / std::max(A.weight, 1.0f);
    B.velocity -= impulse / std::max(B.weight, 1.0f);
}

void GameClass::handlePlayerCollisionResponse(clutter& player, clutter& obj) {
    sf::Vector2f relativeVelocity = player.velocity - obj.velocity;
    sf::Vector2f collisionNormal = ic::normalVector(obj.sprite.getPosition() - player.sprite.getPosition());
    float dotProduct = relativeVelocity.x * collisionNormal.x + relativeVelocity.y * collisionNormal.y;
    float radiusA = 0.5f * (player.boundingBox.width);
    float radiusB = 0.5f * (obj.boundingBox.width);
    float overlap = radiusA + radiusB - ic::calcMagnitude(player.sprite.getPosition() - obj.sprite.getPosition());
    sf::Vector2f separation = collisionNormal;// *(overlap / 2.0f);
    //player.sprite.move(separation);
    player.velocity = separation;
    //float combinedMass = player.weight + obj.weight;
    //float impulseMagnitude = (-(1.0f + player.bounciness) * dotProduct) / std::max(combinedMass, 1.0f);
    //sf::Vector2f impulse = impulseMagnitude * collisionNormal;
    //player.velocity += impulse / std::max(player.weight, 1.0f);
}

void GameClass::handleOutOfScreen(clutter& obj) {
    const float screenWidth = static_cast<float>(SCREEN_WIDTH);
    const float screenHeight = static_cast<float>(SCREEN_HEIGHT);

    sf::Vector2f objPosition = obj.sprite.getPosition();
    sf::FloatRect objBounds = obj.sprite.getGlobalBounds();

    // Check left and right screen boundaries
    if (objPosition.x < 0.0f) {
        objPosition.x = 0.0f;
        obj.velocity.x = -obj.velocity.x * obj.friction;
    }
    else if (objPosition.x + objBounds.width > screenWidth) {
        objPosition.x = screenWidth - objBounds.width;
        obj.velocity.x = -obj.velocity.x * obj.friction;
    }

    // Check top screen boundary (disable upward movement)
    if (objPosition.y < 0.0f) {
        objPosition.y = 0.0f;
        obj.velocity.y = 0.0f;
    }

    // Check bottom screen boundary (objects sit stationary unless pushed)
    if (objPosition.y + objBounds.height > screenHeight && !obj.isJumping) {
        objPosition.y = screenHeight - objBounds.height;
        obj.velocity.y = 0.0f; // Object sits stationary when it reaches the bottom
    }

    obj.sprite.setPosition(objPosition);
}

void GameClass::applyGravity(clutter& A, float _dt)
{
    const sf::FloatRect groundRect(0.0f, 190.0f, 256.0f, 10.0f);

    // Check if A is above the ground
    if (!A.sprite.getGlobalBounds().intersects(groundRect))
    {
        // Apply gravity
        sf::Vector2f gravityVector(0.0f, GRAVITY);
        A.velocity += gravityVector * _dt;
    }
    else
    {
        // If A is on or below the ground, reset acceleration and velocity in the y-axis
        //A.accel.y = 0.0f;
        A.velocity.y = 0.0f;
    }
}

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
            k.sprite.setPosition(sf::Vector2f((float)(ic::random(randomCount++, randomSeed) % 156) + 64.0f, (float)(ic::random(randomCount++, randomSeed) % 102) + 64.0f));
            k.velocity = { 0.0f, 0.0f };
        }
    }
    else if (mouseButton.button == sf::Mouse::Right) {
        handleRightMouseButtonPressed(mousePos);
    }
}

void GameClass::handleRightMouseButtonPressed(const sf::Vector2f& mousePos) {
    clutter cl;
    cl.velocity = { 4.0f, -16.0f };
    cl.image.loadFromFile("assets/samplebox.png");
    cl.texture.loadFromImage(cl.image);
    cl.sprite.setTexture(objectTex, true);
    cl.sprite.setOrigin(0.0f,0.0f);
    cl.sprite.setPosition(mousePos);
    cl.boundingBox = cl.sprite.getGlobalBounds();
    cl.accel = { 0.0f, -0.0f };
    cl.isActive = true;
    cl.weight = 1.0f;
    cl.bounciness = 0.8f;
    cl.friction = 0.999f;
    clutterPile.push_back(cl);
}

void GameClass::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton) {
    isClicking = false;
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
}

void GameClass::handleWindowResized(int width, int height) {
    applicationWindow.renewView(width, height);
    screenRect = applicationWindow.canvas.canvasToRect();
    // Handle window resized events if needed
}


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

    initializeClutterObjects();

    player.image.loadFromFile("assets/sampleplayer.png");
    player.texture.loadFromImage(playerImg);
    player.sprite.setTexture(playerTex);
    player.velocity = { 0.0f, 0.0f };
    player.sprite.setOrigin(0.0f, 0.0f);
    player.sprite.setPosition(90.0f, 30.0f);
    player.accel = { 0.0f, 0.0f };
    player.isActive = true;
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

void GameClass::renderGame()
{
    applicationWindow.app.clear();
    applicationWindow.app.draw(structureSprite);
    for (auto& k : clutterPile)
    {
        applicationWindow.app.draw(k.sprite);
    }
    applicationWindow.app.draw(player.sprite);
    applicationWindow.app.display();
}

bool GameClass::gameLoop()
{
    if (!handleEvents()) {
        return false; // Exit the game if the window is closed
    }

    deltaTime = dtClock.restart().asSeconds();
    for (auto& k : clutterPile)
    {
        k.boundingBox.left = k.sprite.getPosition().x;
        k.boundingBox.top = k.sprite.getPosition().y;
    }
    for (size_t iterk = 0; iterk < clutterPile.size(); iterk++) {
        for (size_t iterl = iterk + 1; iterl < clutterPile.size(); iterl++) {
            handleClutterCollision(clutterPile[iterk], clutterPile[iterl]);
        }
    }

    // Update player and handle player-object collisions
    updatePlayer();

    // Update clutter objects
    for (auto& k : clutterPile) {
        applyGravity(k, deltaTime);
        k.velocity.x *= k.friction;
        k.velocity += k.accel * deltaTime;
        k.velocity.x = std::min(k.velocity.x, 30.0f);
        k.velocity.y = std::min(k.velocity.y, 30.0f);
        if (std::isfinite(k.velocity.x) && std::isfinite(k.velocity.y)) {
            k.sprite.move(k.velocity * deltaTime);
        }
        handleOutOfScreen(k);
    }

    // Render the game
    renderGame();

    return true;
}

void GameClass::handlePlayerClutterCollision(clutter& _player, clutter& object) {
    if (_player.boundingBox.intersects(object.boundingBox)) {
        // Calculate overlap and collision direction
        float overlapX = std::min(_player.boundingBox.left + _player.boundingBox.width, object.boundingBox.left + object.boundingBox.width) -
            std::max(_player.boundingBox.left, object.boundingBox.left);
        float overlapY = std::min(_player.boundingBox.top + _player.boundingBox.height, object.boundingBox.top + object.boundingBox.height) -
            std::max(_player.boundingBox.top, object.boundingBox.top);

        if (overlapX < overlapY) {
            // Resolve the collision in the x-direction
            float pushX = overlapX / 2.0f;

            // Move objects apart in the x-direction
            _player.sprite.move(-pushX, 0.0f);
            object.sprite.move(pushX, 0.0f);

            // Adjust velocities if needed

            // Update bounding boxes
            _player.boundingBox = _player.sprite.getGlobalBounds();
            object.boundingBox = object.sprite.getGlobalBounds();
        }
        else {
            // Resolve the collision in the y-direction
            float pushY = overlapY / 2.0f;

            // Move objects apart in the y-direction
            _player.sprite.move(0.0f, -pushY);
            object.sprite.move(0.0f, pushY);

            // Adjust velocities if needed
            // ...

            // Update bounding boxes
            _player.boundingBox = player.sprite.getGlobalBounds();
            object.boundingBox = object.sprite.getGlobalBounds();
        }
    }
}