#include <SFML/System.hpp>
#include "SFML/Graphics.hpp"
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "Procedural-Audio");
	window.setFramerateLimit(144);

	sf::CircleShape fire;
	fire.setRadius(50.f);
	fire.setFillColor(sf::Color::Yellow);
	fire.setPosition({ 200 - fire.getRadius(), 200 - fire.getRadius() });

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		window.clear();
		window.draw(fire);
		window.display();
	}

	return 0;
}