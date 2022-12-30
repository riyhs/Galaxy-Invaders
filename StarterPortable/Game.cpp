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
	if (!this->textures["BULLET"]->loadFromFile("Assets/Textures/fireball.png"))
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

void Game::initBackground()
{
	if (!this->worldBackgroundTex.loadFromFile("Assets/Textures/background.jpg"))
	{
		std::cout << "ERROR::GAME::Failed to load background" << "\n";
	}
	this->worldBackground.setTexture(this->worldBackgroundTex);
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
	this->initBackground();
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

void Game::updateCombat()
{
	for (int i = 0; i < this->enemies.size(); ++i)
	{
		bool enemy_deleted = false;
		for (size_t k = 0; k < this->bullets.size() && enemy_deleted == false; k++)
		{
			if (this->enemies[i]->getBounds().intersects(this->bullets[k]->getBounds()))
			{
				delete this->enemies[i];
				this->enemies.erase(this->enemies.begin() + i);

				delete this->bullets[k];
				this->bullets.erase(this->bullets.begin() + k);

				enemy_deleted = true;
			}
		}
	}
}

void Game::update()
{
	this->updatePollEvents();

	this->updateInput();
	
	this->player->update();

	this->updatecollision();
	
	this->updateBullets();
	
	this->updateEnemies();

	this->updateCombat();

	this->updateGUI();
	
	this->updateWorld();
}

void Game::updateGUI()
{

}

void Game::updateWorld()
{
}

void Game::updatecollision()
{
	if (this->player->getBounds().left < 0.f)
	{
		this->player->setPosition(0.f, this->player->getBounds().top);
	}
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

void Game::updateEnemies()
{
	//Spawning
	this->spawnTimer += 0.5f;
	if (this->spawnTimer >= this->spawnTimerMax)
	{
		this->enemies.push_back(new Enemy(rand() % this->window->getSize().x-20.f, -100.f));
		this->spawnTimer = 0.f;
	}

	//Update
	unsigned counter = 0;
	for (auto* enemy : this->enemies)
	{
		enemy->update();

		// Bullet out of screen
		if (enemy->getBounds().top > this->window->getSize().y)
		{
			// Delete bullet
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
			--counter;
		}

		++counter;
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
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) /* ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))*/) &&
		this->player->canAttack()) 
	{
		this->bullets.push_back(
			new Bullet(this->textures["BULLET"], 
			this->player->getPos().x + 33/* + this->player->getBounds().width / 2.f */ ,
			this->player->getPos().y - 20, 
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

void Game::renderBackground()
{
	this->window->draw(this->worldBackground);
}

void Game::render()
{
	this->window->clear();

	//Draw background
	this->renderBackground();

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
