#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "vehicle.h"
#include <list>
#include <iostream>
#include <chrono>
#include <sstream>
#include "coin.h"

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <strsafe.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#define MAX_USERS 4

#define LISTEN_PORT 50100

#define UNKNOWN -1

#define COIN_ID 4

class game
{
private: 
	vehicle *vehicleTab[MAX_USERS];
	int playerScore[MAX_USERS];
	bool vehiclesActive[MAX_USERS]; //czy wyswietlac?
	void loadTextures();
	std::list< sf::Event> keysPressed;
	sf::Font font;
	coin **c;

	
public:
	game();
	~game();
	void runGameLoop(sf::RenderWindow *appWindow);
	void drowScore(sf::RenderWindow *appWindow);
	void loadFont();
	
};

