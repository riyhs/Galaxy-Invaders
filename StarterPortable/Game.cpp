#include<Game.h>
#include <iostream>

// Private functions
void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(600, 800), "OKE JALAN", sf::Style::Default);
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
	this->pointText.setPosition(20.f, 25.f);
	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(70);
	this->pointText.setFillColor(sf::Color::White);
	this->pointText.setString("test");

	//Init game over text
	this->gameOverText.setFont(this->font);
	this->gameOverText.setStyle(sf::Text::Bold);
	this->gameOverText.setCharacterSize(150);
	this->gameOverText.setFillColor(sf::Color::Red);
	this->gameOverText.setString("GAME\nOVER");
	this->gameOverText.setPosition(
		this->window->getSize().x / 2.f - this->gameOverText.getGlobalBounds().width / 2.f,
		this->window->getSize().y / 2.f - this->gameOverText.getGlobalBounds().height / 2.f);

	//Init player GUI
	this->playerHpBar.setSize(sf::Vector2f(300.f, 25.f));
	this->playerHpBar.setFillColor(sf::Color::Red);
	this->playerHpBar.setPosition(sf::Vector2f(20.f, 20.f));

	this->playerHpBarBack = this->playerHpBar;
	this->playerHpBarBack.setFillColor(sf::Color(25, 25, 25, 200));
}

void Game::initBackground()
{
	if (!this->worldBackgroundTex.loadFromFile("Assets/Textures/background.jpg"))
	{
		std::cout << "ERROR::GAME::Failed to load background" << "\n";
	}
	this->worldBackground.setTexture(this->worldBackgroundTex);
}

void Game::initEndingBackground()
{
	if (!this->endingBackgroundTex.loadFromFile("Assets/Textures/ending.jpg"))
	{
		std::cout << "ERROR: Failed to load background" << "n";
	}
	this->endingBackground.setTexture(this->endingBackgroundTex);
}

void Game::initSystem()
{
	this->points = 0;
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
	this->initEndingBackground();
	this->initSystem();

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
	while(this->window->isOpen()) 
	{
		this->updatePollEvents();

		if(this->player->getHp() > 0)
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
				this->points += this->enemies[i]->getPoints();

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
	this->updateInput();
	
	this->player->update();

	this->updatecollision();
	
	this->updateBullets();
	
	this->updateEnemies();

	this->updateCombat();

	this->updateGUI();
	
	this->updateWorld();

	this->updateEndingBackground();
}

void Game::updateGUI()
{
	std::stringstream ss;
		
	ss << "points: " << this->points;

	this->pointText.setString(ss.str());

	//Update player GUI
	float hpPercent = static_cast<float>(this->player->getHp()) / this->player->getHpMax();
	this->playerHpBar.setSize(sf::Vector2f(300.f * hpPercent, this->playerHpBar.getSize().y));
}

void Game::updateWorld()
{
}

void Game::updateEndingBackground()
{
}

void Game::updatecollision()
{
	//left world collision
	if (this->player->getBounds().left < 0.f)
	{
		this->player->setPosition(0.f, this->player->getBounds().top);
	}

	//right world collision
	else if (this->player->getBounds().left + this->player->getBounds().width >= this->window->getSize().x)
	{
		this->player->setPosition(this->window->getSize().x - this->player->getBounds().width, this->player->getBounds().top);
	}
	
	//top world collision
	if (this->player->getBounds().top < 0.f)
	{
		this->player->setPosition(this->player->getBounds().left, 0.f);
	}

	//bottom world collision
	else if (this->player->getBounds().top + this->player->getBounds().height >= this->window->getSize().y)
	{
		this->player->setPosition(this->player->getBounds().left, this->window->getSize().y - this->player->getBounds().height);
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
			// Delete enemy
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
		}
		//Enemy player collision
		else if (enemy->getBounds().intersects(this->player->getBounds()))
		{
			this->player->loseHp(this->enemies.at(counter)->getDamage());
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
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
	this->window->draw(this->playerHpBarBack);
	this->window->draw(this->playerHpBar);
}

void Game::renderBackground()
{
	this->window->draw(this->worldBackground);
}

void Game::renderEndingBackground()
{
	if (this->player->getHp() <= 0)
		this->window->draw(this->endingBackground);
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

	//Game Over screen
	this->renderEndingBackground();

	if (this->player->getHp() <= 0)
		this->window->draw(this->gameOverText);

	this->window->display();
}
