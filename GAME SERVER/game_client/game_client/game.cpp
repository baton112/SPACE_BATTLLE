#include "game.h"
//********************************************* SERWER 

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
	std::cout << "Connected " << functionParams->clietntNumber << "wysylanie czegos "<< std::endl;
	char *  buf = new char[(sizeof(message))];
	while (true)
	{

		for (int i = 0; i < MAX_USERS; i++){
			if (functionParams->vehActive[i] && functionParams->clietntNumber != i){
				
				msg->ID = i;
				msg->X = functionParams->MyVehicle[i]->position.x;
				msg->Y = functionParams->MyVehicle[i]->position.y;
				msg->angle = functionParams->MyVehicle[i]->angle;
				send(functionParams->Connect, (char*)msg, sizeof(message), 0);
				std::cout << "wyslano pozucjeeee" << msg->X << " " << msg->Y << " " << msg->angle << " " << msg->ID  << std::endl;
			}
		}
		//Sleep(100/60); // magiczny sleep usuwajacy lagi - wysylanie 60 razy na sekunde 
		Sleep(1000);
		
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
	std::cout << "Connected " << functionParams->clietntNumber << std::endl;
	std::cout << "Odbieram dane od " << functionParams->clietntNumber << std::endl;
	EventMassage *msg;
	char *  buf = new char[(sizeof(EventMassage))];

	//recev -- blokuje dalsze wykowyanie programu do nadejscia otrzymania czegos lub zamkniecia 
	for (;;)
	{
		while (sizeof(EventMassage) == recv(functionParams->Connect, buf, sizeof(EventMassage), 0)) // przesylanie pojednym znaku 
		{
			//-- eventMassage
			msg = (EventMassage*)buf;
			if (msg->ID == UNKNOWN)
			{											//jesli przyslal niewazne ID, (podłączył się), to odeslij mu ID tylko ID
				msg->ID = functionParams->clietntNumber;
				send(functionParams->Connect, (char*)&msg->ID, sizeof(int), 0);
			}
			else
			{
				if (msg->keysPressed.type == sf::Event::KeyPressed) // dodanie odebranego klawisza do listy - bez powtorek
				{
					bool isOnList = false;
					if (!functionParams->keysPressedClientList->empty())
					{
						for (std::list<EventMassage>::iterator iter = functionParams->keysPressedClientList->begin();
							iter != functionParams->keysPressedClientList->end(); iter++)
						{
							if (iter->keysPressed.key.code == msg->keysPressed.key.code && msg->ID == iter->ID)
								isOnList = true;
						}
					}
					if (!isOnList) functionParams->keysPressedClientList->push_front(*msg);
				}
				else if (msg->keysPressed.type == sf::Event::KeyReleased) // usuniecie klawisza z listy 
				{
					//usuwanie z listy klawisza ktory zostal puszczony
					std::list< EventMassage>::iterator i;
					for (std::list<EventMassage>::iterator iter = functionParams->keysPressedClientList->begin(); 
						iter != functionParams->keysPressedClientList->end(); iter++)
					{
						if (iter->keysPressed.key.code == msg->keysPressed.key.code && iter->ID == msg->ID)
							i = iter;
					}
					functionParams->keysPressedClientList->erase(i);
				}
				//std::cout << "Odebrano " << msg->ID << " klawisz " << msg->keysPressed.key.code << std::endl;;
			}

		}
	}

	std::cout << "zakonczono polaczenie !!!!!" << std::endl;

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
	netowrkThreadParams * parametry = (netowrkThreadParams*)lpParam;

	for (;;)
	{
		if (Connect = accept(Listen, (struct sockaddr FAR*)&server, &server_size))
		{
			if (ConnectedClients < MAX_USERS-1){
				std::cout << "CONNECTED - watek obslugi polaczen" << std::endl;
				ConnectedClients++;
				parametry->vehActive[ConnectedClients] =  true;


				ThreadParam * a = new ThreadParam;				//przygotowanie parametru dla funkcji watkow
				a->MyVehicle = parametry->vtab;
				a->clietntNumber = ConnectedClients;
				a->Connect = Connect;
				a->vehActive = parametry->vehActive;
				a->keysPressedClientList = parametry->keysPressedClientList;

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
	a.keysPressedClientList = &this->keysPressedClientList;
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
				appWindow->close();/*
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
			}*/
		};

		//-- PRZESUWANIE POJAZDOW 
		{
			for (std::list<EventMassage>::iterator iter = keysPressedClientList.begin(); iter != keysPressedClientList.end(); iter++)
			{
				if (iter->keysPressed.key.code == sf::Keyboard::W)
					vehicleTab[iter->ID]->buttonAction(direction::up, delta);
				if (iter->keysPressed.key.code == sf::Keyboard::A)
					vehicleTab[iter->ID]->buttonAction(direction::left, delta);
				if (iter->keysPressed.key.code == sf::Keyboard::S)
					vehicleTab[iter->ID]->buttonAction(direction::down, delta);
				if (iter->keysPressed.key.code == sf::Keyboard::D)
					vehicleTab[iter->ID]->buttonAction(direction::right, delta);
				if (iter->keysPressed.key.code == sf::Keyboard::Up)
					vehicleTab[iter->ID]->buttonAction(direction::up, delta);
				if (iter->keysPressed.key.code == sf::Keyboard::Left)
					vehicleTab[iter->ID]->buttonAction(direction::left, delta);
				if (iter->keysPressed.key.code == sf::Keyboard::Down)
					vehicleTab[iter->ID]->buttonAction(direction::down, delta);
				if (iter->keysPressed.key.code == sf::Keyboard::Right)
					vehicleTab[iter->ID]->buttonAction(direction::right, delta);

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


