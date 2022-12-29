#pragma once
#include"SFML\Graphics.hpp"

class Enemy
{
private:
	sf::CircleShape shape;
	int type;
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

	// Functions
	void update();
	void render(sf::RenderTarget* target);
};

