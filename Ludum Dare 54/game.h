#ifndef GAME_HPP
#define GAME_HPP

#include "global.h"

struct clutter
{
	bool isActive;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	float weight;
	sf::Vector2f velocity;
	sf::Vector2f accel;
	float bounciness = 0.8f;
	float friction;
	sf::FloatRect boundingBox;
};


class GameClass
{
public:
	//initialization should conform to one of the pre-built video modes (gameScreen struct) in "Window.h"
	// we initialize the game with a selection from gameScreen list and with a titlebar title
	// fallback possibly to a configuration file --this does not exist yet
	// 
	// pseudo-code
	// ic::Game game(video::gameboy, "this game's title")
	// held the two parameters in the Window class
	// Window class:
	//  selected video screen (video = (gameboy(160, 144, 8, "Gameboy")))
	//  entered title (title = "this game's title")
	//  
	//  also keep information on desktop mode (sf::VideoMode::getDesktopMode()) for sizing
	//  also select if the screen dimensions are fullscreenable
	//   for(auto& k : fullscreens)
	//   {
	//    
	//   }
	//instance the world
	//instance the game state
	//instance the entities
	//handle player input
	//handle rendering the scene on screen
	//calculate the mouse positions

	WindowClass applicationWindow;
	sf::Clock dtClock;
	float deltaTime = dtClock.restart().asSeconds();
	float lastTime = deltaTime;
	uint32_t randomSeed = static_cast<unsigned>(std::time(nullptr));
	uint32_t randomCount = 0;
	bool boxCollide = false;
	bool pixelCollide = false;
	bool isClicking = false;
	void initializeClutterObjects();
	void handleCollision(clutter& A, clutter& B);
	void applyGravity(clutter& A, float _dt);
	void handleClutterCollision(clutter& A, clutter& B);

	sf::Sprite playerSprite;
	sf::Texture playerTex;
	sf::Image playerImg;
	sf::Vector2f playerVelocity = { 2.0f, 2.0f };
	clutter player;

	std::vector<clutter> clutterPile;

	sf::Sprite objectSprite;
	sf::Texture objectTex;
	sf::Texture bearTexture;
	sf::Texture burritoTexture;
	sf::Texture couchTex;
	sf::Texture tableTex;
	sf::Image objectImg;
	sf::Vector2f objectVelocity = { -4.0f, 2.0f };

	sf::Sprite structureSprite;
	sf::Texture structureTex;
	sf::Image structureImg;

	sf::FloatRect screenRect;

	void handleOutOfScreen(clutter& obj);
	void handleCollisionResponse(clutter& A, clutter& B);
	void handleWindowResized(int width, int height);
	void handleKeyPressed(const sf::Event::KeyEvent& keyEvent);
	void handleMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton);
	void handleRightMouseButtonPressed(const sf::Vector2f& mousePos);
	void handleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton);
	bool handleEvents();
	void renderGame();
	void handlePlayerInput();
	bool playerIsOnGround();
	void updatePlayer();
	void updatePlayerPosition();
	void handlePlayerClutterCollision(clutter& player, clutter& object);

	GameClass(const ic::gameScreen _screen = ic::vm_sms);
	~GameClass();

	bool gameLoop();
};

#endif //GAME_HPP