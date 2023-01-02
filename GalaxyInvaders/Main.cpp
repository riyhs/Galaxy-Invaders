/*
* GALAXY INVADERS
* Final Project Konsep Pemrograman 2022
* 
* - Naufal Ega Setyo Nasrudin      	( L0122123 )
* - Rifqi Ghaida Sakhi              ( L0122139 )
* - Rista Purnamasari               ( L0122140 )
* - Riyaldi Hasan Setiawan          ( L0122141 )
* - Masud Zahron Notosoewito		( L0122159 )
*/

#include"Game.h"
#include<time.h>

int main() {
	srand(time(0));

	Game game;

	game.run();

	return 0;
}