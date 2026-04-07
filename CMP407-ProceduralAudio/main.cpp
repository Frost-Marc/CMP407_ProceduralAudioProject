#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/System.hpp>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "FireAudio.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "Procedural-Audio");
	window.setFramerateLimit(144);
	ImGui::SFML::Init(window);

	sf::CircleShape fireObject;
	fireObject.setRadius(50.f);
	fireObject.setFillColor(sf::Color::Yellow);
	fireObject.setPosition({ 200 - fireObject.getRadius(), 200 - fireObject.getRadius() });

	FireAudio fireAudio;
	fireAudio.play();

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

		float intensity = fireAudio.intensity.load();
		if (ImGui::SliderFloat("Base Intensity", &intensity, 0.0f, 1.0f))
		{
			fireAudio.intensity.store(intensity);
		}

		float crackle = fireAudio.crackle.load();
		if (ImGui::SliderFloat("Crackle", &crackle, 0.0f, 1.0f))
		{
			fireAudio.crackle.store(crackle);
		}

		float hiss = fireAudio.hiss.load();
		if (ImGui::SliderFloat("Hiss", &hiss, 0.0f, 1.0f))
		{
			fireAudio.hiss.store(hiss);
		}

		ImGui::End();

		window.clear();
		window.draw(fireObject);
		ImGui::SFML::Render(window);
		window.display();
	}

	fireAudio.stop();
	ImGui::SFML::Shutdown();
	return 0;
}