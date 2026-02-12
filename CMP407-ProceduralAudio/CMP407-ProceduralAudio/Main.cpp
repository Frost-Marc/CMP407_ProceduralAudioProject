#include <SFML/System.hpp>
#include "FireSoundGen.h"
#include <iostream>

int main()
{
    /*sf::RenderWindow window(sf::VideoMode({ 500, 500 }), "ProceduralAudio");

    sf::CircleShape circle;
    circle.setRadius(50.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition({ 200,200 });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(circle);
        window.display();
    }*/

    FireSoundGen fire;

    fire.setIntensity(0.8f);
    fire.setCrackleRate(25.0f);
    fire.setBrightness(0.6f);
    fire.setFlickerSpeed(0.4f);

    fire.play();

    std::cout << "Procedural Fire Running...\n";
    std::cout << "Press Enter to quit.\n";
    std::cin.get();

	return 0;
}