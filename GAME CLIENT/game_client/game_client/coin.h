#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#define COIN_RADIUS 10

class coin
{
public:
	sf::Vector2 <double> position; //x, y -- srodek kola 
	void drowCoin(sf::RenderWindow *appWindow);
	bool display;

	coin(int x, int y);
	~coin();
};

