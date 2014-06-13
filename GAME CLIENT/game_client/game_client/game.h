#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "vehicle.h"
#include <list>
#include <iostream>
#include <chrono>
#include "coin.h"

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <strsafe.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#define MAX_USERS 4

#define UNKNOWN -1

#define COIN_ID 4

#define SERVER_IP "127.0.0.1"
#define LISTEN_PORT 50100
#define COINS_NUMBER 5

DWORD WINAPI ThreadSend(LPVOID lpParam);


class game
{
private:
	vehicle *vehicleTab[MAX_USERS];
	coin *c[COINS_NUMBER];
	void loadTextures();
	std::list< sf::Event> keysPressed;
	bool vehiclesActive[MAX_USERS];
	int ID;
public:
	game();
	~game();
	void runGameLoop(sf::RenderWindow *appWindow);

};

