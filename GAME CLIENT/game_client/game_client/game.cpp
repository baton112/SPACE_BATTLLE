#include "game.h"

void game::runGameLoop(sf::RenderWindow *appWindow)
{
	while (appWindow->isOpen())
	{
		sf::Event event;
		while (appWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				appWindow->close();
			if (event.type == sf::Event::KeyPressed)
			{
				// trzymanie klawisza dodaje go do listy klawiszy wcisnietych 
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
				tmpVehicle.buttonAction(direction::up);
			if (iter->key.code == sf::Keyboard::A)
				tmpVehicle.buttonAction(direction::left);
			if (iter->key.code == sf::Keyboard::S)
				tmpVehicle.buttonAction(direction::down);
			if (iter->key.code == sf::Keyboard::D)
				tmpVehicle.buttonAction(direction::right);

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


