#pragma once

#include <random>

#include "point.hpp"
#include "camara.hpp"
#include "vector.hpp"

class Circle : public sf::Drawable, public sf::Transformable
{
public:
	Circle() = default;
	Circle(unsigned count, float radius, float radiusSD, float hight, float hightSD, float palstance, float palstanceSD, sf::Color color) : pointCount{ count }, vertex(sf::Points, count)
	{
		delete[]point;
		point = new Point[count];

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> angelDistribution(-M_PI, M_PI);
		std::normal_distribution<float> radiusDistribution(radius, radiusSD);
		std::normal_distribution<float> hightDistribution(hight, hightSD);
		std::normal_distribution<float> palstanceDistribution(palstance, palstanceSD);

		for (unsigned i = 0; i < count; i++)
		{
			point[i].angel = angelDistribution(gen);
			point[i].radius = radiusDistribution(gen);
			point[i].hight = hightDistribution(gen);
			point[i].palstance = palstanceDistribution(gen);

			vertex[i].color = color;
		}
	}

	auto& update(float deltaTime)
	{
		for (unsigned i = 0; i < pointCount; i++)
		{
			point[i].update(deltaTime);
		}
		return *this;
	}

	auto& render(const Camara& camara)
	{
		for (unsigned i = 0; i < pointCount; i++)
		{
			auto position = camara(point[i].getPosition());
			vertex[i].position.x = position.x / -position.z;
			vertex[i].position.y = position.y / -position.z;
			vertex[i].color.a = position.z < 0 ? 0xFF : 0;
		}
		return *this;
	}

private:
	unsigned pointCount = 0;
	Point* point = nullptr;

	sf::VertexArray vertex{};

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// our particles don't use a texture
		states.texture = NULL;

		// draw the vertex array
		target.draw(vertex, states);
	}
};
