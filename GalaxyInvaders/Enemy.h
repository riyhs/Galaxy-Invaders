#pragma once
#include"SFML\Graphics.hpp"

class Enemy
{
private:
	unsigned pointCount;
	sf::Texture rockTex;
	sf::CircleShape shape;
	int type;
	float speed;
	int hp;
	int hpMax;
	int damage;
	int points;

	// Private Functions
	void initVariables();
	void initTextures();
	void initShape();

public:
	Enemy(float posX, float posY);
	virtual ~Enemy();

	// Accessors
	const sf::FloatRect getBounds() const;
	const int& getPoints() const;
	const int& getDamage() const;
	const int& getHp() const;


	// Functions
	void update();
	void render(sf::RenderTarget* target);
	void decreaseHp();
};

