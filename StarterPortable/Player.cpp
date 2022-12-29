#include "Player.h"
#include<iostream>

// Private functions
void Player::initTexture()
{
	if (!this->texture.loadFromFile("Assets/Textures/ship.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture file." << "\n";
	}
}

void Player::initSprite()
{
	this->sprite.setTexture(this->texture);

	// Resize sprite
	this->sprite.scale(0.15f, 0.15f);
}

Player::Player()
{
	this->movementSpeed = 2.f;

	this->initTexture();
	this->initSprite();
}

Player::~Player()
{
}

// Functions
void Player::move(const float dirX, const float dirY)
{
	this->sprite.move(this->movementSpeed * dirX, this->movementSpeed * dirY);
}

void Player::update()
{
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
