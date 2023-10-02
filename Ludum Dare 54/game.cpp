#include "global.h"

// Constants
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 192;

constexpr float PLAYER_SPEED = 0.5f;
constexpr float JUMP_FORCE = 10.0f;

void GameClass::initializeClutterObjects() {
    const int numObjects = 20;
    myPlayer.initialize(waitTime, spaceWorld.getSpace(), sf::FloatRect(0.0f, 0.0f, 32.0f, 32.0f), playerTex, sf::Vector2f(12.0f,  36.0f), 10.0f, 0.15f, 0.2f);
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

        randomCount++;
        sf::Vector2f randomVector = { float(ic::random(randomCount++, 12345) % 5) * 21.25f, float(ic::random(randomCount++, 54321) % 5)*21.25f };
        sf::Vector2f posVector = sf::Vector2f(50.0f, 100.0f) + randomVector;

        int selection = ic::random(randomCount++, time(nullptr)) % 15;
        sf::FloatRect boreRect;
        sf::Texture* boreTex;
        bool furn = false;
        switch (selection)
        {
        case 0:
        case 1:
            boreRect = sf::FloatRect(0, 0, 22, 31);
            boreTex = &bearTexture;
            break;
        case 2:
            boreRect = sf::FloatRect(0, 0, 64, 37);
            boreTex = &couchTex;
            furn = true;
            break;
        case 3:
        case 4:
        case 5:
            boreRect = sf::FloatRect(0, 0, 25, 17);
            boreTex = &burritoTexture;
            break;
        case 6:
            boreRect = sf::FloatRect(0, 0, 39, 26);
            boreTex = &tableTex;
            furn = true;
            break;
        case 7:
        default:
            boreRect = sf::FloatRect(0, 0, 30, 29);
            boreTex = &objectTex;
            break;
        }

        RigidObject ob(spaceWorld.getSpace(), boreRect, *boreTex, posVector, 10.0f, 0.05f, 1.0f);
        if (furn) { ob.isFurniture = true; }
        furn = false;
        spaceWorld.addRigidObject(ob);
    }
}

void GameClass::handlePlayerInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        myPlayer.sprite.setScale(-1.0f, 1.0f);
        player.velocity.x = -PLAYER_SPEED; // Move left
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        myPlayer.sprite.setScale(1.0f, 1.0f);
        player.velocity.x = PLAYER_SPEED; // Move right
    }
    else {
        player.velocity.x = 0.0f; // Stop horizontal movement if no key is pressed
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !player.isJumping) {
        player.velocity.y = -8.0f * JUMP_FORCE;
        player.isJumping = true;// Apply an upward force for jumping
    }
    else
    {
        player.velocity.y = 0.0f;
        //player.isJumping = false;
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        player.isJumping = false;
    }
    myPlayer.move(player.velocity, player.isJumping);
}

bool GameClass::playerIsOnGround() {
    const float screenHeight = static_cast<float>(SCREEN_HEIGHT);
    sf::FloatRect playerBounds = player.sprite.getGlobalBounds();
    if (playerBounds.top + playerBounds.height >= screenHeight-62.0f) {
        player.isJumping = false;
        return true;
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
    if (gameState == FRESH)
    {
        gameState = START;
    }
    if (keyEvent.code == sf::Keyboard::F11) {
        if (applicationWindow.isFullScreen) {
            applicationWindow.launchWindow("Clutterphobia");
        }
        else {
            applicationWindow.launchFullScreen("Clutterphobia");
        }
    }
    if (keyEvent.code == sf::Keyboard::Escape) {
        applicationWindow.app.close();
    }
}

void GameClass::handleWindowResized(int width, int height) {
    applicationWindow.renewView(width, height);
    screenRect = applicationWindow.canvas.canvasToRect();
}


GameClass::GameClass(const ic::gameScreen _screen)
{
    waitTime = 0.0f;
    splashTex.loadFromFile("assets/game-art.png");
    splashScreen.setTexture(splashTex);
    splashScreen.setPosition(sf::Vector2f(0.0f, 0.0f));
    playerImg.loadFromFile("assets/player32x.png");
    playerTex.loadFromImage(playerImg);
    playerTex2.loadFromFile("assets/player32b.png");
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

    winner.loadFromFile("assets/winner.png");
    winnerSprite.setTexture(winner);
    winnerSprite.setScale(3.0f, 3.0f);
    winnerSprite.setPosition(32.0f, 32.0f);


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
    int winState = 0;
    for (auto& k : spaceWorld.objects)
    {
        if (k.isActive && !k.isFurniture)
        {
            winState++;
        }
    }

    for (auto& obj : spaceWorld.objects)
    {
        if (obj.isActive)
        {
            applicationWindow.app.draw(obj.sprite);
        }
    }

    applicationWindow.app.draw(myPlayer.sprite);
    if (winState < 1)
    {
        applicationWindow.app.draw(winnerSprite);
    }

    applicationWindow.app.display();
}

bool GameClass::gameLoop()
{
    waitTime += deltaTime;
    myPlayer.sprite.setTexture(playerTex, true);
    if (!handleEvents()) {
        return false; // Exit the game if the window is closed
    }
    if (gameState == START)
    {
        if (sf::Keyboard::isKeyPressed(suctionKey)) {
            myPlayer.sprite.setTexture(playerTex2, true);
            sf::Vector2f playerPosition = myPlayer.sprite.getPosition();/* Get the player's position */;
            float suctionForce = 90.0f;/* Define the suction force */;

            for (auto& obj : spaceWorld.objects) {
                if (!obj.isFurniture && obj.isActive)
                {
                    obj.ApplyVacuumEffect(playerPosition, suctionForce);
                    float dist = ic::calcDist(obj.sprite.getPosition(), playerPosition) - ((myPlayer.sprite.getGlobalBounds().width + obj.sprite.getGlobalBounds().width) / 2.0f);
                    if (dist <= 7.0f)
                    {
                        spaceWorld.recreateBody(obj.getBody(), obj, 0.75f);
                    }
                    if (dist <= 6.0f)
                    {
                        spaceWorld.recreateBody(obj.getBody(), obj, 0.50f);
                    }
                    if (dist <= 4.0f)
                    {
                        spaceWorld.recreateBody(obj.getBody(), obj, 0.25f);
                    }
                    if (dist <= 2.0f)
                    {
                        spaceWorld.recreateBody(obj.getBody(), obj, 0.15f);
                    }
                    if (dist <= 1.0f)
                    {
                        spaceWorld.destroyBody(obj.getBody(), obj);
                    }
                }
            }
        }


        deltaTime = dtClock.restart().asSeconds();

        updatePlayer();
        spaceWorld.updateWorld(myPlayer, deltaTime);

        // Update player and handle player-object collisions


        // Render the game
        renderGame();
    }
    else
    {
        deltaTime = dtClock.restart().asSeconds();
        waitTime = deltaTime;
        applicationWindow.app.clear();
        applicationWindow.app.draw(splashScreen);
        applicationWindow.app.display();
    }
//    spaceWorld.renderWorld(myPlayer, applicationWindow.app);
    return true;
}