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
#include <tchar.h>
#include <strsafe.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#define MAX_USERS 4

#define UNKNOWN -1

#define SERVER_IP "127.0.0.1"
#define LISTEN_PORT 50100

//obsluga poaczen 
DWORD WINAPI ThreadHandleConnections(LPVOID lpParam);
//wysylanie eventow do servera
DWORD WINAPI ThreadSend(LPVOID lpParam);
//odbieranie pozycji od servera
DWORD WINAPI ThreadFunctionRecive(LPVOID lpParam);

class game
{
private:
	vehicle *vehicleTab[MAX_USERS];
	void loadTextures();
	std::list< sf::Event> keysPressed;
	bool vehiclesActive[MAX_USERS];
	int ID;
public:
	game();
	~game();
	void runGameLoop(sf::RenderWindow *appWindow);

};


struct ThreadParam{
	vehicle **MyVehicle;
	SOCKET Connect;
	int clietntNumber;
	bool *vehActive;
	int * ID;
	std::list< sf::Event> *keysPressed;
};

struct EventMassage{
	int ID;
	sf::Event keysPressed;
};

struct message{
	int ID;
	double X;
	double Y;
	double angle;
};

struct netowrkThreadParams{
	vehicle ** vtab;
	std::list< sf::Event> *keysPressed;
	bool *vehActive;
	int * ID;
};