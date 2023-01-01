#pragma once
#include"SFML\Graphics.hpp"

class Bullet
{
private:
	sf::Sprite shape;
	sf::Texture* texture;

	sf::Vector2f direction;
	float movementSpeed;

public:
	//Bullet();
	Bullet(sf::Texture* texture, float posX, float posY, float dirX, float dirY, float movementSpeed);
	virtual ~Bullet();

	// Accessor
	const sf::FloatRect getBounds() const;

	// Functions
	void update();
	void render(sf::RenderTarget* target);
};

