#pragma once
#include<map>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

class Game
{
private:
	sf::RenderWindow* window;

	// Resources
	std::map<std::string, sf::Texture*> textures;
	std::vector<Bullet*> bullets;


	// GUI
	sf::Font font;
	sf::Text pointText;

	//World
	sf::Texture worldBackgroundTex;
	sf::Sprite worldBackground;

	// Player
	Player* player;

	// Enemy
	float spawnTimer;
	float spawnTimerMax;
	std::vector<Enemy*> enemies;

	// Private functions
	void initWindow();
	void initTextures();
	void initGUI();
	void initWorld();

	void initPlayer();
	void initEnemies();

public:
	Game();
	virtual ~Game();

	// Functions
	void run();

	void update();
	void updateBullets();
	void updateGUI();
	void updateWorld();
	void updateCollision();
	void updateEnemies();
	void updateCombat();
	void updatePollEvents();
	void updateInput();

	void renderGUI();
	void renderWorld();
	void render();
};

