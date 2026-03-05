#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "ProceduralAudio");
	window.setFramerateLimit(144);

	sf::CircleShape Fire;
	Fire.setRadius(50.f);
	Fire.setFillColor(sf::Color::Yellow);
	Fire.setPosition({ 200 - Fire.getRadius(), 200 - Fire.getRadius() });

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
		gui();
	}

	return 0;
}

void gui()
{

}