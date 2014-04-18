#include "vehicle.h"


void vehicle::drowVehicle(sf::RenderWindow *appWindow)
{
	sf::Sprite square;
	square.setTexture(bitMap);
	square.setTextureRect(sf::IntRect(0, 0, VEHICLE_SIZE_X, VEHICLE_SIZE_Y));
	square.setOrigin(VEHICLE_SIZE_X / 2, VEHICLE_SIZE_Y / 2);
	square.rotate(angle);
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
		//position.x--;
		angle--;
		break;
	case right:
		//position.x++;
		angle++;
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
	angle = 0;
}


vehicle::~vehicle()
{
}
