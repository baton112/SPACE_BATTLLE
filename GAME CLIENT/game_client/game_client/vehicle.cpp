#include "vehicle.h"


void vehicle::drowVehicle(sf::RenderWindow *appWindow)
{
	sf::Sprite square;
	square.setTexture(bitMap);
	square.setTextureRect(sf::IntRect(0, 0, VEHICLE_SIZE_X, VEHICLE_SIZE_Y));
	square.setPosition(position.x,position.y);
	appWindow->draw(square);
}

void vehicle::buttonAction(direction d)
{
	switch (d)
	{
	case up:
		position.y--;
		break;
	case down:
		position.y++;
		break;
	case left:
		position.x--;
		break;
	case right:
		position.x++;
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
	position.x = 0;
	position.y = 0;
}


vehicle::~vehicle()
{
}
