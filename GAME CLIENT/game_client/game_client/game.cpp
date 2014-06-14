#include "game.h"


struct ThreadParam{
	vehicle **MyVehicle;
	SOCKET Connect;
	coin * c[COINS_NUMBER];
	int clietntNumber;
	bool *vehActive;
	int * ID;
	int *score;
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
	int * ID;
	coin * c[COINS_NUMBER];
	int *score;
};

//odbieranie pozycji innych pojazdow 
DWORD WINAPI ThreadFunctionRecive(LPVOID lpParam)	
{
	HANDLE hStdout;										//uchwyt
	ThreadParam *functionParams;						//struktura dla pojazdu

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);			//uchwyt na standardowe wyjscie
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	functionParams = (ThreadParam*)lpParam;				//rzutowanie parametru na strukture

	std::cout << "Connected " << functionParams->clietntNumber << std::endl;		//wypisanie numery klienta


	//najpierw odbieramy ID (tylko INT) i odpowiednio inicjalizujemy
			char * buf = new char[sizeof(int)];
	recv(functionParams->Connect, buf, sizeof(int), 0);
			int *id =  new int; 
			id = (int*)buf;
			functionParams->clietntNumber = *id;
			functionParams->vehActive[functionParams->clietntNumber] = true;
			*functionParams->ID = *id;
	//std::cout <<"id= " <<*id << std::endl;;



	message  * msg;
	char * buf2 = new char[(sizeof(message))];


	while (recv(functionParams->Connect, buf2, sizeof(message), 0) == sizeof(message)) // odbieanie wiadomosci od serwera
	{
		msg = (message *)buf2;
		if (msg->ID >= 0 && msg->ID <= 3)
		{
			functionParams->MyVehicle[msg->ID]->position.x = msg->X;					//aktualizacja pozycji
			functionParams->MyVehicle[msg->ID]->position.y = msg->Y;
			(functionParams->MyVehicle[msg->ID]->angle) = (msg->angle);
			functionParams->vehActive[msg->ID] = true;
		}
		else if (msg->ID >= COIN_ID && msg->ID < SCORE_ID)
		{
			functionParams->c[msg->ID-COIN_ID]->position.x = msg->X;
			functionParams->c[msg->ID - COIN_ID]->position.y = msg->Y;
			functionParams->c[msg->ID - COIN_ID]->display = true;
			if (msg->X == UNKNOWN)
				functionParams->c[msg->ID - COIN_ID]->display = false;
		}
		else if (msg->ID >= SCORE_ID && msg->ID < SCORE_ID+MAX_USERS)
		{
			if (msg->X == *functionParams->ID)
			*functionParams->score = msg->Y;
			//std::cout << msg->X << " " << msg->Y << " " << *functionParams->ID << std::endl;
		}
		std::cout <<"ID=" << msg->ID << " X="<< msg->X << " " << msg->Y << " " << *functionParams->ID << std::endl;
//	else
	//	std::cout << "lagi";
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
	a->score = param->score;
	for (int i = 0; i < COINS_NUMBER; i++){
		a->c[i] = param->c[i];
	}
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
	
	message  * msg = new message;
	functionParams = (ThreadParam*)lpParam;				//rzutowanie parametru na strukture
	

	//zamienic na wysylanie eventa 
	while (true)
	{
		if (*functionParams->ID != UNKNOWN){			//wysylanie na serwer swoich pozycji
			msg->angle = functionParams->MyVehicle[*functionParams->ID]->angle;
			msg->ID = *functionParams->ID;
			msg->X = functionParams->MyVehicle[*functionParams->ID]->position.x;
			msg->Y = functionParams->MyVehicle[*functionParams->ID]->position.y;

			send(functionParams->Connect, (char*)msg, sizeof(message), 0);
			Sleep(2*100/60); // magiczny sleep usuwajacy lagi 
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
	for (int i = 0; i < COINS_NUMBER; i++){
		c[i] = new coin(0, 0);
	}
	

	//////////tworzenie watku do wywsylania eventow na server 
	netowrkThreadParams a;
	a.vtab = this->vehicleTab; //przeslanie tablicy wskaznikow na pojazdy obslugiwane przez uzytkownikow
	a.vehActive = this->vehiclesActive;
	a.ID = &this->ID;
	a.score = &this->score;
	for (int i = 0; i < COINS_NUMBER; i++){
		a.c[i] = this->c[i];
	}
	//
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
		
		for (int i = 0; i < COINS_NUMBER; i++){
			if (c[i]->display)
				c[i]->drowCoin(appWindow);
		}
		drowScore(appWindow);


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
	loadFont();
	score = 0;
}

game::~game()
{
}

void game::loadFont()
{
	// Load it from a file
	if (!font.loadFromFile("SCRATCHM.TTF"))
	{
		// error...
		std::cout << "Nie wczytano czcionki" << std::endl;
	}
}


void game::drowScore(sf::RenderWindow *appWindow)
{
	sf::Text text;
	text.setCharacterSize(15);
	text.setPosition(10, 10);
	text.setFont(font);
	text.setColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setPosition(appWindow->getSize().x - 150, 20);
	sf::Text sfText;
	std::ostringstream ss;
	ss << "Player ";
	ss << this->ID;
	ss << " ";
	ss << score;
	text.setString(ss.str());
	appWindow->draw(text);

}
