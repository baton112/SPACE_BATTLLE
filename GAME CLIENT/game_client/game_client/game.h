#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "vehicle.h"
#include <list>
#include <iostream>

class game
{
private: 
	vehicle tmpVehicle;
	void loadTextures();
	std::list< sf::Event> keysPressed;
	
public:
	game();
	~game();
	void runGameLoop(sf::RenderWindow *appWindow);
	
};

