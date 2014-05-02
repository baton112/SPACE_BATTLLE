#include "game.h"

void game::runGameLoop(sf::RenderWindow *appWindow)
{
	std::chrono::high_resolution_clock::time_point time;
	time = std::chrono::high_resolution_clock::now();

	while (appWindow->isOpen())
	{
		std::chrono::high_resolution_clock::duration time_enlapsed = std::chrono::high_resolution_clock::now() - time;
		float delta = time_enlapsed.count() * 0.00001; // milisekundy //// 0.0000001 - dla sekund 
		time = std::chrono::high_resolution_clock::now();
		std::cout << delta << std::endl; 
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
				for (std::list< sf::Event>::iterator iter = keysPressed.begin(); iter != keysPressed.end();iter++)
				{
					if (iter->key.code == event.key.code)
						i = iter;
				}
				keysPressed.erase(i);
			}
			
		};

		// obsluga klawiszy znajdujacych sie na liscie keyPressed 
		for (std::list< sf::Event>::iterator iter = keysPressed.begin(); iter != keysPressed.end(); iter++)
		{
			if (iter->key.code == sf::Keyboard::W)
				tmpVehicle.buttonAction(direction::up, delta);
			if (iter->key.code == sf::Keyboard::A)
				tmpVehicle.buttonAction(direction::left, delta);
			if (iter->key.code == sf::Keyboard::S)
				tmpVehicle.buttonAction(direction::down, delta);
			if (iter->key.code == sf::Keyboard::D)
				tmpVehicle.buttonAction(direction::right, delta);
			if (iter->key.code == sf::Keyboard::Up)
				tmpVehicle.buttonAction(direction::up, delta);
			if (iter->key.code == sf::Keyboard::Left)
				tmpVehicle.buttonAction(direction::left, delta);
			if (iter->key.code == sf::Keyboard::Down)
				tmpVehicle.buttonAction(direction::down, delta);
			if (iter->key.code == sf::Keyboard::Right)
				tmpVehicle.buttonAction(direction::right, delta);

		}
		//czyszczenie okna 
		appWindow->clear();


		// rysowanie 
		tmpVehicle.drowVehicle(appWindow);

		//wyswietlenie okna
		appWindow->display();
	}

}


game::game()
{
}


game::~game()
{
}


