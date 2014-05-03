#include <SFML/Graphics.hpp>
#include "vehicle.h"
#include "game.h"
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#include <ws2tcpip.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#define RESOLUTION_X 1280
#define RESOLUTION_Y 720 

#define LISTEN_PORT 50000

int main()
{
	//sf::RenderWindow window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y), "SPACE BATTLE - CLIENT");
	//window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);

	WSAData wsaData;
	WORD version = MAKEWORD(2,2); ///wersja winsock

	int error = WSAStartup(version, &wsaData);
	if (error != 0) 
	{
		std::cout << "Tell the user that we could not find a usable  Winsock DLL.  " << std::endl;
		system("pause");
		return 1;
	}

	SOCKET Listen = socket(AF_INET, SOCK_STREAM, NULL);
	SOCKET Connect = socket(AF_INET, SOCK_STREAM, NULL);

	std::cout<< "po zmianie";
	SOCKADDR_IN server;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(LISTEN_PORT);
	int server_size = sizeof(server);

	int result = bind(Listen, (struct sockaddr FAR*)&server,  sizeof(server));	

	listen(Listen, SOMAXCONN); /// SOMAXCONN -- liczba maksymalnych polaczonych 

	std::cout << "LISTEN " << std::endl;
	
	for (;;)
	{
		//// LACZENIE W TELLNET 
		/*
		open ip port
		*/

		if (Connect = accept(Listen, (struct sockaddr FAR*)&server, &server_size))
		{
			std::cout << "CONNECTED " << std::endl;
		}

		char  buf;
		//recev -- blokuje dalsze wykowyanie programu do nadejscia otrzymania czegos lub zamkniecia polaczenia
		while (recv(Connect, &buf, 1, 0) > 0 ) // przesylanie pojednym znaku 
		{
			printf("%c", buf);
		}
		
	}

	WSACleanup();



	
	//game game; 
	//game.runGameLoop(&window);


	return 0;
}
