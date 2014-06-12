#include "vehicle.h"

void vehicle::drowVehicle(sf::RenderWindow *appWindow)
{
	sf::Sprite square;
	square.setTexture(bitMap);
	square.setTextureRect(sf::IntRect(0, 0, VEHICLE_SIZE_X, VEHICLE_SIZE_Y));
	square.setOrigin(VEHICLE_SIZE_X / 2, VEHICLE_SIZE_Y / 2); // obrot i pozycja wgledem srodka obiektu 
	square.rotate(angle+90.0);
	square.setPosition(position.x,position.y);
	appWindow->draw(square);
}

void vehicle::move(float delta, bool forward)
{
	if (forward)
	{
		position.y += velocity*delta/100 * sin(angle*M_PI / 180);
		position.x += velocity*delta/100 * cos(angle*M_PI / 180);
	}
	else
	{
		position.y -= velocity*delta/100 * sin(angle*M_PI / 180);
		position.x -= velocity*delta/100 * cos(angle*M_PI / 180);
	}
}

void vehicle::changeAngle(float delta, bool right)
{
	if (right)
		angle += delta*truningSpeed / 100.0;
	else
		angle -= delta*truningSpeed / 100.0;
}

void vehicle::buttonAction(direction d, float delta)
{
	switch (d)
	{
	case up:
		this->move(delta, true);
		break;
	case down:
		this->move(delta, false);
		break;
	case left:
		this->changeAngle(delta, false);
		break;
	case right:
		this->changeAngle(delta, true);
		break;
	case none:
		break;
	default:
		break;
	}
}

vehicle::vehicle()
{
	bitMap.loadFromFile(VEHICLE_IMAGE_PATH);
	position.x = 50;
	position.y = 50;
	angle = -90;
	velocity = 100;
	truningSpeed = 100;
}

vehicle::~vehicle()
{
}
