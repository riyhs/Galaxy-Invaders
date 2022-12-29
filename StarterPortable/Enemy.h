#pragma once
#include"SFML\Graphics.hpp"

class Enemy
{
private:
	sf::CircleShape shape;
	int type;
	float speed;
	int hp;
	int hpMax;
	int damage;
	int points;

	// Private Functions
	void initShape();
	void initVariables();
public:
	Enemy(float posX, float posY);
	virtual ~Enemy();

	// Accessors
	const sf::FloatRect getBounds() const

	// Functions
	void update();
	void render(sf::RenderTarget* target);
};

