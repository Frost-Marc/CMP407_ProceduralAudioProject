#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/System.hpp>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "FireAudio.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Procedural-Audio");
	window.setFramerateLimit(144);
	ImGui::SFML::Init(window);

	sf::Vector2f fireCenter = { 960.f, 540.f };
	float fireRadius = 50.f;

	sf::CircleShape fireObject(fireRadius);
	fireObject.setOrigin({ fireRadius, fireRadius });
	fireObject.setFillColor(sf::Color::Yellow);
	fireObject.setPosition({ fireCenter });

	sf::CircleShape playerListener(30);
	playerListener.setOrigin({ 15, 15 });
	playerListener.setFillColor(sf::Color::Green);
	playerListener.setPosition({ 100, 100 });
	float moveSpeed = 1.0f;

	FireAudio fireAudio;
	fireAudio.setPosition({ fireCenter.x, fireCenter.y, 0.0f });
	fireAudio.setVolume(50.0f);
	fireAudio.play();

	// Visual representation of the "Full Volume" radius
	sf::CircleShape distanceVisual;
	distanceVisual.setFillColor(sf::Color(255, 0, 0, 30));
	distanceVisual.setOutlineColor(sf::Color::Red);
	distanceVisual.setOutlineThickness(2.f);

	// Visual representation of the "Inaudible" raduis
	sf::CircleShape maxDistanceVisual;
	maxDistanceVisual.setFillColor(sf::Color(255, 255, 255, 30));
	maxDistanceVisual.setOutlineColor(sf::Color::White);
	maxDistanceVisual.setOutlineThickness(2.f);

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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			playerListener.move({ 0.0f, -moveSpeed });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			playerListener.move({ 0.0f, moveSpeed });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			playerListener.move({ -moveSpeed, 0.0f });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			playerListener.move({ moveSpeed, 0.0f });
		}

		//update the listener
		//sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		//sf::Listener::setPosition({ (float)mousePos.x, (float)mousePos.y, 0.0f });
		sf::Listener::setPosition({ playerListener.getPosition().x, playerListener.getPosition().y, 100.0f });
		sf::Listener::setDirection({ 0.f, 0.f, -1.f });
		sf::Listener::setUpVector({ 0.f, 1.f, 0.f });

		ImGui::SFML::Update(window, deltaClock.restart());

		// --- ImGui Interface ---
		ImGui::Begin("Fire Sound Settings");
		ImGui::Text("Master Controls");

		//Master Volume Slider
		float currentVolume = fireAudio.getVolume();
		if (ImGui::SliderFloat("Master Volume", &currentVolume, 0.0f, 100.0f, "%.0f%%"))
		{
			fireAudio.setVolume(currentVolume);
		}
		ImGui::Separator();

		ImGui::Text("Adjust the layers to change the fire personality.");
		//sound component sliders
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
		ImGui::Separator();

		ImGui::Text("Adjust sound attenuation");
		//attenuation sliders
		if (ImGui::CollapsingHeader("Spatial Attenuation", ImGuiTreeNodeFlags_DefaultOpen)) 
		{
			float currentAtten = fireAudio.getAttenuation();
			if (ImGui::SliderFloat("Attenuation", &currentAtten, 10.0f, 20.0f))
			{
				fireAudio.setAttenuation(currentAtten);
			}

			float currentMinDist = fireAudio.getMinDistance();
			if (ImGui::SliderFloat("Min Distance", &currentMinDist, 50.0f, 100.0f)) 
			{
				fireAudio.setMinDistance(currentMinDist);
			}
		}
		ImGui::Separator();

		if (ImGui::Button("Reset to Default")) 
		{
			fireAudio.setVolume(50.0f);
			fireAudio.setMinDistance(60.f);
			fireAudio.setAttenuation(10.0f);
			fireAudio.crackleProbability = 0.00015f;
			fireAudio.crackleFilterFreq = 3500.0f;
			fireAudio.hissGain = 0.04f;
			fireAudio.hissFilterFreq = 8000.0f;
			fireAudio.roarGain = 0.35f;
			fireAudio.roarFilterFreq = 100.0f;
			fireAudio.lappingSpeed = 0.5f;
		}

		ImGui::End();

		//update visual cirlces
		float minDist = fireAudio.getMinDistance();
		float atten = fireAudio.getAttenuation();
		float threshold = 0.01f;

		//updated minDistance visual (red circle)
		distanceVisual.setRadius(minDist);
		distanceVisual.setOrigin({ minDist, minDist });
		distanceVisual.setPosition(fireCenter);

		//update maxDistance visual (white circle)
		float maxRadius = minDist;

		if (atten > 0.01f) {
			// This formula determines where volume drops to the threshold
			maxRadius = minDist + (minDist * (1.0f / threshold - 1.0f)) / atten;
		}
		else {
			maxRadius = 10000.0f; // Infinite
		}

		//maxRadius = minDist + (minDist * (1.0f / threshold - 1.0f)) / atten;

		maxDistanceVisual.setRadius(maxRadius);
		maxDistanceVisual.setOrigin({ maxRadius, maxRadius });
		maxDistanceVisual.setPosition(fireCenter);

		//rendering
		window.clear(sf::Color(10, 10, 10));
		window.draw(maxDistanceVisual);
		window.draw(distanceVisual);
		window.draw(fireObject);
		window.draw(playerListener);
		ImGui::SFML::Render(window);
		window.display();
	}

	fireAudio.stop();
	ImGui::SFML::Shutdown();
	return 0;
}