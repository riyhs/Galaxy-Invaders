#include "Game.h"

void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "OKE JALAN", sf::Style::Default);
}

Game::Game()	
{
	this->initWindow();
}

Game::~Game()
{
	delete this->window;
}

// Functions
void Game::run()
{
	while(this->window->isOpen()) {
		this->update();
		this->render();
	}

}

void Game::update()
{
}

void Game::render()
{
	this->window->clear();

	this->window->display();
}
