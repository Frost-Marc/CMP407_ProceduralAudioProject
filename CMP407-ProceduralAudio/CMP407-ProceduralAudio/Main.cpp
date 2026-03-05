#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1000, 800 }), "ProceduralAudio");

	sf::CircleShape Fire;
	Fire.setRadius(50.f);
	Fire.setFillColor(sf::Color::Yellow);
	Fire.setPosition({ 200,200 });

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		window.clear();
		window.draw(Fire);
		window.display();
	}

	return 0;
}