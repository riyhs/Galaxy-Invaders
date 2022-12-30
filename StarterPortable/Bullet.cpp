#include "Bullet.h"

//Bullet::Bullet()
//{
//
//}

Bullet::Bullet(sf::Texture* texture, float posX, float posY, float dirX, float dirY, float movementSpeed)
{
	this->shape.setTexture(*texture);
	this->shape.setScale(.07f, .07f);
	this->shape.setPosition(posX, posY);
	this->direction.x = dirX;
	this->direction.y = dirY;
	this->movementSpeed = movementSpeed;
}

Bullet::~Bullet()
{
}

const sf::FloatRect Bullet::getBounds() const
{
	return this->shape.getGlobalBounds();
}

// Functions
void Bullet::update()
{
	this->shape.move(this->movementSpeed * this->direction);
}

void Bullet::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}
