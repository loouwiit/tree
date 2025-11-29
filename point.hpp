#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>

class Point
{
public:
	float angel = 0.0f;
	float radius = 0.0f;
	float& hight = position.y;

	float palstance = 0.0f; // 角频率

	auto& update(float deltaTime)
	{
		angel += palstance * deltaTime;
		if (angel > +M_PI) angel -= 2 * M_PI;
		if (angel < -M_PI) angel += 2 * M_PI;
		position.z = radius * std::cos(angel);
		position.x = radius * std::sin(angel);
		return *this;
	}

	auto getPosition()
	{
		return position;
	}

private:
	sf::Vector3f position{};
};
