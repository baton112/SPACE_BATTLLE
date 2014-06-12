#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <string>
#define M_PI        3.14159265358979323846
#include <cmath>
#include "coin.h"


#define VEHICLE_SIZE_X 80
#define VEHICLE_SIZE_Y 80

#define VEHICLE_RADIAS 40

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
	 
	float velocity ; // odleglosc przebyta na sekunde
	float truningSpeed ; // ilosc stopini na sekunde
	void move(float defta, bool forward);
	void changeAngle(float delta, bool right); 
public:
	vehicle();
	~vehicle();
	sf::Vector2 <double> position; ///pozycja 0,0 -- lewy gorny rog
	void drowVehicle(sf::RenderWindow *appWindow);
	void buttonAction(direction d, float delta);
	bool checkCoin(coin *c);
	void drowVehicleCircle(sf::RenderWindow *appWindow);

	double angle; /// kat 0 - pojazd patrzy pozniomo w prawo /// kat -90 ustawiony w konstruktorze
};

