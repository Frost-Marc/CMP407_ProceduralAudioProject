#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/System.hpp>
#include "SFML/Graphics.hpp"
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "Procedural-Audio");
	window.setFramerateLimit(144);
	ImGui::SFML::Init(window);

	sf::CircleShape fire;
	fire.setRadius(50.f);
	fire.setFillColor(sf::Color::Yellow);
	fire.setPosition({ 200 - fire.getRadius(), 200 - fire.getRadius() });

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		while (const auto event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(window, *event);

			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Fire Settings");
		ImGui::Text("TEXT!");
		ImGui::End();

		window.clear();
		window.draw(fire);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}