#pragma once
#include"SFML\System.hpp"
#include"SFML\Graphics.hpp"

class Player
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

	float movementSpeed;

	float attackCooldown;
	float attackCooldownMax;

	// Private functions
	void initVariables();
	void initTexture();
	void initSprite();

public:
	Player();
	virtual ~Player();

	// Accessor
	const sf::Vector2f& getPos() const;
	const sf::FloatRect getBounds() const;

	// Functions
	void move(const float dirX, const float dirY);
	const bool canAttack();
	void updateAttack();
	void update();
	void render(sf::RenderTarget& target);
};

