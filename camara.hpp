#pragma once

#include "rotator.hpp"

class Camara
{
public:
	auto getPosition() const
	{
		// return externalRotator.position;
		return -innerRotator.position;
	}
	void setPosition(sf::Vector3f position)
	{
		// externalRotator.position = position;
		innerRotator.position = -position;
	}

	auto getPitch() const
	{
		return pitch;
	}
	void setPitch(float pitch)
	{
		if (pitch > +M_PI_2) pitch = +M_PI_2;
		if (pitch < -M_PI_2) pitch = -M_PI_2;
		Camara::pitch = pitch;
		computeMatrix();
	}

	auto getYaw() const
	{
		return yaw;
	}
	void setYaw(float yaw)
	{
		if (yaw > +M_PI) yaw -= (((int)(yaw / M_PI) + 1) / 2) * 2 * M_PI;
		if (yaw < -M_PI) yaw -= (((int)(yaw / M_PI) + 1) / 2) * 2 * M_PI;
		Camara::yaw = yaw;
		computeMatrix();
	}

	sf::Vector3f getFront() const
	{
		return -innerRotator.matrix[2];
	}

	sf::Vector3f getLeft() const
	{
		return -innerRotator.matrix[0];
	}

	sf::Vector3f getUp() const
	{
		return innerRotator.matrix[1];
	}

	auto operator()(sf::Vector3f point) const
	{
		return innerRotator(point);
	}

	operator const Rotator() const
	{
		return innerRotator;
	}

	// const auto getInnerRotator() const
	// {
	// 	return innerRotator;
	// }

	// const auto getExternalRotator() const
	// {
	// 	return externalRotator;
	// }

private:
	float pitch = 0.0f;
	float yaw = 0.0f;
	Rotator innerRotator{};
	// Rotator externalRotator{};

	void computeMatrix()
	{
		auto sinY = std::sin(yaw);
		auto cosY = std::cos(yaw);
		auto sinP = std::sin(pitch);
		auto cosP = std::cos(pitch);

		auto& innerMatrix = innerRotator.matrix;
		// auto& externalMatrix = externalRotator.matrix;

		innerMatrix[0] = { cosY, 0, -sinY };
		innerMatrix[1] = { -sinY * sinP, cosP, -cosY * sinP };
		innerMatrix[2] = { sinY * cosP, sinP, cosY * cosP };
		// 正交矩阵求逆 -> 转置
		// externalMatrix[0] = { innerMatrix[0].x , innerMatrix[1].x, innerMatrix[2].x };
		// externalMatrix[1] = { innerMatrix[0].y , innerMatrix[1].y, innerMatrix[2].y };
		// externalMatrix[2] = { innerMatrix[0].z , innerMatrix[1].z, innerMatrix[2].z };
	}
};
