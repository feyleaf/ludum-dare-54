#include "global.h"

// Constants
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 192;

constexpr float PLAYER_SPEED = 25.0f;
constexpr float JUMP_FORCE = 20.0f;

void GameClass::initializeClutterObjects() {
    const int numObjects = 10;

    for (int i = 0; i < numObjects; i++) {
        /*
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
        */

        RigidObject ob(spaceWorld.getSpace(), sf::FloatRect(0, 0, 32, 32), bearTexture, 10.0f, 1.0f, 0.5f);
        spaceWorld.addRigidObject(ob);
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
    // Render the RigidObject instances
    for (auto& obj : spaceWorld.objects)
    {
        applicationWindow.app.draw(obj.sprite);
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

    spaceWorld.updateWorld(deltaTime);

    // Update player and handle player-object collisions
    updatePlayer();

    
    // Render the game
    renderGame();
    spaceWorld.renderWorld(applicationWindow.app);
    return true;
}