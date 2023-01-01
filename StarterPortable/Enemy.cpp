#include "Enemy.h"
#include<iostream>

void Enemy::initVariables()
{
	this->pointCount = rand() % 8 + 5; // min = 5, max = 12
	//this->type		= 0;
	this->speed		= static_cast<float>(this->pointCount/3);
	this->hpMax		= static_cast<int>(this->pointCount);
	this->hp		= this->hpMax;
	this->damage	= this->pointCount;
	this->points	= this->pointCount / 3;
}

// Private Functions
void Enemy::initShape()
{
	this->shape.setRadius(this->pointCount * 5);
	this->shape.setPointCount(this->pointCount);
	this->shape.setTexture(&rockTex);
	//this->shape.setFillColor(sf::Color(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1, 255));
}

//Rock texture to shapes
void Enemy::initTextures()
{
	if (!this->rockTex.loadFromFile("Assets/Textures/rock.jpg"))
	{
		std::cout << "ERROR: Could not load texture file." << "\n";
	}
}


Enemy::Enemy(float posX, float posY)
{
	this->initVariables();
	this->initTextures();
	this->initShape();

	this->shape.setPosition(posX, posY);
}

Enemy::~Enemy()
{
}

// Accessors
const sf::FloatRect Enemy::getBounds() const
{
	return this->shape.getGlobalBounds();
}

const int& Enemy::getPoints() const
{
	return this->points;
}

const int& Enemy::getDamage() const
{
	return this->damage;
}

// Functions
void Enemy::update()
{
	this->shape.move(0.f, this->speed);
}

void Enemy::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}
