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

	// SFX & MUSIC
	sf::Music bgMusicMain;

	sf::SoundBuffer enemyDestroyedBuff;
	sf::SoundBuffer missileLaunchBuff;
	sf::SoundBuffer gameOverBuff;
	sf::SoundBuffer playerCollisionBuff;
	sf::Sound enemyDestroyedSound;
	sf::Sound missileLaunchSound;
	sf::Sound gameOverSound;
	sf::Sound playerCollisionSound;

	// GUI
	sf::Font font;
	sf::Text pointText;

	sf::Text gameOverText;
	sf::Text restartText;

	//Background
	sf::Texture worldBackgroundTex;
	sf::Sprite worldBackground;

	sf::Texture endingBackgroundTex;
	sf::Sprite endingBackground;

	//system
	unsigned points;

	// Player
	Player* player;

	// PlayerGUI
	sf::RectangleShape playerHpBar;
	sf::RectangleShape playerHpBarBack;

	// Enemy
	float spawnTimer;
	float spawnTimerMax;
	std::vector<Enemy*> enemies;

	// Private functions
	void initWindow();
	void initTextures();
	void initGUI();
	void initBackground();
	void initEndingBackground();
	void initSystem();
	void initAudio();

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
	void updateEndingBackground();
	void updatecollision();
	void updateEnemies();
	void updateCombat();
	void updatePollEvents();
	void updateInput();
	
	void playEnemyDestroyed();
	void playMissileLaunch();
	void playGameOver();
	void playPlayerCollision();

	void playBgMain();
	void stopBgMain();
	
	void renderGUI();
	void renderBackground();
	void renderEndingBackground();
	void render();
};

