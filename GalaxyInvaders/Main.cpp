#include"Game.h"
#include<time.h>

int main() {
	srand(time(0));

	Game game;

	game.run();

	return 0;
}