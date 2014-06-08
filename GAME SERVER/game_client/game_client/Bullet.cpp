#include "Bullet.h"


Bullet::Bullet(sf::Vector2 <double> position, int angle)
{
	this->angle = angle;
	this->position = position;
}


Bullet::~Bullet()
{
}

void Bullet::drow(sf::RenderWindow *appWindow)
{
	sf::CircleShape circle(BULLET_RADIUS); // tworzymy ko³o
	circle.setPosition(this->position.x, this->position.y); // a teraz ustawiamy jego pozycjê
	circle.setFillColor(sf::Color::Red);
	appWindow->draw(circle);
}

void Bullet::move(float delta)
{
		position.y += BULLET_VELOCITY*delta / 100 * sin(angle*M_PI / 180);
		position.x += BULLET_VELOCITY*delta / 100 * cos(angle*M_PI / 180);
}