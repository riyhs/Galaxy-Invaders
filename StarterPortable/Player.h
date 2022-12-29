#pragma once
#include"SFML\System.hpp"
#include"SFML\Graphics.hpp"

class Player
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

	float movementSpeed;

	// Private functions
	void initTexture();
	void initSprite();

public:
	Player();
	virtual ~Player();

	// Functions
	void move(const float dirX, const float dirY);
	void update();
	void render(sf::RenderTarget& target);
};

