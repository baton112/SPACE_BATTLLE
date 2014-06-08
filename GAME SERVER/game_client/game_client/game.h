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

#define LISTEN_PORT 50100

#define UNKNOWN -1

//wysylanie pozycji "wsyzstkich do wszystkich"
DWORD WINAPI ThreadSendToEverybody(LPVOID lpParam);
//odbior wiadomosci od klientow
DWORD WINAPI ThreadFunctionRecive(LPVOID lpParam);
//obsluga polaczen
DWORD WINAPI ThreadHandleConnections(LPVOID lpParam);

struct EventMassage{
	int ID;
	sf::Event keysPressed;
};

class game
{
private: 
	vehicle *vehicleTab[MAX_USERS];
	bool vehiclesActive[MAX_USERS]; //czy wyswietlac?
	void loadTextures();
	std::list< EventMassage> keysPressedClientList;
	
public:
	game();
	~game();
	void addEvent();
	void runGameLoop(sf::RenderWindow *appWindow);
	
};

struct ThreadParam{
	vehicle **MyVehicle;
	std::list< EventMassage> *keysPressedClientList;
	SOCKET Connect;
	int clietntNumber;
	bool * vehActive;
};

struct message{
	int ID;
	double X;
	double Y;
	double angle;
};


struct netowrkThreadParams{
	vehicle ** vtab;
	std::list< EventMassage> *keysPressedClientList;
	bool *vehActive;
};
