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
	

	vehicle *vehicleTab[MAX_USERS];
	for (int i = 0; i < MAX_USERS; i++)
	{
		vehicleTab[i] = NULL;
	}
	
	
	

	//Uruchomienie gry 
	game game(vehicleTab);
	game.runGameLoop(&window);


	
	
	

	


	return 0;
}
