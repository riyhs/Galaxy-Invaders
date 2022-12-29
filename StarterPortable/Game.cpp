#include "Game.h"
#include <iostream>

// Private functions
void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "OKE JALAN", sf::Style::Default);
	this->window->setFramerateLimit(144);
}

void Game::initTextures()
{
	this->textures["BULLET"] = new sf::Texture();
	if (!this->textures["BULLET"]->loadFromFile("Assets/Textures/bullet2.png"))
	{
		std::cout << "ERROR::GAME::INITTEXTURE::Could not load texture file." << "\n";
	}
}

void Game::initGUI()
{
	// Load font
	if (!this->font.loadFromFile("Fonts/PixellettersFull.ttf"))
		std::cout << "ERROR::GAME::Failed to load font" << "\n";

	//Init point text
	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(12);
	this->pointText.setFillColor(sf::Color::White);
	this->pointText.setString("test");
}

void Game::initPlayer()
{
	this->player = new Player();
}

void Game::initEnemies()
{
	this->spawnTimerMax = 50.f;
	this->spawnTimer = this->spawnTimerMax;
}

Game::Game()	
{
	this->initWindow();
	this->initTextures();
	this->initGUI();
	this->initPlayer();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
	delete this->player;

	// Delete textures
	for (auto& i : this->textures)
	{
		delete i.second;
	}

	// Delete bullets
	for (auto& i : this->bullets)
	{
		delete i;
	}

	// Delete enemies
	for (auto& i : this->enemies)
	{
		delete i;
	}
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
	this->updatePollEvents();

	this->updateInput();
	
	this->player->update();
	
	this->updateBullets();
	
	this->updateEnemiesAndCombat();

	this->updateGUI();
}

void Game::updateGUI()
{

}

void Game::updateBullets()
{
	unsigned counter = 0;

	for (auto* bullet : this->bullets)
	{
		bullet->update();
	
		// Bullet out of screen
		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			// Delete bullet
			delete this->bullets.at(counter);
			this->bullets.erase(this->bullets.begin() + counter);
			--counter;
		}

		++counter;
	}
}

void Game::updateEnemiesAndCombat()
{
	this->spawnTimer += 0.5f;
	if (this->spawnTimer >= this->spawnTimerMax)
	{
		this->enemies.push_back(new Enemy(rand() % this->window->getSize().x-20.f, -100.f));
		this->spawnTimer = 0.f;
	}


	for (int i = 0; i < this->enemies.size(); ++i)
	{
		bool enemy_removed = false;
		this->enemies[i]->update();

		for (size_t k = 0; k < this->bullets.size() && !enemy_removed; k++)
		{
			if (this->bullets[k]->getBounds().intersects(this->enemies[k]->getBounds()))
			{
				this->bullets.erase(this->bullets.begin() + k);
				this->enemies.erase(this->enemies.begin() + i);
				enemy_removed = true;
			}
		}

		//Remove enemy at the bottom of the screen
		if (!enemy_removed)
		{
			if (this->enemies[i]->getBounds().top > this->window->getSize().y)
			{
				this->enemies.erase(this->enemies.begin() + i);
				std::cout << this->enemies.size() << "\n";
				enemy_removed = true;
			}
		}
	}
}

void Game::updatePollEvents()
{
	sf::Event e;

	while (this->window->pollEvent(e)) {
		if (e.Event::type == sf::Event::Closed)
			this->window->close();
		if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
			this->window->close();
	}
}

void Game::updateInput()
{
	// Move player
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->player->move(-1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->player->move(1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->player->move(0.f, -1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->player->move(0.f, 1.f);

	// Add bullets
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) || 
		sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) &&
		this->player->canAttack()) 
	{
		this->bullets.push_back(
			new Bullet(this->textures["BULLET"], 
			this->player->getPos().x + this->player->getBounds().width / 2.f, 
			this->player->getPos().y, 
			0.f, 
			-1.f,
			4.f)
		);
	}
}

void Game::renderGUI()
{
	this->window->draw(this->pointText);
}

void Game::render()
{
	this->window->clear();

	// Draw all the stufs
	this->player->render(*this->window);

	for (auto* bullet : this->bullets)
	{
		bullet->render(this->window);
	}

	for (auto* enemy : this->enemies)
	{
		enemy->render(this->window);
	}

	this->renderGUI();

	this->window->display();
}
