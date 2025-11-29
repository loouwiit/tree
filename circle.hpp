#pragma once

#include <random>

#include "point.hpp"
#include "camara.hpp"
#include "vector.hpp"

class Circle : public sf::Drawable
{
public:
	Circle() = default;
	Circle(unsigned count, float radius, float radiusSD, float hight, float hightSD, float palstance, float palstanceSD, sf::Color color) : pointCount{ count }, vertex(sf::Triangles, 4 * 3 * count)
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
		}

		auto vertexCount = 4 * 3 * count;
		for (unsigned i = 0; i < vertexCount; i++)
			vertex[i].color = color;
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
			auto zFactor = -1 / position.z;
			setVertexs(i, { position.x * zFactor, position.y * zFactor }, std::min(std::max(0.05f, 4.0f * zFactor), 0.4f), zFactor > 0 ? 0xFF : 0x00);
		}
		return *this;
	}

private:
	unsigned pointCount = 0;
	Point* point = nullptr;

	sf::VertexArray vertex{};

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(vertex, states);
	}

	void setVertexs(unsigned index, sf::Vector2f position, float size, sf::Uint8 alpha)
	{
		index *= 4 * 3; // offset to it

		constexpr float DefaultSize = 0.02f;

		const sf::Vector2f hexagon[6] = {
			{std::cos(M_PIf / 3 * 0) * size * DefaultSize, std::sin(M_PIf / 3 * 0) * size * DefaultSize},
			{std::cos(M_PIf / 3 * 1) * size * DefaultSize, std::sin(M_PIf / 3 * 1) * size * DefaultSize},
			{std::cos(M_PIf / 3 * 2) * size * DefaultSize, std::sin(M_PIf / 3 * 2) * size * DefaultSize},
			{std::cos(M_PIf / 3 * 3) * size * DefaultSize, std::sin(M_PIf / 3 * 3) * size * DefaultSize},
			{std::cos(M_PIf / 3 * 4) * size * DefaultSize, std::sin(M_PIf / 3 * 4) * size * DefaultSize},
			{std::cos(M_PIf / 3 * 5) * size * DefaultSize, std::sin(M_PIf / 3 * 5) * size * DefaultSize},
		};

		vertex[index + 0 * 3 + 0].position = position + hexagon[0];
		vertex[index + 0 * 3 + 1].position = position + hexagon[1];
		vertex[index + 0 * 3 + 2].position = position + hexagon[5];
		vertex[index + 0 * 3 + 0].color.a = alpha;
		vertex[index + 0 * 3 + 1].color.a = alpha;
		vertex[index + 0 * 3 + 2].color.a = alpha;

		vertex[index + 1 * 3 + 0].position = position + hexagon[1];
		vertex[index + 1 * 3 + 1].position = position + hexagon[2];
		vertex[index + 1 * 3 + 2].position = position + hexagon[5];
		vertex[index + 1 * 3 + 0].color.a = alpha;
		vertex[index + 1 * 3 + 1].color.a = alpha;
		vertex[index + 1 * 3 + 2].color.a = alpha;

		vertex[index + 2 * 3 + 0].position = position + hexagon[2];
		vertex[index + 2 * 3 + 1].position = position + hexagon[4];
		vertex[index + 2 * 3 + 2].position = position + hexagon[5];
		vertex[index + 2 * 3 + 0].color.a = alpha;
		vertex[index + 2 * 3 + 1].color.a = alpha;
		vertex[index + 2 * 3 + 2].color.a = alpha;

		vertex[index + 3 * 3 + 0].position = position + hexagon[2];
		vertex[index + 3 * 3 + 1].position = position + hexagon[3];
		vertex[index + 3 * 3 + 2].position = position + hexagon[4];
		vertex[index + 3 * 3 + 0].color.a = alpha;
		vertex[index + 3 * 3 + 1].color.a = alpha;
		vertex[index + 3 * 3 + 2].color.a = alpha;
	}
};
