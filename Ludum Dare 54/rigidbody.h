#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "global.h"

constexpr float GRAVITY = 98.81f;

class RigidBody
{
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
    float bounciness; //0 to 1

    RigidBody(sf::Vector2f startPos, float startMass, float bounce)
        : position(startPos), mass(startMass), bounciness(bounce){}

    void applyForce(sf::Vector2f force)
    {
        // Update velocity based on applied force and mass
        // F = m * a => a = F / m
        velocity += force / mass;
    }

    void update(float deltaTime)
    {
        // Update position based on velocity and time
        position += velocity * deltaTime;

    }
};

class RectangularBody : public RigidBody
{
private:
    sf::RectangleShape shape;

public:
    RectangularBody(sf::Vector2f startPos, float startMass, sf::Vector2f size, float bounce)
        : RigidBody(startPos, startMass, bounce)
    {
        shape.setSize(size);
        shape.setFillColor(sf::Color::Green); // You can set the rectangle's color
        shape.setPosition(startPos);
    }
    void setColor(sf::Color _color)
    {
        shape.setFillColor(_color);
    }
    sf::RectangleShape getShape() { return shape; }

    void handleCollision(RectangularBody& other) {
        sf::FloatRect thisBounds = shape.getGlobalBounds();
        sf::FloatRect otherBounds = other.getShape().getGlobalBounds();

        if (thisBounds.intersects(otherBounds)) {
            // Simple elastic collision response
            sf::Vector2f relativeVelocity = velocity - other.velocity;
            sf::Vector2f collisionNormal = ic::normalVector(sf::Vector2f(
                thisBounds.left + thisBounds.width / 2.0f - (otherBounds.left + otherBounds.width / 2.0f),
                thisBounds.top + thisBounds.height / 2.0f - (otherBounds.top + otherBounds.height / 2.0f)
            ));
            float dotProduct = relativeVelocity.x * collisionNormal.x + relativeVelocity.y * collisionNormal.y;

            // Check if objects are moving towards each other
            if (dotProduct < 0) {
                // Calculate new velocities
                float combinedMass = std::max(other.mass+mass, 1.0f); // You might want to use actual mass here
                sf::Vector2f impulse = (-(1.0f + bounciness) * dotProduct) / combinedMass * collisionNormal;

                velocity += impulse;
            }
        }
    }
    void update(float deltaTime)
    {
        position += velocity * deltaTime;

        if (position.y + shape.getSize().y > 192) // Adjust for the rectangle's height
        {
            float friction = 0.99f;
            // Stop the object by setting its velocity to zero
            velocity.x *= friction;
            velocity.y = 0;
            // Place the object exactly at the bottom of the screen
            position.y = 192 - shape.getSize().y;
        }
        shape.setPosition(position);

    }
    void draw(sf::RenderWindow& window)
    {
        window.draw(shape);
    }
};


#endif//RIGIDBODY_H
