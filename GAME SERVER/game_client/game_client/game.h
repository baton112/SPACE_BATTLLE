#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "vehicle.h"
#include <list>
#include <iostream>
#include <chrono>

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#include <ws2tcpip.h>
#include <stdio.h>

#include <tchar.h>
#include <strsafe.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#define MAX_USERS 4

#define LISTEN_PORT 50000

class game
{
private: 
	vehicle **vehicleTab;
	void loadTextures();
	std::list< sf::Event> keysPressed;
	
public:
	game();
	game(vehicle **vtab);
	~game();
	void runGameLoop(sf::RenderWindow *appWindow);
	
};

