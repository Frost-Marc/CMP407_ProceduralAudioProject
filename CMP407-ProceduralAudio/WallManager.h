#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

class WallManger
{
public:

	struct Wall
	{
		sf::RectangleShape shape;
		bool isActive = false;
		std::string name;
		float muffleFreq = 600.0f;
	};

	std::vector<Wall> m_walls;

	WallManger(sf::Vector2f fireCenter)
	{
		addWall("North Wall", fireCenter + sf::Vector2f(-150, -150), { 300, 20 });
		addWall("South Wall", fireCenter + sf::Vector2f(-150, 150), { 300, 20 });
		addWall("West Wall", fireCenter + sf::Vector2f(-200, -150), { 20, 300 });
		addWall("East Wall", fireCenter + sf::Vector2f(200, -150), { 20, 300 });
	}

	void addWall(std::string name, sf::Vector2f pos, sf::Vector2f size)
	{
		Wall wall;
		wall.name = name;
		wall.shape.setSize(size);
		wall.shape.setPosition(pos);
		wall.shape.setFillColor(sf::Color(sf::Color::Blue));
		m_walls.push_back(wall);
	}

	float getOcclusionFrequency(sf::Vector2f lsitener, sf::Vector2f source)
	{
		float lowestFreq = 20000.0f;

		for (const auto& wall : m_walls)
		{
			if (!wall.isActive)
			{
				continue;
			}

			sf::FloatRect bounds = wall.shape.getGlobalBounds();
			for (float t = 0.05f; t < 0.95f; t += 0.05f)
			{
				sf::Vector2f point = lsitener + t * (source - lsitener);
				if (bounds.contains(point))
				{
					lowestFreq = std::min(lowestFreq, wall.muffleFreq);
					break;
				}
			}
		}
		return lowestFreq;
	};
};