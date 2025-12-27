#include <SFML/Graphics.hpp>

#include "point.hpp"
#include "circle.hpp"
#include "camara.hpp"

#include <fstream>
#include <thread>
#include <unistd.h>

int main();
int importConfiguration();
void movement(float deltaTime);
void key(sf::Event::KeyEvent key, bool press);
void mouse(sf::Event::MouseMoveEvent mouse);

sf::RenderWindow window(sf::VideoMode(512, 256), "tree");

std::vector<Circle> circle{};
Camara camara{};

float moveSensitivity = 40.0f;
float camaraSensitivity = 1.0f;

sf::Vector2f mousePosition{};

constexpr auto ThreadCount = 16;
std::thread coThreads[ThreadCount]{};
Circle* coThreadDeal[ThreadCount]{};
bool running = true;
float deltaTime = 0;

int main()
{
	srand(time(nullptr));
	camara.setPosition({ 0.0f, 30.0f, 100.0f });
	camara.setPitch(0.1f);

	importConfiguration();
	for (auto i = 0; i < ThreadCount; i++)
	{
		void coThreadMain(int id);
		coThreads[i] = std::thread{ coThreadMain, i };
	}

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
			case Event::MouseButtonPressed:
			{
				mousePosition = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
				break;
			}
			default:
				break;
			}
		}

		deltaTime = clock.restart().asSeconds();
		movement(deltaTime);
		{
			auto id = 0;
			for (auto& i : circle)
			{
				auto count = ThreadCount;
				while (coThreadDeal[id] != nullptr)
				{
					id++;
					id = id % ThreadCount;
					if (--count == 0)
					{
						count = ThreadCount;
						usleep(10);
					}
				}
				coThreadDeal[id] = &i;
				id++;
				id = id % ThreadCount;
			}
		}

		window.clear();
		for (auto& i : circle)
			window.draw(i);
		window.display();
	}

	running = false;
	for (auto i = 0; i < ThreadCount; i++)
	{
		coThreadDeal[i] = (Circle*)-1;
	}
	for (auto& i : coThreads)
		i.join();

	return 0;
}

void coThreadMain(int id)
{
	Circle* circle = nullptr;
	while (running)
	{
		while ((circle = coThreadDeal[id]) == nullptr) usleep(1000);
		if (!running) return;
		circle->update(deltaTime);
		circle->render(camara);
		coThreadDeal[id] = nullptr;
		if (!running) return;
	}
}

int importConfiguration()
{
	std::ifstream file{ "configuration.txt" };
	if (!file.is_open()) return 0;
	circle.clear();
	while (file.peek() != EOF)
	{
		if (file.peek() == '#')
		{
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			while (std::isspace(file.peek()))
				file.ignore();
			continue;
		}

		unsigned count;
		float radius;
		float radiusSD;
		float hight;
		float hightSD;
		float palstance;
		float palstanceSD;
		sf::Uint32 color;
		file >> std::dec >> count >> radius >> radiusSD >> hight >> hightSD >> palstance >> palstanceSD >> std::hex >> color;
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (count > 0) circle.push_back({ count, radius, radiusSD, hight, hightSD, palstance , palstanceSD, sf::Color{color} });

		while (std::isspace(file.peek()))
			file.ignore();
	}

	return circle.size();
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
