#include "coin.h"


coin::coin(int x, int y)
{
	position.x = x;
	position.y = y;
}


coin::~coin()
{
}

void coin::drowCoin(sf::RenderWindow *appWindow)
{
	sf::CircleShape circle(COIN_RADIUS); // tworzymy kolo
	circle.setPosition(this->position.x-COIN_RADIUS,this->position.y-COIN_RADIUS); // a teraz ustawiamy jego pozycje
	circle.setFillColor(sf::Color::Red);
	appWindow->draw(circle);

}