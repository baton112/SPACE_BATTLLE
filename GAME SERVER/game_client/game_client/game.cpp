#include "game.h"




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



void game::runGameLoop(sf::RenderWindow *appWindow)
{
	std::chrono::high_resolution_clock::time_point time;
	time = std::chrono::high_resolution_clock::now();
	int ConnectedClients = 0;
	WSAData wsaData;
	WORD version = MAKEWORD(2, 2); ///wersja winsock

	HANDLE threadsHandleTab[MAX_USERS];


	int error = WSAStartup(version, &wsaData);
	if (error != 0)
	{
		std::cout << "Tell the user that we could not find a usable  Winsock DLL.  " << std::endl;
		system("pause");
		return ;
	}

	SOCKET Listen = socket(AF_INET, SOCK_STREAM, NULL);
	SOCKET Connect = socket(AF_INET, SOCK_STREAM, NULL);

	std::cout << "po zmianie";
	SOCKADDR_IN server;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(LISTEN_PORT);
	int server_size = sizeof(server);

	int result = bind(Listen, (struct sockaddr FAR*)&server, sizeof(server));

	listen(Listen, SOMAXCONN); /// SOMAXCONN -- liczba maksymalnych polaczonych 

	std::cout << "LISTEN " << std::endl;
	//sf::Window window;
	//window.create(sf::VideoMode(800, 600), "My window");

	while (appWindow->isOpen())
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

		


		std::chrono::high_resolution_clock::duration time_enlapsed = std::chrono::high_resolution_clock::now() - time;
		float delta = time_enlapsed.count() * 0.00001; // milisekundy //// 0.0000001 - dla sekund 
		time = std::chrono::high_resolution_clock::now();
		//std::cout << delta << std::endl;

		/*
		sf::Event event;
		while (appWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				appWindow->close();
			if (event.type == sf::Event::KeyPressed)
			{
				// wcisniecie klawisza dodaje go do listy klawiszy wcisnietych 
				bool isOnList = false;
				if (!keysPressed.empty())
				{
					for (std::list< sf::Event>::iterator iter = keysPressed.begin(); iter != keysPressed.end(); iter++)
					{
						if (iter->key.code == event.key.code)
							isOnList = true;
					}
				}
				if (!isOnList) keysPressed.push_front(event);
			}
			if (event.type == sf::Event::KeyReleased)
			{
				//usuwanie z listy klawisza ktory zostal puszczony 
				std::list< sf::Event>::iterator i;
				for (std::list< sf::Event>::iterator iter = keysPressed.begin(); iter != keysPressed.end(); iter++)
				{
					if (iter->key.code == event.key.code)
						i = iter;
				}
				keysPressed.erase(i);
			}

		};

		*/
		/*
		if (vehicleTab != NULL && vehicleTab[0] != NULL)
		{
			// obsluga klawiszy znajdujacych sie na liscie keyPressed
			for (std::list< sf::Event>::iterator iter = keysPressed.begin(); iter != keysPressed.end(); iter++)
			{
				if (iter->key.code == sf::Keyboard::W)
					vehicleTab[0]->buttonAction(direction::up, delta);
				if (iter->key.code == sf::Keyboard::A)
					vehicleTab[0]->buttonAction(direction::left, delta);
				if (iter->key.code == sf::Keyboard::S)
					vehicleTab[0]->buttonAction(direction::down, delta);
				if (iter->key.code == sf::Keyboard::D)
					vehicleTab[0]->buttonAction(direction::right, delta);
				if (iter->key.code == sf::Keyboard::Up)
					vehicleTab[0]->buttonAction(direction::up, delta);
				if (iter->key.code == sf::Keyboard::Left)
					vehicleTab[0]->buttonAction(direction::left, delta);
				if (iter->key.code == sf::Keyboard::Down)
					vehicleTab[0]->buttonAction(direction::down, delta);
				if (iter->key.code == sf::Keyboard::Right)
					vehicleTab[0]->buttonAction(direction::right, delta);
			}
		}

		*/
		//czyszczenie okna 
		appWindow->clear();


		// rysowanie 
		/*if (vehicleTab != NULL)
		{
			for (int i = 0; i < MAX_USERS; i++)
			{
				if (this->vehicleTab[i] != NULL)
					vehicleTab[i]->drowVehicle(appWindow);
			}
		}*/
		

		//wyswietlenie okna
		appWindow->display();
	}

	WSACleanup();

}

game::game()
{

}


game::game(vehicle **vtab)
{
	this->vehicleTab = vtab;
}


game::~game()
{
}


