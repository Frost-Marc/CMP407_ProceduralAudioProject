#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/System.hpp>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "Camera.h"
#include "FireAudio.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1200, 600 }), "Procedural-Audio");
	window.setFramerateLimit(144);
	ImGui::SFML::Init(window);

	sf::Vector2f fireCenter = { 600.f, 300.f };
	float fireRadius = 50.f;

	sf::CircleShape fireObject(fireRadius);
	fireObject.setOrigin({ fireRadius, fireRadius });
	fireObject.setFillColor(sf::Color::Yellow);
	fireObject.setPosition({ fireCenter });

	FireAudio fireAudio;
	fireAudio.setRelativeToListener(false);
	fireAudio.setPosition({ fireCenter.x, fireCenter.y, 0.0f });
	fireAudio.setMinDistance(150.f);
	fireAudio.setAttenuation(5.0f);
	fireAudio.play();

	// Visual representation of the "Full Volume" radius
	sf::CircleShape distanceVisual;
	distanceVisual.setRadius(fireAudio.getMinDistance());
	distanceVisual.setOrigin({ fireAudio.getMinDistance(), fireAudio.getMinDistance() });
	distanceVisual.setPosition({ fireCenter });
	distanceVisual.setFillColor(sf::Color(255, 0, 0, 30));
	distanceVisual.setOutlineColor(sf::Color::Red);
	distanceVisual.setOutlineThickness(2.f);

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

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Listener::setPosition({ (float)mousePos.x, (float)mousePos.y, 20.f });
		sf::Listener::setDirection({ 0.f, 0.f, -1.f });

		ImGui::SFML::Update(window, deltaClock.restart());

		// --- ImGui Interface ---
		ImGui::Begin("Fire Sound Settings");
		ImGui::Text("Adjust the layers to change the fire personality.");
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Wood & Crackle", ImGuiTreeNodeFlags_DefaultOpen)) 
		{
			ImGui::SliderFloat("Crackle Probability", &fireAudio.crackleProbability, 0.0f, 0.001f, "%.5f");
			ImGui::SliderFloat("Crackle Filter", &fireAudio.crackleFilterFreq, 2000.0f, 8000.0f);
		}

		if (ImGui::CollapsingHeader("Gas & Hiss", ImGuiTreeNodeFlags_DefaultOpen)) 
		{
			ImGui::SliderFloat("Hiss Volume", &fireAudio.hissGain, 0.0f, 0.2f);
			ImGui::SliderFloat("Hiss Filter", &fireAudio.hissFilterFreq, 5000.0f, 12000.0f);
		}

		if (ImGui::CollapsingHeader("Flame & Roar", ImGuiTreeNodeFlags_DefaultOpen)) 
		{
			ImGui::SliderFloat("Roar Volume", &fireAudio.roarGain, 0.0f, 1.0f);
			ImGui::SliderFloat("Roar Filter", &fireAudio.roarFilterFreq, 10.0f, 500.0f);
			ImGui::SliderFloat("Lapping Speed", &fireAudio.lappingSpeed, 0.1f, 5.0f);
		}

		if (ImGui::CollapsingHeader("Attenuation", ImGuiTreeNodeFlags_DefaultOpen)) 
		{
			float currentAtten = fireAudio.getAttenuation();
			if (ImGui::SliderFloat("Attenuation", &currentAtten, 0.0f, 10.0f))
			{
				fireAudio.setAttenuation(currentAtten);
			}

			float currentMinDist = fireAudio.getMinDistance();
			if (ImGui::SliderFloat("Min Distance", &currentMinDist, 0.1f, 20.0f)) {
				fireAudio.setMinDistance(currentMinDist);
			}
		}

		ImGui::Separator();
		if (ImGui::Button("Reset to Default")) {
			fireAudio.crackleProbability = 0.00015f;
			fireAudio.crackleFilterFreq = 3500.0f;
			fireAudio.hissGain = 0.04f;
			fireAudio.hissFilterFreq = 8000.0f;
			fireAudio.roarGain = 0.35f;
			fireAudio.roarFilterFreq = 100.0f;
			fireAudio.lappingSpeed = 0.5f;
		}

		ImGui::End();

		window.clear(sf::Color(10, 10, 10));
		window.draw(distanceVisual);
		window.draw(fireObject);
		ImGui::SFML::Render(window);
		window.display();
	}

	fireAudio.stop();
	ImGui::SFML::Shutdown();
	return 0;
}