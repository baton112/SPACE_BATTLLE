#include "game.h"

//odbieranie pozycji innych pojazdow 
DWORD WINAPI ThreadFunctionRecive(LPVOID lpParam)	
{
	HANDLE hStdout;										//uchwyt
	ThreadParam *functionParams;						//struktura dla pojazdu

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);			//uchwyt na standardowe wyjscie
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	functionParams = (ThreadParam*)lpParam;				//rzutowanie parametru na strukture


	//najpierw odbieramy ID (tylko INT) i odpowiednio inicjalizujemy
	char * buf = new char[sizeof(int)];
	recv(functionParams->Connect, buf, sizeof(int), 0);
	int *id =  new int; 
	id = (int*)buf;
	functionParams->clietntNumber = *id;
	functionParams->vehActive[functionParams->clietntNumber] = true;
	*functionParams->ID = *id;

	std::cout << "Przypisano ID klienta = " << functionParams->clietntNumber << std::endl;		//wypisanie numery klienta
	std::cout << "Odbieram pozycje samolocikow" << std::endl;
	message  * msg;
	char * buf2 = new char[(sizeof(message))];

	//odbieranie pozycji od servera
	while (recv(functionParams->Connect, buf2, sizeof(message), 0) == sizeof(message)) // odbieanie wiadomosci od serwera
	{
		msg = (message *)buf2;
		if (msg->ID >= 0 && msg->ID <= MAX_USERS-1)
		{
			functionParams->MyVehicle[msg->ID]->position.x = msg->X;					//aktualizacja pozycji
			functionParams->MyVehicle[msg->ID]->position.y = msg->Y;
			functionParams->MyVehicle[msg->ID]->angle = msg->angle;
			functionParams->vehActive[msg->ID] = true;
		}
		else std::cout << "lagi";
	}

	return 0;
}

//obsluga wsztstkich polaczenie 
DWORD WINAPI ThreadHandleConnections(LPVOID lpParam)
{

	WSAData wsaData;
	WORD version = MAKEWORD(2, 2); ///wersja winsock
	int error = WSAStartup(version, &wsaData);//inicjalizacja
	if (error != 0)
	{
		std::cout << "Tell the user that we could not find a usable  Winsock DLL.  " << std::endl;
		system("pause");
		return 0;
	}
	HANDLE handle, handle2;
	SOCKADDR_IN Client;				//punkt koncowy polaczenia (struktura informacyjna)
	Client.sin_addr.s_addr = inet_addr(SERVER_IP);		//	ustawianie paramterow tego gniazdka
	Client.sin_family = AF_INET;							//	
	Client.sin_port = htons(LISTEN_PORT);					//
	int Client_size = sizeof(Client);						//pobranie rozmiaru struktury informacyjnej
	SOCKET Connect = socket(AF_INET, SOCK_STREAM, NULL);	//autentyczna inicjalizacja socketu
	std::cout << "laczenie.. " << std::endl;				//napisz ze zaczynasz
	netowrkThreadParams * param = (netowrkThreadParams*)lpParam;

	//wiadmosc inicjalizacyjna, zeby serwer nadal klientowi ID
	message  msg;
	msg.ID = UNKNOWN;
	msg.X = UNKNOWN;
	msg.Y = UNKNOWN;
	msg.angle = UNKNOWN;

	for (;;)
	{
		if (connect(Connect, (SOCKADDR*)&Client, sizeof(Client)))
		{
			
			int len, bytes_sent;
			char * x = (char*)&msg;
			//len = strlen(msg2);
			bytes_sent = send(Connect, x, sizeof(message), 0);		//wysylanie wiadomosci inicjalizacyjnej
			break;
		}
	}

	ThreadParam * a = new ThreadParam;
	a->clietntNumber = UNKNOWN;
	a->Connect = Connect;
	a->MyVehicle = param->vtab;
	a->vehActive = param->vehActive;
	a->ID = param->ID;
	a->keysPressed = param->keysPressed;
	
	//watek do odbioru
	handle = CreateThread(											
		NULL,                   // default security attributes
		0,                      // use default stack size  
		ThreadFunctionRecive,       // thread function name
		a,          // argument to thread function 
		0,                      // use default creation flags 
		NULL);   // returns the thread identifier

	//watek do wysylania
	handle2 = CreateThread(											
		NULL,                   // default security attributes
		0,                      // use default stack size  
		ThreadSend,       // thread function name
		a,          // argument to thread function 
		0,                      // use default creation flags 
		NULL);   // returns the thread identifier


	std::cout << "Wyslano " << std::endl;

	return 0;
}

//watek wysylania eventow 
DWORD WINAPI ThreadSend(LPVOID lpParam)
{

	HANDLE hStdout;										//uchwyt
	ThreadParam *functionParams;						//struktura dla pojazdu

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);			//uchwyt na standardowe wyjscie
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;
	
	//message  * msg = new message;
	EventMassage *msg = new EventMassage;
	functionParams = (ThreadParam*)lpParam;				//rzutowanie parametru na strukture
	

	//zamienic na wysylanie eventa 
	while (true)
	{
		if (*functionParams->ID != UNKNOWN && !functionParams->keysPressed->empty())
		{
			//-- nowa massage - event ------
			msg->ID = *functionParams->ID;
			msg->keysPressed = *functionParams->keysPressed->begin();
			
			send(functionParams->Connect, (char*)msg, sizeof(EventMassage), 0); 
			std::cout << "Wyslano " << msg->keysPressed.key.code << " ID " << msg->ID << std::endl;

			functionParams->keysPressed->pop_front(); // usuniecie z listy klienta wcisnietych klawiszy

			Sleep(100/60); // magiczny sleep usuwajacy lagi 
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

	//////////tworzenie watku do wywsylania eventow na server 
	netowrkThreadParams a;
	a.vtab = this->vehicleTab; //przeslanie tablicy wskaznikow na pojazdy obslugiwane przez uzytkownikow
	a.vehActive = this->vehiclesActive;
	a.ID = &this->ID;
	a.keysPressed = &(this->keysPressed);
	NetworkindThreadHandle = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		ThreadHandleConnections,       // thread function name
		&a,          // argument to thread function 
		0,                      // use default creation flags 
		NULL);   // returns the thread identifier
	//////////////////////-----------/////////////////////
	std::cout << "przekazano liste " <<a.keysPressed->empty() << std::endl;
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
				/*
				bool isOnList = false;
				if (!keysPressed.empty())
				{
					for (std::list< sf::Event>::iterator iter = keysPressed.begin(); iter != keysPressed.end(); iter++)
					{
						if (iter->key.code == event.key.code)
							isOnList = true;
					}
				}
				if (!isOnList)
				{
					keysPressed.push_front(event);
					std::cout << "dodano event do listy" << std::endl;
					//std::cout << a.keysPressed->empty() << std::endl;
				}*/
				keysPressed.push_front(event);

			}
			if (event.type == sf::Event::KeyReleased)
			{
				//usuwanie z listy klawisza ktory zostal puszczony 
				/*std::list< sf::Event>::iterator i;
				for (std::list< sf::Event>::iterator iter = keysPressed.begin(); iter != keysPressed.end(); iter++)
				{
					if (iter->key.code == event.key.code)
						i = iter;
				}
				//keysPressed.erase(i);
				*/
				keysPressed.push_front(event);
			}
		};

		
		/*
		if (vehicleTab != NULL && vehicleTab[0] != NULL)
		{
			// obsluga klawiszy znajdujacych sie na liscie keyPressed
			for (std::list< sf::Event>::iterator iter = keysPressed.begin(); iter != keysPressed.end(); iter++)
			{
				if (iter->key.code == sf::Keyboard::W)
					vehicleTab[ID]->buttonAction(direction::up, delta);
				if (iter->key.code == sf::Keyboard::A)
					vehicleTab[ID]->buttonAction(direction::left, delta);
				if (iter->key.code == sf::Keyboard::S)
					vehicleTab[ID]->buttonAction(direction::down, delta);
				if (iter->key.code == sf::Keyboard::D)
					vehicleTab[ID]->buttonAction(direction::right, delta);
				if (iter->key.code == sf::Keyboard::Up)
					vehicleTab[ID]->buttonAction(direction::up, delta);
				if (iter->key.code == sf::Keyboard::Left)
					vehicleTab[ID]->buttonAction(direction::left, delta);
				if (iter->key.code == sf::Keyboard::Down)
					vehicleTab[ID]->buttonAction(direction::down, delta);
				if (iter->key.code == sf::Keyboard::Right)
					vehicleTab[ID]->buttonAction(direction::right, delta);
			}
		}*/


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
		vehicleTab[i] = new vehicle;
		vehiclesActive[i] = false;
		this->ID = UNKNOWN;
	}
}

game::~game()
{
}


