#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#define M_PI        3.14159265358979323846

#define BULLET_RADIUS 5
#define BULLET_VELOCITY 100

class Bullet
{
private:
	sf::Vector2 <double> position;
	int angle;
	
public:
	Bullet(sf::Vector2 <double> position, int angle);
	~Bullet();
	void move(float delta);
	void drow(sf::RenderWindow *appWindow);
};

