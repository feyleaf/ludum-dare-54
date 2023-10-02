#ifndef GAME_HPP
#define GAME_HPP

#include "global.h"

class RigidObject
{
public:
	sf::Sprite sprite;
	sf::FloatRect dimensions;
	sf::Vector2f origin;
	sf::Vector2f position;
	float rotation;
	float bounciness;
	float mass;
	float friction;

	RigidObject(cpSpace* _space, sf::FloatRect _dimension, sf::Texture& tex, float _mass, float _bounce, float _friction)
	{
		body = cpBodyAlloc();
		sprite.setTexture(tex);
		sprite.setPosition(sf::Vector2f(50.0f, 100.0f));
		sprite.setRotation(0.0f);

		mass = _mass;
		dimensions = _dimension;
		friction = _friction;
		bounciness = _bounce;
		origin = { dimensions.getSize().x / 2.0f, dimensions.getSize().y / 2.0f };
		sprite.setOrigin(origin);
		cpFloat moment = cpMomentForBox(mass, dimensions.width, dimensions.height);
		cpBody* body = cpSpaceAddBody(_space, cpBodyNew(mass, moment));
		cpBodySetPosition(body, cpv(sprite.getPosition().x, sprite.getPosition().y));
		cpShape* boxShape = cpBoxShapeNew(body, dimensions.width, dimensions.height, 0.0);
		cpShapeSetFriction(boxShape, friction);
		cpShapeSetElasticity(boxShape, bounciness);
		cpSpaceAddShape(_space, boxShape);
	}

	cpBody* getBody() const { return body; }
private:
	cpBody* body;
};

class SpaceClass
{
public:
	cpVect gravityVector;


	SpaceClass() {
		space = cpSpaceNew();
		gravityVector = cpv(0, 98.1f);
		cpSpaceSetGravity(space, gravityVector);
		cpBody* staticBody = cpSpaceGetStaticBody(space);
		// Create a ground segment
		cpVect groundA = cpv(-320, -240);
		cpVect groundB = cpv(320, -240);
		cpShape* ground = cpSegmentShapeNew(staticBody, groundA, groundB, 0.0);
		cpShapeSetFriction(ground, 1.0);
		cpShapeSetElasticity(ground, 0.5);
		cpSpaceAddShape(space, ground);
	}

	void addRigidObject(RigidObject& obj)
	{
		objects.push_back(obj);
	}

	void updateWorld(float _deltaTime)
	{
		for (auto& k : objects)
		{
			cpBodySetForce(k.getBody(), gravityVector);
			cpVect vec = cpBodyGetPosition(k.getBody());
			k.sprite.setPosition(vec.x, vec.y);
			float angle = cpBodyGetAngle(k.getBody());
			k.sprite.setRotation(angle * 180 / PI);
		}
		cpSpaceStep(space, _deltaTime);
	}

	void renderWorld(sf::RenderWindow& _app)
	{
		for (auto& k : objects)
		{
			_app.draw(k.sprite);
		}
	}

	cpSpace* getSpace() { return space; }

	std::vector<RigidObject> objects;


private:
	cpSpace* space;
};
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
	bool isJumping = false;
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

	SpaceClass spaceWorld;

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

	GameClass(const ic::gameScreen _screen = ic::vm_sms);
	~GameClass();

	bool gameLoop();
};

#endif //GAME_HPP