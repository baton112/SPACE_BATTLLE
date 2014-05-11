#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <string>
#define M_PI        3.14159265358979323846
#include <cmath>


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
	 
	const float velocity = 100; // odleglosc przebyta na sekunde
	const float truningSpeed = 100; // ilosc stopini na sekunde
	void move(float defta, bool forward);
	void changeAngle(float delta, bool right); 
public:
	vehicle();
	~vehicle();
	sf::Vector2 <double> position; ///pozycja 0,0 -- lewy gorny rog
	void drowVehicle(sf::RenderWindow *appWindow);
	void buttonAction(direction d, float delta);

	double angle; /// kat 0 - pojazd patrzy pozniomo w prawo /// kat -90 ustawiony w konstruktorze
};

