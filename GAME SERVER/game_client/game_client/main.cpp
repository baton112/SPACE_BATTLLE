#include <SFML/Graphics.hpp>

//#include "vehicle.h"
#include "game.h"
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#include <ws2tcpip.h>
#include <stdio.h>

#include <tchar.h>
#include <strsafe.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")


#define RESOLUTION_X 1280
#define RESOLUTION_Y 720 

#define LISTEN_PORT 50000

#define MAX_USERS 4


struct ThreadParam{
	vehicle *MyVehicle;
	SOCKET Connect;
	int clietntNumber;

};

DWORD WINAPI ThreadFunctionRecive(LPVOID lpParam)
{
	HANDLE hStdout;
	ThreadParam *functionParams;

	//TCHAR msgBuf[BUF_SIZE];
	//size_t cchStringSize;
	//DWORD dwChars;

	// Make sure there is a console to receive output results. 

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	// Cast the parameter to the correct data type.
	// The pointer is known to be valid because 
	// it was checked for NULL before the thread was created.

	functionParams = (ThreadParam*)lpParam;

	// Print the parameter values using thread-safe functions.

	//StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d\n"),
	//	pDataArray->val1, pDataArray->val2);
	//StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
	//WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

	std::cout << "Connected " << functionParams->clietntNumber << std::endl;

	while (1)
	{
		//std::cout << functionParams->clietntNumber << std::endl;
		char  buf;
		//recev -- blokuje dalsze wykowyanie programu do nadejscia otrzymania czegos lub zamkniecia polaczenia
		while (recv(functionParams->Connect, &buf, 1, 0) > 0) // przesylanie pojednym znaku 
		{
			printf("%c", buf);
		}
		std::cout << "a";
	}
	
	/*printf("Received parameters: i %d=, msg = \n", functionParams->clietntNumber);

	//PMYDATA pDataArray;
	#define BUF_SIZE 255
	TCHAR msgBuf[BUF_SIZE];
	size_t cchStringSize;
	DWORD dwChars;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d\n"),
		11, 22);
	StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
	WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
	*/
	return 0;
}


int main()
{
	//sf::RenderWindow window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y), "SPACE BATTLE - CLIENT");
	//window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);
	
	WSAData wsaData;
	WORD version = MAKEWORD(2,2); ///wersja winsock

	int ConnectedClients = 0; 
	vehicle *vehicleTab[10];
	
	HANDLE threadsHandleTab[MAX_USERS];
	
	
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
			vehicleTab[ConnectedClients] = new vehicle;
			ThreadParam *a;
			a = new ThreadParam;
			a->MyVehicle = vehicleTab[ConnectedClients];
			a->clietntNumber = ConnectedClients;
			a->Connect = Connect;
			//odpalenie watku obslugi polaczenienia przychodzacego 
			threadsHandleTab[0] = CreateThread(
				NULL,                   // default security attributes
				0,                      // use default stack size  
				ThreadFunctionRecive,       // thread function name
				a,          // argument to thread function 
				0,                      // use default creation flags 
				NULL);   // returns the thread identifier

			//Sleep(10);

			//WaitForSingleObject(threadsHandleTab[0], INFINITE);
			ConnectedClients++;

		}
		//Sleep(10);
		
		
	}

	//WSACleanup();

	

	//Uruchomienie gry 
	//game game; 
	//game.runGameLoop(&window);


	return 0;
}
