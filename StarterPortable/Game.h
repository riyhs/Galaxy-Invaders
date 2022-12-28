#pragma once
#include"SFML\System.hpp"
#include"SFML\Graphics.hpp"

class Game
{
private:
	sf::RenderWindow* window;

	void initWindow();

public:
	Game();
	virtual ~Game();

	// Functions
	void run();
	void update();
	void render();
};

