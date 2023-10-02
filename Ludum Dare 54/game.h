#ifndef GAME_HPP
#define GAME_HPP

#include "global.h"

class RigidObject
{
public:
	bool isFurniture = false;
	sf::Sprite sprite;
	sf::FloatRect dimensions;
	sf::Vector2f origin;
	sf::Vector2f position;
	float rotation = 0.0f;
	float bounciness = 0.0f;
	float mass = 1.0f;
	float friction = 1.0f;

	RigidObject(cpSpace* _space, sf::FloatRect _dimension, sf::Texture& tex, sf::Vector2f _pos, float _mass, float _bounce, float _friction)
	{
		isFurniture = false;
		rotation = 0.0f;
		//randomCount = ic::random(90, time(nullptr));
		sprite.setTexture(tex);
		sprite.setPosition(_pos);
		sprite.setRotation(rotation);

		mass = _mass;
		dimensions = _dimension;
		friction = _friction;
		bounciness = _bounce;
		origin = { dimensions.getSize().x / 2.0f, dimensions.getSize().y / 2.0f };
		sprite.setOrigin(origin);
		cpFloat moment = cpMomentForBox(mass, dimensions.width, dimensions.height);
		body = cpSpaceAddBody(_space, cpBodyNew(mass, moment));
		cpBodySetPosition(body, cpv(_pos.x, _pos.y));
		boxShape = cpBoxShapeNew(body, dimensions.width, dimensions.height, 0.0);
		cpShapeSetFriction(boxShape, friction);
		cpShapeSetElasticity(boxShape, bounciness);
		cpSpaceAddShape(_space, boxShape);
	}
	// Function to apply vacuum cleaner effect
	void ApplyVacuumEffect(const sf::Vector2f& playerPosition, float suctionForce)
	{
		if (!isFurniture)
		{
			// Calculate the direction from the object to the player
			sf::Vector2f direction = playerPosition - sprite.getPosition();

			// Calculate the unit vector (normalized direction)
			sf::Vector2f unitVector = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);

			// Apply a force in the direction of the unit vector
			cpVect force = cpv(unitVector.x * suctionForce, unitVector.y * suctionForce);
			cpBodyApplyForceAtWorldPoint(body, force, cpv(origin.x, origin.y));

		}
	}


	cpBody* getBody() const { return body; }
	cpShape* getShape() const { return boxShape; }
	void setShape(cpShape* newShape) { boxShape = newShape; }
private:
	cpBody* body;
	cpShape* boxShape;
};

class PlayerClass
{
public:
	sf::Sprite sprite;
	sf::FloatRect dimensions;
	sf::Vector2f origin;
	sf::Vector2f position;
	float rotation = 0.0f;
	float bounciness = 0.0f;
	float mass = 1.0f;
	float friction = 1.0f;

	PlayerClass() {};
	void initialize(cpSpace* _space, sf::FloatRect _dimension, sf::Texture& tex, sf::Vector2f _pos, float _mass, float _bounce, float _friction)
	{
		//randomCount = ic::random(90, time(nullptr));
		sprite.setTexture(tex);
		sprite.setPosition(_pos);
		sprite.setRotation(0.0f);

		mass = _mass;
		dimensions = _dimension;
		friction = _friction;
		bounciness = _bounce;
		origin = { dimensions.getSize().x / 2.0f, dimensions.getSize().y / 2.0f };
		sprite.setOrigin(origin);
		cpFloat moment = cpMomentForBox(mass, dimensions.width, dimensions.height);
		body = cpSpaceAddBody(_space, cpBodyNew(mass, moment));
		cpBodySetPosition(body, cpv(_pos.x, _pos.y));
		boxShape = cpBoxShapeNew(body, dimensions.width, dimensions.height, 0.0);
		cpShapeSetFriction(boxShape, friction);
		cpShapeSetElasticity(boxShape, bounciness);
		cpSpaceAddShape(_space, boxShape);
	}
	void move(sf::Vector2f moveVector, bool isImpulse)
	{
		if (isImpulse)
		{
			cpBodyApplyImpulseAtWorldPoint(body, cpv(moveVector.x, moveVector.y*30.0f), cpv(origin.x - moveVector.x, origin.y));
		}
		else
		{
			cpBodyApplyImpulseAtWorldPoint(body, cpv(moveVector.x, moveVector.y), cpv(origin.x - moveVector.x, origin.y - moveVector.y));
		}
		float maxRotation = 30.0f;  // Adjust as needed
		float currentRotation = cpBodyGetAngle(body);
		if (currentRotation > maxRotation) {
			cpBodySetAngle(body, maxRotation);
		}
		else if (currentRotation < -maxRotation) {
			cpBodySetAngle(body, -maxRotation);
		}
		if (cpBodyGetAngularVelocity(body)>0.5f)
		{
			cpBodySetAngle(body, 0.0f);
		}

		// Apply damping to rotational velocity
		cpFloat damping = 0.9f;  // Adjust as needed
		cpBodySetAngularVelocity(body, cpBodyGetAngularVelocity(body) * damping);
	}
	cpBody* getBody() { return body; }
private:
	cpBody* body = nullptr;
	cpShape* boxShape = nullptr;
};

class SpaceClass
{
public:
	cpVect gravityVector;
	//PlayerClass thePlayer;

	SpaceClass() {
		space = cpSpaceNew();
		gravityVector = cpv(0, 98.1f);
		cpSpaceSetGravity(space, gravityVector);
		cpBody* staticBody = cpSpaceGetStaticBody(space);
		// Create a ground segment
		cpVect groundA = cpv(0, 190);
		cpVect groundB = cpv(256, 192);
		cpShape* ground = cpSegmentShapeNew(staticBody, groundA, groundB, 0.0);
		cpShapeSetFriction(ground, 1.0);
		cpShapeSetElasticity(ground, 0.5);
		cpSpaceAddShape(space, ground);
		cpVect wallRA = cpv(250, 0);
		cpVect wallRB = cpv(256, 192);
		cpShape* wallR = cpSegmentShapeNew(staticBody, wallRA, wallRB, 0.0);
		cpShapeSetFriction(wallR, 1.0);
		cpShapeSetElasticity(wallR, 0.5);
		cpSpaceAddShape(space, wallR);
		cpVect wallLA = cpv(0, 0);
		cpVect wallLB = cpv(6, 192);
		cpShape* wallL = cpSegmentShapeNew(staticBody, wallLA, wallLB, 0.0);
		cpShapeSetFriction(wallL, 1.0);
		cpShapeSetElasticity(wallL, 0.5);
		cpSpaceAddShape(space, wallL);
		cpVect ceilA = cpv(0, 0);
		cpVect ceilB = cpv(256, 2);
		cpShape* ceiling = cpSegmentShapeNew(staticBody, ceilA, ceilB, 0.0);
		cpShapeSetFriction(ceiling, 1.0);
		cpShapeSetElasticity(ceiling, 0.5);
		cpSpaceAddShape(space, ceiling);

	}

	void recreateBody(cpBody* oldBody, RigidObject& item, float scale)
	{
		if (item.sprite.getScale().x > scale)
		{
			float newWidth = item.sprite.getGlobalBounds().width * scale;
			float newHeight = item.sprite.getGlobalBounds().height * scale;
			float newMass = item.mass * (scale * scale);
			item.mass = newMass;
			item.friction *= 2.0f;
			item.bounciness /= 2.0f;
			// Create a new cpShape with different dimensions
			cpShape* newShape = cpBoxShapeNew(item.getBody(), newWidth, newHeight, 0.0);
			cpBodySetMass(oldBody, newMass);
			cpShapeSetFriction(newShape, item.friction);
			cpShapeSetElasticity(newShape, item.bounciness);

			// Remove the old shape and add the new shape to the same body
			cpSpaceRemoveShape(space, item.getShape());
			cpSpaceAddShape(space, newShape);

			// You can free the old shape if it's no longer needed
			cpShapeFree(item.getShape());
			item.setShape(newShape);
			item.sprite.setScale(scale, scale);
		}
	}

	void addRigidObject(RigidObject& obj)
	{
		objects.push_back(obj);
	}

	void updateWorld(PlayerClass& thePlayer, float _deltaTime)
	{
		cpBodyUpdatePosition(thePlayer.getBody(), _deltaTime);
		//			cpBodySetForce(k.getBody(), gravityVector);
		cpVect vec = cpBodyGetPosition(thePlayer.getBody());
		thePlayer.sprite.setPosition(vec.x, vec.y);
		float angle = cpBodyGetAngle(thePlayer.getBody());
		thePlayer.sprite.setRotation(angle * 180 / PI);

		for (auto& k : objects)
		{
			cpBodyUpdatePosition(k.getBody(), _deltaTime);
//			cpBodySetForce(k.getBody(), gravityVector);
			cpVect vec = cpBodyGetPosition(k.getBody());
			k.sprite.setPosition(vec.x, vec.y);
			float angle = cpBodyGetAngle(k.getBody());
			k.sprite.setRotation(angle * 180 / PI);
		}
		cpSpaceStep(space, _deltaTime);
	}

	void renderWorld(PlayerClass& thePlayer, sf::RenderWindow& _app)
	{
		for (auto& k : objects)
		{
			_app.draw(k.sprite);
		}
		_app.draw(thePlayer.sprite);
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

	sf::Keyboard::Key suctionKey = sf::Keyboard::V;

	// Define the strength of the suction
	float suctionStrength = 1000.0f;
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
	PlayerClass myPlayer;

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