#pragma once
#include<map>
#include<string>
#include<sstream>
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

	//Background
	sf::Texture worldBackgroundTex;
	sf::Sprite worldBackground;

	//system
	unsigned points;

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
	void initBackground();
	void initSystem();

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
	void updatecollision();
	void updateEnemies();
	void updateCombat();
	void updatePollEvents();
	void updateInput();
	void renderGUI();
	void renderBackground();
	void render();
};

