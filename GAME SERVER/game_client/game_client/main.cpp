#include <SFML/Graphics.hpp>

#define RESOLUTION_X 1280
#define RESOLUTION_Y 720 




//#include "vehicle.h"
#include "game.h"
#include <iostream>



int main()
{
	sf::RenderWindow window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y), "SPACE BATTLE - CLIENT");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	
	//Uruchomienie gry 
	game game;
	game.runGameLoop(&window);


	return 0;
}
