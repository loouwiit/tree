#pragma once

#include <cmath>

#include <SFML/System.hpp>

#include "vector.hpp"

class Rotator
{
public:
	auto getPosition() const
	{
		return position;
	}
	auto getAxis() const
	{
		return rotateAxis;
	}
	auto getTheta() const
	{
		return theta;
	}
	void setPosition(sf::Vector3f position)
	{
		Rotator::position = position;
	}
	void setAxis(sf::Vector3f axis)
	{
		Rotator::rotateAxis = axis / abs(axis);
	}
	void setTheta(float theta)
	{
		Rotator::theta = theta;
		float cos = std::cos(theta);
		float subCos = 1 - cos;
		float sin = std::sin(theta);

		auto [x, y, z] = rotateAxis;
		matrix[0].x = cos + (x * x) * subCos;
		matrix[0].y = x * y * subCos - z * sin;
		matrix[0].z = x * z * subCos + y * sin;

		matrix[1].x = y * x * subCos + z * sin;
		matrix[1].y = cos + (y * y) * subCos;
		matrix[1].z = y * z * subCos + x * sin;

		matrix[2].x = z * x * subCos - y * sin;
		matrix[2].y = z * y * subCos + x * sin;
		matrix[2].z = cos + (z * z) * subCos;
	}

	auto operator()(sf::Vector3f point) const
	{
		point += position;
		sf::Vector3f ret{};
		ret.x = matrix[0] * point;
		ret.y = matrix[1] * point;
		ret.z = matrix[2] * point;
		return ret;
	}

private:
	sf::Vector3f position{};
	sf::Vector3f rotateAxis{ 0,0,1 };

	sf::Vector3f matrix[3]{ {1,0,0},{0,1,0},{0,0,1} };

	float theta = 0.0f;

	friend class Camara;
};
