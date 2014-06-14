#include "game.h"
//********************************************* SERWER 
struct ThreadParam{
	vehicle **MyVehicle;
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
	bool *vehActive;
};

//wysylanie pozycji "wsyzstkich do wszystkich"
DWORD WINAPI ThreadSendToEverybody(LPVOID lpParam)				
{
	HANDLE hStdout;
	ThreadParam *functionParams;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	functionParams = (ThreadParam*)lpParam;
	message * msg = new message;
	std::cout << "Connected " << functionParams->clietntNumber << std::endl;
	char *  buf = new char[(sizeof(message))];
	while (true)
	{

		for (int i = 0; i < 4; i++){
			if (functionParams->vehActive[i] && functionParams->clietntNumber != i){
				
				msg->ID = i;
				msg->X = functionParams->MyVehicle[i]->position.x;
				msg->Y = functionParams->MyVehicle[i]->position.y;
				msg->angle = functionParams->MyVehicle[i]->angle;
				send(functionParams->Connect, (char*)msg, sizeof(message), 0);
			}
		}
		Sleep(100/60); // magiczny sleep usuwajacy lagi - wysylanie 60 razy na sekunde 
	}
}

//odbior wiadomosci od klientow
DWORD WINAPI ThreadFunctionRecive(LPVOID lpParam)				
{
	HANDLE hStdout;
	ThreadParam *functionParams;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	functionParams = (ThreadParam*)lpParam;
	message * msg;
	std::cout << "Connected " << functionParams->clietntNumber << std::endl;
	char *  buf = new char[(sizeof(message))];
	while (1)
	{

		//recev -- blokuje dalsze wykowyanie programu do nadejscia otrzymania czegos lub zamkniecia polaczenia
		while (sizeof(message)==recv(functionParams->Connect, buf, sizeof(message), 0)) // przesylanie pojednym znaku 
		{
			 msg = (message*)buf;
			 if (msg->ID == UNKNOWN){											//jesli przyslal niewazne ID, (podłączył się), to odeslij mu ID tylko ID
				 msg->ID = functionParams->clietntNumber;
				 send(functionParams->Connect, (char*)&msg->ID, sizeof(int), 0);
			 }
			 else{
				 functionParams->MyVehicle[msg->ID]->position.x = msg->X;		//zapisywanie odebranyc pozycji
				 functionParams->MyVehicle[msg->ID]->position.y = msg->Y;
				 functionParams->MyVehicle[msg->ID]->angle = msg->angle;
			 }
		}

		std::cout << "koncze";
	}

	return 0;
}

//obsluga polaczen
DWORD WINAPI ThreadHandleConnections(LPVOID lpParam)
{
	SOCKET Listen = socket(AF_INET, SOCK_STREAM, NULL);
	SOCKET Connect = socket(AF_INET, SOCK_STREAM, NULL);

	SOCKADDR_IN server;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(LISTEN_PORT);
	int server_size = sizeof(server);
	int ConnectedClients = 0; /// liczba polaczonych kientow --- 0 - brak lientow

	int result = bind(Listen, (struct sockaddr FAR*)&server, sizeof(server));

	listen(Listen, SOMAXCONN); /// SOMAXCONN -- liczba maksymalnych polaczonych 

	HANDLE threadsHandleTab[MAX_USERS];
	std::cout << "LISTEN " << std::endl;
	vehicle **vehicleTab;
	netowrkThreadParams * parametry = (netowrkThreadParams*)lpParam;
	vehicleTab = parametry->vtab;

	for (;;)
	{
		if (Connect = accept(Listen, (struct sockaddr FAR*)&server, &server_size))
		{
			if (ConnectedClients < 3){
				std::cout << "CONNECTED " << std::endl;
				ConnectedClients++;
				parametry->vehActive[ConnectedClients] =  true;


				ThreadParam * a = new ThreadParam;				//przygotowanie parametru dla funkcji watkow
				a->MyVehicle = vehicleTab;
				a->clietntNumber = ConnectedClients;
				a->Connect = Connect;
				a->vehActive = parametry->vehActive;

				// -- odpalenie watku obslugi polaczenienia przychodzacego 
				threadsHandleTab[ConnectedClients] = CreateThread (
					NULL,                   // default security attributes
					0,                      // use default stack size  
					ThreadFunctionRecive,       // thread function name
					a,          // argument to thread function 
					0,                      // use default creation flags 
					NULL);   // returns the thread identifier

				Sleep(100);	//czekaj, zeby zdazyl przydzieic ID (bez tego sa lagi)

				// -- odpalenie watku do wysylki do wsztstkich 
				threadsHandleTab[ConnectedClients] = CreateThread(
					NULL,                   // default security attributes
					0,                      // use default stack size  
					ThreadSendToEverybody,       // thread function name
					a,          // argument to thread function 
					0,                      // use default creation flags 
					NULL);   // returns the thread identifier
			}
		}
	}

	return 0;
}

void game::runGameLoop(sf::RenderWindow *appWindow)
{
	std::chrono::high_resolution_clock::time_point time;
	time = std::chrono::high_resolution_clock::now();
	int ConnectedClients = 0;
	WSAData wsaData;
	WORD version = MAKEWORD(2, 2); ///wersja winsock

	HANDLE NetworkindThreadHandle;


	this->vehiclesActive[0]=true;

	int error = WSAStartup(version, &wsaData);
	if (error != 0)
	{
		std::cout << "Tell the user that we could not find a usable  Winsock DLL.  " << std::endl;
		system("pause");
		return ;
	}

	//-- tworzenie watku do oblugi polaczen 
	netowrkThreadParams a;
	a.vtab = this->vehicleTab; //przeslanie tablicy wskaznikow na pojazdy obslugiwane przez uzytkownikow
	a.vehActive = this->vehiclesActive;
	NetworkindThreadHandle = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		ThreadHandleConnections,       // thread function name
		&a,          // argument to thread function 
		0,                      // use default creation flags 
		NULL);   // returns the thread identifier
	//////////////////////-----------/////////////////////

	while (appWindow->isOpen())
	{
		std::chrono::high_resolution_clock::duration time_enlapsed = std::chrono::high_resolution_clock::now() - time;
		float delta = time_enlapsed.count() * 0.00001; // milisekundy //// 0.0000001 - dla sekund 
		time = std::chrono::high_resolution_clock::now();
		//std::cout << delta << std::endl;

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

		
		//czyszczenie okna 
		appWindow->clear();


		// rysowanie 
		if (vehicleTab != NULL)
		{
			for (int i = 0; i < MAX_USERS; i++)
			{
				if (this->vehiclesActive[i])
					vehicleTab[i]->drowVehicle(appWindow);
			}
		}
		//wyswietlenie okna
		appWindow->display();
	}

	WSACleanup();
}

game::game()
{
	for (int i = 0; i < MAX_USERS; i++)
	{
		vehicleTab[i] = new vehicle();
		vehiclesActive[i] = false;
	}
}



game::~game()
{
}


