#ifndef GAME_HPP
#define GAME_HPP

#include "global.h"

struct entity
{
	bool isActive;
	int textureIndex;
	sf::Sprite sprite;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;
	float spinSpeed;
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
	uint32_t randomCount = 0;
	bool isClicking = false;

	sf::Sprite playerSprite;
	sf::Texture playerTex;
	sf::Image playerImg;
	sf::Vector2f playerVelocity = { 2.0f, 2.0f };

	sf::Sprite structureSprite;
	sf::Texture structureTex;
	sf::Image structureImg;

	sf::FloatRect screenRect;


	GameClass(const ic::gameScreen _screen = ic::vm_sms);
	~GameClass();

	bool gameLoop();
};

#endif //GAME_HPP