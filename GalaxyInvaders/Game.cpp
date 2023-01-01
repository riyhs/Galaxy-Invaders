#include<Game.h>
#include <iostream>

// Private functions
void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(700, 900), "Galaxy Invaders", sf::Style::Default);
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
	if (!this->font.loadFromFile("Assets/Fonts/PixellettersFull.ttf"))
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
	this->gameOverText.setCharacterSize(120);
	this->gameOverText.setFillColor(sf::Color::Red);
	this->gameOverText.setString("GAME OVER");
	this->gameOverText.setPosition(
		this->window->getSize().x / 2.f - this->gameOverText.getGlobalBounds().width / 2.f,
		this->window->getSize().y / 3.f /* - this->gameOverText.getGlobalBounds().height / 2.f */
	);

	//Init restart text
	this->restartText.setFont(this->font);
	this->restartText.setStyle(sf::Text::Bold | sf::Text::Italic);
	this->restartText.setCharacterSize(70);
	this->restartText.setFillColor(sf::Color::Red);
	this->restartText.setString("TAP ENTER TO RESTART");
	this->restartText.setPosition(
		this->window->getSize().x / 2.f - this->restartText.getGlobalBounds().width / 2.f,
		this->window->getSize().y / 2.f - this->restartText.getGlobalBounds().height / 2.f
	);

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

void Game::initAudio()
{
	enemyDestroyedBuff.loadFromFile("Assets/SFX/whereisthepoint.wav");
	missileLaunchBuff.loadFromFile("Assets/SFX/Explosion_103.wav");
	gameOverBuff.loadFromFile("Assets/SFX/gameover_loud.wav");
	playerCollisionBuff.loadFromFile("Assets/SFX/Hit_hurt_200_(34).wav");

	bgMusicMain.openFromFile("Assets/Music/Music_BG.wav");
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
	this->initAudio();

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
	playBgMain();

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
			// Bullet colllision with enemy
			if (this->enemies[i]->getBounds().intersects(this->bullets[k]->getBounds()))
			{
				if (this->enemies[i]->getHp() > 0) {
					this->enemies[i]->decreaseHp();

					delete this->bullets[k];
					this->bullets.erase(this->bullets.begin() + k);
				}
				else {					
					this->playEnemyDestroyed();
					this->points += this->enemies[i]->getPoints();
				
					delete this->enemies[i];
					this->enemies.erase(this->enemies.begin() + i);
					
					enemy_deleted = true;
				}
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
	this->playGameOver();
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

		// Enemy out of screen
		if (enemy->getBounds().top > this->window->getSize().y)
		{
			// Delete enemy
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
		}
		//Enemy player collision
		else if (enemy->getBounds().intersects(this->player->getBounds()))
		{
			this->playPlayerCollision();
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->player->move(-1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->player->move(1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		this->player->move(0.f, -1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		this->player->move(0.f, 1.f);

	// Add bullets
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || 
		(sf::Mouse::isButtonPressed(sf::Mouse::Left))) && 
		this->player->canAttack())
	{
		this->bullets.push_back(
			new Bullet(this->textures["BULLET"], 
			this->player->getPos().x + 62,
			this->player->getPos().y - 31, 
			0.f, 
			-1.f,
			4.f)
		);

		this->playMissileLaunch();
	}
}

void Game::playEnemyDestroyed()
{
	enemyDestroyedSound.setBuffer(enemyDestroyedBuff);
	enemyDestroyedSound.setVolume(35.f); // Make the sound volume to 35% of it's original
	enemyDestroyedSound.play();
}

void Game::playMissileLaunch()
{
	missileLaunchSound.setBuffer(missileLaunchBuff);
	missileLaunchSound.setVolume(35.f); // Make the sound volume to 35% of it's original
	missileLaunchSound.play();
}

void Game::playGameOver()
{
	gameOverSound.setBuffer(gameOverBuff);
	gameOverSound.setVolume(35.f); // Make the sound volume to 35% of it's original
	gameOverSound.play();
}

void Game::playPlayerCollision()
{
	playerCollisionSound.setBuffer(playerCollisionBuff);
	playerCollisionSound.setVolume(55.f); // Make the sound volume to 55% of it's original
	playerCollisionSound.play();
}

void Game::playBgMain()
{
	bgMusicMain.play();
	bgMusicMain.setLoop(true);
}

void Game::stopBgMain()
{
	bgMusicMain.stop();
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
	{
		this->window->draw(this->gameOverText);
		this->window->draw(this->restartText);
		this->stopBgMain();

		// Restart Game
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			this->player->setPosition(
				this->window->getSize().x / 2.f - this->player->getBounds().width / 2.f, //Middle X axle
				this->window->getSize().y - 200.f //200 off the bottom
			);
			this->points = 0;
			this->player->setHp(100);
			this->playBgMain();
		}
	}

	this->window->display();
}
