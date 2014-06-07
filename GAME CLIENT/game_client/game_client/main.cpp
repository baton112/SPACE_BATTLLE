#include <SFML/Graphics.hpp>
#include "vehicle.h"
#include "game.h"
#include <iostream>
#include <string>

/*
#define RESOLUTION_X 1280
#define RESOLUTION_Y 720 
*/

#define RESOLUTION_X 640
#define RESOLUTION_Y 480 




int main()
{
	sf::RenderWindow window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y), "SPACE BATTLE - CLIENT");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);	

	
	game game; 
	game.runGameLoop(&window);


	return 0;
}