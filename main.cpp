#include <SFML/Graphics.hpp>

#include "point.hpp"
#include "circle.hpp"
#include "camara.hpp"

#include <iostream>

int main();
void movement(float deltaTime);
void key(sf::Event::KeyEvent key, bool press);
void mouse(sf::Event::MouseMoveEvent mouse);

sf::RenderWindow window(sf::VideoMode(512, 256), "tree");

Circle circle{ 10000, 10, 1, 10, 1, 2 * M_PI / 30 , 0 };
Camara camara{};

float moveSensitivity = 40.0f;
float camaraSensitivity = 1.0f;

sf::Vector2f mousePosition{};

int main()
{
	srand(time(nullptr));
	camara.setPosition({ 0, 0, 100 });

	sf::Clock clock{};

	window.setView(sf::View{ sf::Vector2f{ 0, 0 }, {(float)window.getSize().x / (float)window.getSize().y, -1.0f} });
	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			using Event = sf::Event;
			switch (event.type)
			{
			case Event::Closed:
			{
				window.close();
				break;
			}
			case Event::Resized:
			{
				window.setView(sf::View{ sf::Vector2f{ 0, 0 }, { (float)event.size.width / (float)event.size.height, -1.0f } });
				break;
			}
			case Event::KeyPressed:
			{
				key(event.key, true);
				break;
			}
			case Event::KeyReleased:
			{
				key(event.key, false);
				break;
			}
			case Event::MouseMoved:
			{
				mouse(event.mouseMove);
				break;
			}
			default:
				break;
			}
		}

		sf::Time elapsed = clock.restart();
		movement(elapsed.asSeconds());
		circle.update(elapsed.asSeconds());
		circle.render(camara);

		window.clear();
		window.draw(circle);
		window.display();
	}

	return 0;
}

void movement(float deltaTime)
{
	using Key = sf::Keyboard;
	if (Key::isKeyPressed(Key::W))
	{
		auto position = camara.getPosition();
		position += camara.getFront() * moveSensitivity * deltaTime;
		camara.setPosition(position);
	}
	if (Key::isKeyPressed(Key::S))
	{
		auto position = camara.getPosition();
		position -= camara.getFront() * moveSensitivity * deltaTime;
		camara.setPosition(position);
	}
	if (Key::isKeyPressed(Key::A))
	{
		auto position = camara.getPosition();
		position += camara.getLeft() * moveSensitivity * deltaTime;
		camara.setPosition(position);
	}
	if (Key::isKeyPressed(Key::D))
	{
		auto position = camara.getPosition();
		position -= camara.getLeft() * moveSensitivity * deltaTime;
		camara.setPosition(position);
	}
	if (Key::isKeyPressed(Key::R))
	{
		auto position = camara.getPosition();
		position += camara.getUp() * moveSensitivity * deltaTime;
		camara.setPosition(position);
	}
	if (Key::isKeyPressed(Key::F))
	{
		auto position = camara.getPosition();
		position -= camara.getUp() * moveSensitivity * deltaTime;
		camara.setPosition(position);
	}
	if (Key::isKeyPressed(Key::Space))
	{
		auto position = camara.getPosition();
		position.y += moveSensitivity * deltaTime;
		camara.setPosition(position);
	}
	if (Key::isKeyPressed(Key::LShift))
	{
		auto position = camara.getPosition();
		position.y -= moveSensitivity * deltaTime;
		camara.setPosition(position);
	}
}

void key(sf::Event::KeyEvent key, bool press)
{
	using Key = sf::Keyboard;
	switch (key.code)
	{
	case Key::Escape:
	{
		window.close();
		break;
	}

	default:
		break;
	}
}

void mouse(sf::Event::MouseMoveEvent mouse)
{
	auto mouseNew = window.mapPixelToCoords({ mouse.x, mouse.y });
	auto mouseMove = mouseNew - mousePosition;
	mousePosition = mouseNew;

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		return;

	auto yaw = camara.getYaw();
	auto pitch = camara.getPitch();

	yaw += mouseMove.x * camaraSensitivity;
	pitch += mouseMove.y * camaraSensitivity;

	camara.setYaw(yaw);
	camara.setPitch(pitch);
}
