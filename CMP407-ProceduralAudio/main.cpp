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
	fireObject.setOrigin({ 60.f, 60.f });
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

		// --- ImGui Interface ---
		ImGui::Begin("Fire Sound Settings");
		ImGui::Text("Adjust the layers to change the fire personality.");
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Wood & Crackle", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::SliderFloat("Crackle Probability", &fireAudio.crackleProbability, 0.0f, 0.001f, "%.5f");
			ImGui::SliderFloat("Crackle Filter", &fireAudio.crackleFilterFreq, 2000.0f, 8000.0f);
		}

		if (ImGui::CollapsingHeader("Gas & Hiss", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::SliderFloat("Hiss Volume", &fireAudio.hissGain, 0.0f, 0.2f);
			ImGui::SliderFloat("Hiss Filter", &fireAudio.hissFilterFreq, 5000.0f, 12000.0f);
		}

		if (ImGui::CollapsingHeader("Flame & Roar", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::SliderFloat("Roar Volume", &fireAudio.roarGain, 0.0f, 1.0f);
			ImGui::SliderFloat("Roar Filter", &fireAudio.roarFilterFreq, 10.0f, 500.0f);
			ImGui::SliderFloat("Lapping Speed", &fireAudio.lappingSpeed, 0.1f, 5.0f);
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
		window.draw(fireObject);
		ImGui::SFML::Render(window);
		window.display();
	}

	fireAudio.stop();
	ImGui::SFML::Shutdown();
	return 0;
}