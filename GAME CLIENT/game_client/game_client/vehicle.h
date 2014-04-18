#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <string>

#define VEHICLE_SIZE_X 80
#define VEHICLE_SIZE_Y 80

#define VEHICLE_IMAGE_PATH "IMAGES/vehicle.png"

enum direction
{
	up,
	down,
	left,
	right, 
	none
};

class vehicle
{
private: 
	sf::Texture bitMap;
	sf::Vector2 <double> position; ///pozycja 0,0 -- lewy gorny rog
	int angle;
public:
	vehicle();
	~vehicle();
	void drowVehicle(sf::RenderWindow *appWindow);
	void buttonAction(direction d);

};

